#if defined(ARDUINO_ARCH_AVR)

#include <avr/interrupt.h>
#include <Arduino.h>

#include "../Servo.h"

#define usToTicks(_us) ((clockCyclesPerMicrosecond() * _us) / 8)
#define ticksToUs(_ticks) (((unsigned)_ticks * 8) / clockCyclesPerMicrosecond())

#define TRIM_DURATION 2

static servo_t servos[MAX_SERVOS];
static volatile int8_t Channel[_Nbr_16timers];

uint8_t ServoCount = 0;

#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER))
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)
#define SERVO_INDEX(_timer, _channel) ((_timer * SERVOS_PER_TIMER) + _channel)
#define SERVO(_timer, _channel) (servos[SERVO_INDEX(_timer, _channel)])

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)

/************ static functions common to all instances ***********************/

static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t *OCRnA)
{
  if (Channel[timer] < 0)
    *TCNTn = 0;
  else
  {
    if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && SERVO(timer, Channel[timer]).Pin.isActive == true)
      digitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, LOW);
  }

  Channel[timer]++;
  if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER)
  {
    *OCRnA = *TCNTn + SERVO(timer, Channel[timer]).ticks;
    if (SERVO(timer, Channel[timer]).Pin.isActive == true)
      digitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, HIGH);
  }
  else
  {

    if (((unsigned)*TCNTn) + 4 < usToTicks(REFRESH_INTERVAL))
      *OCRnA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    else
      *OCRnA = *TCNTn + 4;
    Channel[timer] = -1;
  }
}

#ifndef WIRING

#if defined(_useTimer1)
SIGNAL(TIMER1_COMPA_vect)
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif

#if defined(_useTimer3)
SIGNAL(TIMER3_COMPA_vect)
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif

#if defined(_useTimer4)
SIGNAL(TIMER4_COMPA_vect)
{
  handle_interrupts(_timer4, &TCNT4, &OCR4A);
}
#endif

#if defined(_useTimer5)
SIGNAL(TIMER5_COMPA_vect)
{
  handle_interrupts(_timer5, &TCNT5, &OCR5A);
}
#endif

#elif defined WIRING

#if defined(_useTimer1)
void Timer1Service()
{
  handle_interrupts(_timer1, &TCNT1, &OCR1A);
}
#endif
#if defined(_useTimer3)
void Timer3Service()
{
  handle_interrupts(_timer3, &TCNT3, &OCR3A);
}
#endif
#endif

static void initISR(timer16_Sequence_t timer)
{
#if defined(_useTimer1)
  if (timer == _timer1)
  {
    TCCR1A = 0;
    TCCR1B = _BV(CS11);
    TCNT1 = 0;
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF1A);
    TIMSK |= _BV(OCIE1A);
#else

    TIFR1 |= _BV(OCF1A);
    TIMSK1 |= _BV(OCIE1A);
#endif
#if defined(WIRING)
    timerAttach(TIMER1OUTCOMPAREA_INT, Timer1Service);
#endif
  }
#endif

#if defined(_useTimer3)
  if (timer == _timer3)
  {
    TCCR3A = 0;
    TCCR3B = _BV(CS31);
    TCNT3 = 0;
#if defined(__AVR_ATmega128__)
    TIFR |= _BV(OCF3A);
    ETIMSK |= _BV(OCIE3A);
#else
    TIFR3 = _BV(OCF3A);
    TIMSK3 = _BV(OCIE3A);
#endif
#if defined(WIRING)
    timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);
#endif
  }
#endif

#if defined(_useTimer4)
  if (timer == _timer4)
  {
    TCCR4A = 0;
    TCCR4B = _BV(CS41);
    TCNT4 = 0;
    TIFR4 = _BV(OCF4A);
    TIMSK4 = _BV(OCIE4A);
  }
#endif

#if defined(_useTimer5)
  if (timer == _timer5)
  {
    TCCR5A = 0;
    TCCR5B = _BV(CS51);
    TCNT5 = 0;
    TIFR5 = _BV(OCF5A);
    TIMSK5 = _BV(OCIE5A);
  }
#endif
}

static void finISR(timer16_Sequence_t timer)
{

#if defined WIRING
  if (timer == _timer1)
  {
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
    TIMSK1 &= ~_BV(OCIE1A);
#else
    TIMSK &= ~_BV(OCIE1A);
#endif
    timerDetach(TIMER1OUTCOMPAREA_INT);
  }
  else if (timer == _timer3)
  {
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);
#else
    ETIMSK &= ~_BV(OCIE3A);
#endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else

  (void)timer;
#endif
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
    this->servoIndex = INVALID_SERVO;
}

uint8_t Servo::adjuntar(int pin)
{
  return this->adjuntar(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::adjuntar(int pin, int min, int max)
{
  if (this->servoIndex < MAX_SERVOS)
  {
    pinMode(pin, OUTPUT);
    servos[this->servoIndex].Pin.nbr = pin;

    this->min = (MIN_PULSE_WIDTH - min) / 4;
    this->max = (MAX_PULSE_WIDTH - max) / 4;

    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false)
      initISR(timer);
    servos[this->servoIndex].Pin.isActive = true;
  }
  return this->servoIndex;
}

void Servo::desvincular()
{
  servos[this->servoIndex].Pin.isActive = false;
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
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
    if (value > 180)
      value = 180;
    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  this->escribirMicrosegundos(value);
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

    uint8_t oldSREG = SREG;
    cli();
    servos[channel].ticks = value;
    SREG = oldSREG;
  }
}

int Servo::leer()
{
  return map(this->leerMicrosegundos() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
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
