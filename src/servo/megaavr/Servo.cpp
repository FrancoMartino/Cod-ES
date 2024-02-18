#if defined(ARDUINO_ARCH_MEGAAVR)

#include <Arduino.h>
#include "../Servo.h"

#define usToTicks(_us) ((clockCyclesPerMicrosecond() / 16 * _us) / 4)
#define ticksToUs(_ticks) (((unsigned)_ticks * 16) / (clockCyclesPerMicrosecond() / 4))

#define TRIM_DURATION 5

static servo_t servos[MAX_SERVOS];

uint8_t ServoCount = 0;

static volatile int8_t currentServoIndex[_Nbr_16timers];

#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER))
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)
#define SERVO_INDEX(_timer, _channel) ((_timer * SERVOS_PER_TIMER) + _channel)
#define SERVO(_timer, _channel) (servos[SERVO_INDEX(_timer, _channel)])

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)

#undef REFRESH_INTERVAL
#define REFRESH_INTERVAL 16000

void ServoHandler(int timer)
{
  if (currentServoIndex[timer] < 0)
  {

    _timer->CCMP = 0;
  }
  else
  {
    if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && SERVO(timer, currentServoIndex[timer]).Pin.isActive == true)
    {
      digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, LOW);
    }
  }

  currentServoIndex[timer]++;

  if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && currentServoIndex[timer] < SERVOS_PER_TIMER)
  {
    if (SERVO(timer, currentServoIndex[timer]).Pin.isActive == true)
    {
      digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, HIGH);
    }

    uint16_t tcCounterValue = 0;
    _timer->CCMP = (uint16_t)(tcCounterValue + SERVO(timer, currentServoIndex[timer]).ticks);
  }
  else
  {

    uint16_t tcCounterValue = _timer->CCMP;

    if (tcCounterValue + 4UL < usToTicks(REFRESH_INTERVAL))
    {
      _timer->CCMP = (uint16_t)usToTicks(REFRESH_INTERVAL);
    }
    else
    {
      _timer->CCMP = (uint16_t)(tcCounterValue + 4UL);
    }

    currentServoIndex[timer] = -1;
  }

  _timer->INTFLAGS = TCB_CAPT_bm;
}

#if defined USE_TIMERB0
ISR(TCB0_INT_vect)
#elif defined USE_TIMERB1
ISR(TCB1_INT_vect)
#elif defined USE_TIMERB2
ISR(TCB2_INT_vect)
#endif
{
  ServoHandler(0);
}

static void initISR(timer16_Sequence_t timer)
{

  _timer->CTRLA = TCB_CLKSEL_CLKTCA_gc;

  _timer->CTRLB = TCB_CNTMODE_INT_gc;

  _timer->INTCTRL = TCB_CAPTEI_bm;

  _timer->CTRLA |= TCB_ENABLE_bm;
}

static void finISR(timer16_Sequence_t timer)
{

  _timer->INTCTRL = 0;
}

static boolean isTimerActive(timer16_Sequence_t timer)
{

  for (uint8_t channel = 0; channel < SERVOS_PER_TIMER; channel++)
  {
    if (SERVO(timer, channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS)
  {
    this->servoIndex = ServoCount++;
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);
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
  timer16_Sequence_t timer;

  if (this->servoIndex < MAX_SERVOS)
  {
    pinMode(pin, OUTPUT);
    servos[this->servoIndex].Pin.nbr = pin;

    this->min = (MIN_PULSE_WIDTH - min) / 4;
    this->max = (MAX_PULSE_WIDTH - max) / 4;

    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false)
    {
      initISR(timer);
    }
    servos[this->servoIndex].Pin.isActive = true;
  }
  return this->servoIndex;
}

void Servo::desvincular()
{
  timer16_Sequence_t timer;

  servos[this->servoIndex].Pin.isActive = false;
  timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if (isTimerActive(timer) == false)
  {
    finISR(timer);
  }
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

  byte channel = this->servoIndex;
  if ((channel < MAX_SERVOS))
  {
    if (value < SERVO_MIN())
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);
    servos[channel].ticks = value;
  }
}

int Servo::leer()
{
  return map(leerMicrosegundos() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::leerMicrosegundos()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks) + TRIM_DURATION;
  else
    pulsewidth = 0;

  return pulsewidth;
}

bool Servo::adjunto()
{
  return servos[this->servoIndex].Pin.isActive;
}

#endif
