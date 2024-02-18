#ifndef Servo_h
#define Servo_h

#include <inttypes.h>

#if defined(ARDUINO_ARCH_AVR)
#include "avr/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAM)
#include "sam/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAMD)
#include "samd/ServoTimers.h"
#elif defined(ARDUINO_ARCH_STM32F4)
#include "stm32f4/ServoTimers.h"
#elif defined(ARDUINO_ARCH_NRF52)
#include "nrf52/ServoTimers.h"
#elif defined(ARDUINO_ARCH_MEGAAVR)
#include "megaavr/ServoTimers.h"
#elif defined(ARDUINO_ARCH_MBED)
#include "mbed/ServoTimers.h"
#else
#error "This library only supports boards with an AVR, SAM, SAMD, NRF52 or STM32F4 processor."
#endif

#define Servo_VERSION 2

#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400
#define DEFAULT_PULSE_WIDTH 1500
#define REFRESH_INTERVAL 20000

#define SERVOS_PER_TIMER 12
#define MAX_SERVOS (_Nbr_16timers * SERVOS_PER_TIMER)

#define INVALID_SERVO 255

#if !defined(ARDUINO_ARCH_STM32F4)

typedef struct
{
  uint8_t nbr : 6;
  uint8_t isActive : 1;
} ServoPin_t;

typedef struct
{
  ServoPin_t Pin;
  volatile unsigned int ticks;
} servo_t;

class Servo
{
public:
  Servo();
  uint8_t adjuntar(int pin);
  uint8_t adjuntar(int pin, int min, int max);
  void desvincular();
  void escribir(int value);
  void escribirMicrosegundos(int value);
  int leer();
  int leerMicrosegundos();
  bool adjunto();

private:
  uint8_t servoIndex;
  int8_t min;
  int8_t max;
};

#endif
#endif
