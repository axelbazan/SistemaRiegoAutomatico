#include <Arduino.h>
#include <Ds1302.h>
#include "metodos.h"

//Al inicializar el programa, estas variables son falsas porque todavía no se hizo el vaciado ni el llenado del bancal
bool LlenadoHecho = false;
bool VaciadoHecho = false;

//Inicializamos las válvulas, las bombas y el trigger como output. El Echo como entrada
void setup() {
  Serial.begin(9600);   //iniciamos el puerto serie para poder ver lo que está haciendo el sistema
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(VL1, OUTPUT);
  pinMode(VL2, OUTPUT);
  pinMode(VL3, OUTPUT);
  pinMode(BV1, OUTPUT);
  pinMode(BV2, OUTPUT);
  pinMode(BV3, OUTPUT);
  iniciarReloj();   //iniciamos el reloj
  Serial.println("Iniciando sistema");
  delay(500);
  digitalWrite(VL1, LOW);
  digitalWrite(VL2, LOW);
  digitalWrite(VL3, LOW);
  digitalWrite(BV1, LOW);
  digitalWrite(BV2, LOW);
  digitalWrite(BV3, LOW);
  Serial.println("Válvulas apagadas");  //al inicio las válvulas y bombas están apagadas
  delay(500);
  
}

//este código se ejecuta en loop siempre que el Arduino tenga energía
void loop() {
  Serial.println("Chequeando la hora de riego");
  delay(500);
    
  if (esHoraDeRiego()) {    //verificar si es hora de Regar
    Serial.println("Es hora de riego");
    mostrarNivel();         //medir el nivel de agua    
    if (!LlenadoHecho){     
      realizarLlenado();    //regar si aún no se había realizado         
      //LlenadoHecho = true;
      realizarRemojo();     //esperar que se mojen los plantines
    }
    if (!VaciadoHecho){
      realizarVaciado();    //vaciar si no se había hecho
      //VaciadoHecho = true;
      
    }
  } //else {
    // Reinicio estado cuando termina la franja de riego
      //if (LlenadoHecho || VaciadoHecho) {
        //LlenadoHecho = false;
        //VaciadoHecho = false;
        //Serial.println("Fuera de hora de riego → reseteo estados de riego");
      //}
    //}
  Serial.println("Espera hasta nuevo riego");
  delay(IntervaloLoop);
}