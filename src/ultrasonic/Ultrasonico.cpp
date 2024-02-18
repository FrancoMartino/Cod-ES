#include "Ultrasonico.h"

Ultrasonico::Ultrasonico(int trigPin, int echoPin){
	_trigPin = trigPin;
	_echoPin = echoPin;

	pinMode(_trigPin, OUTPUT);
	pinMode(_echoPin, INPUT);
}

float Ultrasonico::obtenerDistancia(){
	digitalWrite(_trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(_trigPin, LOW);

	long tiempoEcho = pulseIn(_echoPin, HIGH);

	float distanciaCentimetros = (tiempoEcho * 0.0343) / 2;

	return distanciaCentimetros;
}