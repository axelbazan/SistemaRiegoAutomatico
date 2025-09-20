#include <Arduino.h>
#include <Ds1302.h>

#define PIN_ENA 3
#define PIN_CLK 4
#define PIN_DAT 2
#define TRIG 8
#define ECHO 10

// Nico
#define ValvulaLlenado 9
#define ValvulaVaciado 9

const int horaVerano = 07;
const int horaInvierno = 14;
const int profundidadBancal = 5;
const int distanciaDesagote = 10;

// Nico
const float profundidadRecipiente = 11; //cm
const float alturaAguaMenor = 5;
const float alturaAguaMayor = 6;

const int tiempoDeRemojo = 1; //minutos
const int tiempoEntreLlenados = 5; //minutos

long duracion;
float distancia;

bool llenadoHecho = false;
bool vaciadoHecho = false;

int llenadoTimeStamp;

int tiempoDesdeLlenado;

Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);


const static char* WeekDays[] =
{
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};


void setup()
{
  pinMode(ValvulaLlenado, OUTPUT);
  pinMode(ValvulaVaciado, OUTPUT);
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(ValvulaLlenado, HIGH);
  digitalWrite(ValvulaVaciado, HIGH);


    // initialize the RTC
    rtc.init();

    // test if clock is halted and set a date-time (see example 2) to start it
    
    if (rtc.isHalted()) //Nico: este dispositivo no tiene conexión a internet, no?
    {
        Serial.println("RTC is halted. Setting time...");

        Ds1302::DateTime dt = {
            .year = 25,
            .month = Ds1302::MONTH_SEP,
            .day = 17,
            .hour = 14,
            .minute = 40,
            .second = 30,
            .dow = Ds1302::DOW_WED
        };

        rtc.setDateTime(&dt);
    }
}



bool esHoraDeRiego(){

  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  int mesActual=now.month;
  int horaActual = now.hour;
  bool horaAdecuada = false;

 if ((mesActual <= 4) || (mesActual>=10)){ //Verano
  if ((horaActual == horaVerano)){
    horaAdecuada = true;
    Serial.println("Es hora de regar!");
    } 
  }
else { //Invierno
  if (horaActual == horaInvierno){ 
    horaAdecuada = true;
    Serial.println("Es hora de regar!");
 }
 else {
    Serial.println("NO es hora de regar!");
  }
 }

  return horaAdecuada;
}

float distanciaAgua(){ // Nico: Creo que debe ser float
  // Generar pulso de 10us en TRIG
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  
  // Nico: la distancia se podría calcular N veces, a una cierta frecuencia (0,1 Hz, por ejemplo) y devolver el promedio (o la mediana)
  
  int numeroMedidas = 1;
  float distanciaTotal = 0;
  
  for (int i = 0; i<numeroMedidas; i++){
  
    // Medir el tiempo en microsegundos
    duracion = pulseIn(ECHO, HIGH);

    // Calcular la distancia (velocidad del sonido ~340 m/s)
    distancia = duracion * 0.034 / 2;
    //Serial.println(distancia);

    distanciaTotal += distancia;
    Serial.print("Distancia Total: ");
    Serial.println(distanciaTotal);

    delay(1000);
  }

  distancia = distanciaTotal/numeroMedidas;



  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  return (distancia);

}


// Nico: convierto a profundid de agua
float alturaAgua (){
  return profundidadRecipiente - distanciaAgua();
}

void diaYHora(){
  
  // get the current time
    Ds1302::DateTime now;
    rtc.getDateTime(&now);

    static uint8_t last_second = 0;
  if (last_second != now.second)
    {
        last_second = now.second;

        Serial.print("20");
        Serial.print(now.year);    // 00-99
        Serial.print('-');
        if (now.month < 10) Serial.print('0');
        Serial.print(now.month);   // 01-12
        Serial.print('-');
        if (now.day < 10) Serial.print('0');
        Serial.print(now.day);     // 01-31
        Serial.print(' ');
        Serial.print(WeekDays[now.dow - 1]); // 1-7
        Serial.print(' ');
        if (now.hour < 10) Serial.print('0');
        Serial.print(now.hour);    // 00-23
        Serial.print(':');
        if (now.minute < 10) Serial.print('0');
        Serial.print(now.minute);  // 00-59
        Serial.print(':');
        if (now.second < 10) Serial.print('0');
        Serial.print(now.second);  // 00-59
        Serial.println();
  }
}

