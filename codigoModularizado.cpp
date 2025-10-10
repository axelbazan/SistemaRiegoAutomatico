#include <Arduino.h>
#include <Ds1302.h>

//Asignacion de pines digitales
#define PIN_ENA 3
#define PIN_CLK 4
#define PIN_DAT 2
#define UltraSonido_TRIG 12
#define UltraSonido_ECHO 11
#define ValvulaLlenado 8
#define BombaVaciado 8

//Variables de uso global
Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);
bool LlenadoHecho = false;
bool VaciadoHecho = false;
float ProfundidadRecipiente = 7;
float NivelAguaMaximo = 5;
float NivelAguaMinimo = 1;
long TiempoRemojo = 60 * 1000; // 1 minuto en microsegundos
long IntervaloLoop = 60 * 60 * 1000 // 1 hora en ms

const static char* WeekDays[] = { "Lunes","Martes","Miercoles","Jueves","Viernes","Sabado","Domingo" };

//Metodos del setup
void iniciarReloj() {
  rtc.init();
  if (rtc.isHalted()) {
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

//Metodos del programa loop
float distanciaAgua() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duracion = pulseIn(ECHO, HIGH);
  distancia = duracion * 0.034 / 2;
  return distancia;
}

float nivelAguaActual() {
  return ProfundidadRecipiente - distanciaAgua();
}

void mostrarNivel() {
    Serial.print("Nivel actual: ");
    Serial.println(nivelAguaActual());
}

bool esHoraDeRiego() {
  bool horaAdecuada = false;
  int horaVerano = 7;
  int horaInvierno = 14;
  Ds1302::DateTime ahora;
  rtc.getDateTime(&ahora);
  int mesActual = ahora.month;
  int horaActual = ahora.hour;

  if ((mesActual <= 4) || (mesActual >= 10)) // Verano
    if (horaActual == horaVerano) horaAdecuada = true;
  else  // Invierno
    if (horaActual == horaInvierno) horaAdecuada = true;
  
  return horaAdecuada;
}

void activar(int NumeroPin) {
  digitalWrite(NumeroPin, LOW);
} 

void desactivar(int NumeroPin) {
  digitalWrite(NumeroPin, HIGH);
}

void cambiarEstado(int NumeroPin) {
    if (digitalRead(NumeroPin) == HIGH) 
        digitalWrite(NumeroPin, LOW);
    else
        digitalWrite(NumeroPin, HIGH);
}

void realizarLlenado() {
  Serial.println("iniciando llenado.");
  if (nivelAguaActual() < NivelAguaMaximo) {
    activar(ValvulaLlenado);
    while (nivelAguaActual() < NivelAguaMaximo) {
      delay(2000);
      mostrarNivel();
    }
    desactivar(ValvulaLlenado);
  }
  LlenadoHecho = true;
  Serial.println("llenado realizado.");
}

void realizarRemojo() {
  Serial.println("Esperando "+ TiempoRemojo + "ms de remojo."); 
  delay(TiempoRemojo);
  Serial.println("Tiempo de remojo completado.");
}

void realizarVaciado() {
  Serial.println("iniciando vaciado.");
  activar(BombaVaciado);
  while (nivelAguaActual() < NivelAguaMinimo) {
    delay(2000);
    mostrarNivel();
  }
  desactivar(BombaVaciado);
  VaciadoHecho = true;
  Serial.println("vaciado realizado.");
}

void setup() {
  Serial.begin(9600);
  pinMode(UltraSonido_TRIG, OUTPUT);
  pinMode(UltraSonido_ECHO, INPUT);
  pinMode(ValvulaLlenado, OUTPUT);
  pinMode(BombaVaciado, OUTPUT);
  iniciarReloj();
}

void loop() {
  if (esHoraDeRiego()) {
    mostrarNivel();
    if (!LlenadoHecho)
      realizarLlenado();
    realizarRemojo();
    if (!VaciadoHecho)
      realizarVaciado();
  } else {
    // Reinicio estado cuando termina la franja de riego
    if (LlenadoHecho || VaciadoHecho) {
      LlenadoHecho = false;
      VaciadoHecho = false;
      Serial.println("Fuera de hora de riego → reseteo estados de riego");
    }
  }
  delay(IntervaloLoop);
}
