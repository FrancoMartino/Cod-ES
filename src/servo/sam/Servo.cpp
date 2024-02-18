#if defined(ARDUINO_ARCH_SAM)

#include <Arduino.h>
#include "../Servo.h"

#define usToTicks(_us) ((clockCyclesPerMicrosecond() * _us) / 32)
#define ticksToUs(_ticks) (((unsigned)_ticks * 32) / clockCyclesPerMicrosecond())

#define TRIM_DURATION 2

static servo_t servos[MAX_SERVOS];

uint8_t ServoCount = 0;

static volatile int8_t Channel[_Nbr_16timers];

#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER))
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)
#define SERVO_INDEX(_timer, _channel) ((_timer * SERVOS_PER_TIMER) + _channel)
#define SERVO(_timer, _channel) (servos[SERVO_INDEX(_timer, _channel)])

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)

void Servo_Handler(timer16_Sequence_t timer, Tc *pTc, uint8_t channel);
#if defined(_useTimer1)
void HANDLER_FOR_TIMER1(void)
{
  Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
}
#endif
#if defined(_useTimer2)
void HANDLER_FOR_TIMER2(void)
{
  Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
}
#endif
#if defined(_useTimer3)
void HANDLER_FOR_TIMER3(void)
{
  Servo_Handler(_timer3, TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
}
#endif
#if defined(_useTimer4)
void HANDLER_FOR_TIMER4(void)
{
  Servo_Handler(_timer4, TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
}
#endif
#if defined(_useTimer5)
void HANDLER_FOR_TIMER5(void)
{
  Servo_Handler(_timer5, TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
}
#endif

void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel)
{

  tc->TC_CHANNEL[channel].TC_SR;
  if (Channel[timer] < 0)
  {
    tc->TC_CHANNEL[channel].TC_CCR |= TC_CCR_SWTRG;
  }
  else
  {
    if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && SERVO(timer, Channel[timer]).Pin.isActive == true)
    {
      digitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, LOW);
    }
  }

  Channel[timer]++;
  if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER)
  {
    tc->TC_CHANNEL[channel].TC_RA = tc->TC_CHANNEL[channel].TC_CV + SERVO(timer, Channel[timer]).ticks;
    if (SERVO(timer, Channel[timer]).Pin.isActive == true)
    {
      digitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, HIGH);
    }
  }
  else
  {

    if ((tc->TC_CHANNEL[channel].TC_CV) + 4 < usToTicks(REFRESH_INTERVAL))
    {
      tc->TC_CHANNEL[channel].TC_RA = (unsigned int)usToTicks(REFRESH_INTERVAL);
    }
    else
    {
      tc->TC_CHANNEL[channel].TC_RA = tc->TC_CHANNEL[channel].TC_CV + 4;
    }
    Channel[timer] = -1;
  }
}

static void _initISR(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn)
{
  pmc_enable_periph_clk(id);
  TC_Configure(tc, channel,
               TC_CMR_TCCLKS_TIMER_CLOCK3 |
                   TC_CMR_WAVE |
                   TC_CMR_WAVSEL_UP_RC);

  TC_SetRA(tc, channel, 2625);

  NVIC_EnableIRQ(irqn);

  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPAS;

  TC_Start(tc, channel);
}

static void initISR(timer16_Sequence_t timer)
{
#if defined(_useTimer1)
  if (timer == _timer1)
    _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, IRQn_FOR_TIMER1);
#endif
#if defined(_useTimer2)
  if (timer == _timer2)
    _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, ID_TC_FOR_TIMER2, IRQn_FOR_TIMER2);
#endif
#if defined(_useTimer3)
  if (timer == _timer3)
    _initISR(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3, ID_TC_FOR_TIMER3, IRQn_FOR_TIMER3);
#endif
#if defined(_useTimer4)
  if (timer == _timer4)
    _initISR(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4, ID_TC_FOR_TIMER4, IRQn_FOR_TIMER4);
#endif
#if defined(_useTimer5)
  if (timer == _timer5)
    _initISR(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5, ID_TC_FOR_TIMER5, IRQn_FOR_TIMER5);
#endif
}

static void finISR(timer16_Sequence_t timer)
{
#if defined(_useTimer1)
  TC_Stop(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
#endif
#if defined(_useTimer2)
  TC_Stop(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
#endif
#if defined(_useTimer3)
  TC_Stop(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
#endif
#if defined(_useTimer4)
  TC_Stop(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
#endif
#if defined(_useTimer5)
  TC_Stop(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
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
