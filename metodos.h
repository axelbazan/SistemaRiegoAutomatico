#ifndef METODOS_H
#define METODOS_H

#include <Arduino.h>
#include <Ds1302.h>

extern Ds1302 rtc;
extern long IntervaloLoop;

//pines del módulo de reloj
#define PIN_ENA 3
#define PIN_CLK 4
#define PIN_DAT 2

#define TRIG1 10  //Trigger del sensor de ultrasonido 1
#define ECHO1 8  //Echo del sensor de ultrasonido 1

#define VL1 13  //VL = válvula de llenado 1 (Led ROJO)
#define VL2 12  //VL = válvula de llenado 2 (Led AMARILLO)
#define VL3 11  //VL = válvula de llenado 3 (Led VERDE)
#define BV1 7  //BV = Bomba de vaciado 1 (Led ROJO)
#define BV2 6  //BV = Bomba de vaciado 2 (Led AMARILLO)
#define BV3 5  //BV = Bomba de vaciado 3 (Led VERDE)

//declarar las funciones y su tipo
void iniciarReloj();
float distanciaAgua();
float nivelAguaActual();
void mostrarNivel();
void activar(int NumeroPin);
void desactivar(int NumeroPin);
bool esHoraDeRiego();
void realizarLlenado();
void realizarRemojo();
void realizarVaciado();

#endif