// Nico
long timeStampEnMinutos(){
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  long timeStamp = now.hour *3600 + now.minute*60 + now.second;
  Serial.print("Timestamp:");
  Serial.println(timeStamp);
  return timeStamp;
}

void loop() {



  //diaYHora();
  // Version Nico
  if (esHoraDeRiego()){
    float alturaActual = alturaAgua();
    Serial.println("Es hora de riego");
    Serial.print(alturaActual);
    Serial.println();

    if (!llenadoHecho){ // Todavia no se hizo el llenado
      Serial.println("Todavia no se hizo el llenado");
      Serial.print("Nivel:");
      Serial.print(alturaActual);
      Serial.println();

      if (alturaActual < alturaAguaMenor){ // Si la profundidad actual es menor a la deseada
        Serial.println("Altura menor que altura menor");
        digitalWrite(ValvulaLlenado,LOW); // Valvula abierta -- llenando
        while (alturaActual < alturaAguaMenor){ // Mientras la profundidad actual siga siendo menor a la deseada
          // Espero y voy midiendo profundidad...
          delay(1000);
          alturaActual = alturaAgua();
          Serial.print("Nivel: ");
          Serial.print(alturaActual);
          Serial.println();
        }
        Serial.println("Altura suficiente");
        digitalWrite(ValvulaLlenado, HIGH);
        llenadoTimeStamp = timeStampEnMinutos(); // Guardo el minuto en el cual se completo el llenado
        llenadoHecho = true;  // Llenado completo
      }
    
    }
    else { //Ya se hizo un llenado
      Serial.println("Ya se hizo un llenado");
      if (!vaciadoHecho){
        Serial.println("Aún no se hizo el vaciado");
        tiempoDesdeLlenado = timeStampEnMinutos()-llenadoTimeStamp; // Tiempo en minutos que paso desde el llenado
        /*while (tiempoDesdeLlenado < tiempoDeRemojo){ // Mientras no haya pasado el tiempo de llenado...
          // Espero...
          delay(1000); // Espero 1 minuto y actualizo el tiempo desde llenado
          tiempoDesdeLlenado = timeStampEnMinutos()-llenadoTimeStamp;
          Serial.print("Tiempo desde el llenado:");
          Serial.println(tiempoDesdeLlenado);
        }
        */
        delay(2000);
        Serial.println("Tiempo de remojo alcanzado, vaciando...");
        // Cuando pasó el tiempo abro la valvula
        digitalWrite(ValvulaVaciado, LOW);
        alturaActual = alturaAgua();
        
        if (alturaActual > alturaAguaMenor){
          while (alturaActual > alturaAguaMenor){ //Mientras la profundidad sea mayor a la alturaAguaMenor
            // Espero y voy midiendo profundidad...
            delay(1000);
            alturaActual = alturaAgua();
            Serial.print("Nivel: ");
            Serial.print(alturaActual);
          }
        
          // Cierro la valvula.
          Serial.println("Valvula cerrada");
          digitalWrite(ValvulaVaciado, HIGH);
        }

      Serial.println("Vaciado hecho");
      vaciadoHecho = true;
      
      }    
    }    
  }
  
  else {
    if (llenadoHecho || vaciadoHecho){
    // Nico: si cuando llega el horario de no riego y el riego fue hecho, reinicio su estado.
    llenadoHecho = false;
    vaciadoHecho = false;
    }
  } 
  
  delay(3000);  
}
