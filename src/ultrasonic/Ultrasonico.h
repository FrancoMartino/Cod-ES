#ifndef Ultrasonico_h
#define Ultrasonico_h

#include "Arduino.h"

class Ultrasonico{
	public:
		Ultrasonico(int trigPin, int echoPin);
		float obtenerDistancia();
	private:
		int _trigPin;
		int _echoPin;
};

#endif