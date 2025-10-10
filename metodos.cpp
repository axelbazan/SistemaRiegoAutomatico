//Metodos 
#include "metodos.h"


//Variables de uso global
float ProfundidadRecipiente = 6;
float NivelAguaMaximo = 4;
float NivelAguaMinimo = 1;
long TiempoRemojo = 15000; // 15 segundos
long IntervaloLoop = 20000; // 20 segundos

// Definición del objeto Reljo RTC tipo Ds1302
Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);

//Nétodo para inicializar la hora en el módulo del reloj. Se ejecuta por única vez dentro del setup
void iniciarReloj() {
  rtc.init();
  if (rtc.isHalted()) {
    Ds1302::DateTime dt = {
      .year = 25,
      .month = Ds1302::MONTH_SEP,
      .day = 10,
      .hour = 14,
      .minute = 10,
      .second = 30,
      .dow = Ds1302::DOW_WED
    };
    rtc.setDateTime(&dt);
  }
}

//Método para medir la distancia del sensor de ultrasonido hasta la superficie del agua
float distanciaAgua() {
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);

  long duracion = pulseIn(ECHO1, HIGH);
  float distancia = duracion * 0.034 / 2;
  return distancia;
}

//Método para medir la distancia actual del nivel de agua, que resulta de la diferencia entre la profundidad del recipiente y la distancia entre el sensor y el agua
float nivelAguaActual() {
  return ProfundidadRecipiente - distanciaAgua();
}

//Método para mostrar el nivel actual de agua
void mostrarNivel() {
    Serial.print("Nivel actual: ");
    Serial.println(nivelAguaActual());
}

//Método para activar o abrir las válvulas/bombas
void activar(int NumeroPin) {
  digitalWrite(NumeroPin, HIGH);
} 

//Método para desactivar o cerrar las válvulas/bombas
void desactivar(int NumeroPin) {
  digitalWrite(NumeroPin, LOW);
}

//void cambiarEstado(int NumeroPin) {
  //  if (digitalRead(NumeroPin) == HIGH) 
    //    digitalWrite(NumeroPin, LOW);
    //else
      //  digitalWrite(NumeroPin, HIGH);
//}

//Método para chequear si es hora de regar. Compara la hora y el mes actual, porque la hora de riego varía si es invierno o verano
bool esHoraDeRiego() {
  bool horaAdecuada = false;
  int horaVerano = 7;
  int horaInvierno = 18;
  Ds1302::DateTime ahora;
  rtc.getDateTime(&ahora);
  int mesActual = ahora.month;
  int horaActual = ahora.hour;
  Serial.println(mesActual);
  Serial.println(horaActual);

  if ((mesActual <= 4) || (mesActual >= 10)){ // Verano
    if (horaActual == horaVerano){
      horaAdecuada = true;
    }
  }
  else{  // Invierno
    if (horaActual == horaInvierno){
      horaAdecuada = true;
    } 
  }
  return horaAdecuada;
}

//Método para realizar el llenado del bancal hasta la altura máxima predefinida
void realizarLlenado() {
  float nivelH2OActual = nivelAguaActual();
  Serial.println(nivelH2OActual);
  if (nivelH2OActual < NivelAguaMaximo) {
    Serial.println("Iniciando llenado.");
    activar(VL1);
    activar(VL2);
    activar(VL3);
    while (nivelAguaActual() < NivelAguaMaximo) {
      delay(2000);
      Serial.println("Llenando...");
      mostrarNivel();
    }
    desactivar(VL1);
    desactivar(VL2);
    desactivar(VL3);
  }
  //LlenadoHecho = true;
  Serial.println("llenado realizado.");
}

//Método para esperar un tiempo con el bancal lleno de agua hasta que se mojen los plantines
void realizarRemojo() {
  Serial.print("Esperando ");
  Serial.print(TiempoRemojo);
  Serial.println("ms de remojo."); 
  delay(TiempoRemojo);
  Serial.println("Tiempo de remojo completado.");
  delay(500);
}

//Método para realizar el vaciado del bancal hasta la altura mínima predefinida
void realizarVaciado() {
  Serial.println("Iniciando vaciado.");
  activar(BV1);
  activar(BV2);
  activar(BV3);
  while (nivelAguaActual() > NivelAguaMinimo) {
    delay(2000);
    Serial.println("Vaciando...");
    mostrarNivel();
  }
  desactivar(BV1);
  desactivar(BV2);
  desactivar(BV3);
  //VaciadoHecho = true;
  Serial.println("Vaciado realizado.");
  delay(500);
}
