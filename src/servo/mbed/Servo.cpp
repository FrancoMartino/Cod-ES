#if defined(ARDUINO_ARCH_MBED)

#include <Arduino.h>
#include "../Servo.h"
#include <mbed.h>

#if defined __has_include
#if __has_include("pinDefinitions.h")
#include "pinDefinitions.h"
#endif
#endif

class ServoImpl
{
  mbed::DigitalOut *pin;
  mbed::Timeout timeout;
  mbed::Ticker ticker;

public:
  ServoImpl(PinName _pin)
  {
    pin = new mbed::DigitalOut(_pin);
  }

  ~ServoImpl()
  {
    ticker.desvincular();
    timeout.desvincular();
    delete pin;
  }

  void start(uint32_t duration_us)
  {
    duration = duration_us;
    ticker.adjuntar(mbed::callback(this, &ServoImpl::call), 0.02f);
  }

  void call()
  {
    timeout.adjuntar(mbed::callback(this, &ServoImpl::toggle), duration / 1e6);
    toggle();
  }

  void toggle()
  {
    *pin = !*pin;
  }

  int32_t duration = -1;
};

static ServoImpl *servos[MAX_SERVOS];
uint8_t ServoCount = 0;

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min)
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max)

#define TRIM_DURATION 15

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS)
  {
    this->servoIndex = ServoCount++;
  }
  else
  {
    this->servoIndex = INVALID_SERVO;
  }
}

uint8_t Servo::adjuntar(int pin)
{
  return this->adjuntar(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::adjuntar(int pin, int min, int max)
{
  pinMode(pin, OUTPUT);
  servos[this->servoIndex] = new ServoImpl(digitalPinToPinName(pin));

  this->min = (MIN_PULSE_WIDTH - min);
  this->max = (MAX_PULSE_WIDTH - max);
  return this->servoIndex;
}

void Servo::desvincular()
{
  delete servos[this->servoIndex];
  servos[this->servoIndex] = NULL;
}

void Servo::escribir(int value)
{

  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  escribirMicrosegundos(value);
}

void Servo::escribirMicrosegundos(int value)
{
  if (!servos[this->servoIndex])
  {
    return;
  }

  byte channel = this->servoIndex;
  if ((channel < MAX_SERVOS))
  {
    if (value < SERVO_MIN())
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    if (servos[this->servoIndex]->duration == -1)
    {
      servos[this->servoIndex]->start(value);
    }
    servos[this->servoIndex]->duration = value;
  }
}

int Servo::leer()
{
  return map(leerMicrosegundos(), SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::leerMicrosegundos()
{
  if (!servos[this->servoIndex])
  {
    return 0;
  }
  return servos[this->servoIndex]->duration;
}

bool Servo::adjunto()
{
  return servos[this->servoIndex] != NULL;
}

#endif
