//Codigo principal del sistema de riego automatizado con controlador Arduino Uno.

#define PIN_SENSOR_ULTRASONIDO A1
#define PIN_VALVULA_DESAGOTE 1
#define PIN_VALVULA_RIEGO 2

bool yaSeRegoHoy = false;

void setup() {
  //Determina la configuracion inicial de los elementos conectados en los pines del controlador
	pinMode(PIN_SENSOR_ULTRASONIDO, INPUT);

	pinMode(PIN_VALVULA_DESAGOTE, OUTPUT);
	pinMode(PIN_VALVULA_RIEGO, OUTPUT);
  
	digitalWrite(PIN_VALVULA_DESAGOTE, LOw);
	digitalWrite(PIN_VALVULA_RIEGO, LOW);
}

void loop() {  
	DateTime hoy = rtc.now();
	if (esHoraRiego(hoy) && !yaSeRegoHoy) {
		if (condicionesAdecuadas()) {
			ejecutarRiego();
			delay(20*60*1000); // Espera 20 minutos.
			ejecutarDesagote();
			yaSeRegoHoy = true;
		}
	}
	
	//Reset para el dia siguiente
	if (esHoraReseteo(hoy) && yaSeRegoHoy) {
		yaSeRegoHoy = false;
	}
}


bool esHoraRiego(DateTime hoy) {
	//Establece la hora de riego ADECUADA dependiendo de la estación del año.
	//Retorna true si la hora de riego es la adecuada.
}

bool esHoraReseteo(DateTime hoy) {
	//Se puede resetear cuando es de noche xej
}

bool condicionesAdecuadas() {
	//si esta lloviendo settea el boolean de riego en true
	//si no hay energia suficiente no se riega y se espera a que la haya
	
}

bool energiaSuficiente() {
	//compara la demanda del sistema en funcionamiento con la energia acumulada en las baterias
}

bool nivelAguaAlcanzado() {
	//Analiza el nivel de agua del pileton utilizando el sensor de ultrasonido
	//Establece si el nivel es adecuado o no
}

void ejecutarRiego() {
	//abre la valvula de llenado y cierra la de desagote
	//se espera hasta alcanzar el nivel de agua deseado
	//cierra la valvula de llenado 
}
void ejecutarDesagote() {
	//abre la valvula de desagote
}
