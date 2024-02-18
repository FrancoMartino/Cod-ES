

#if defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include "../Servo.h"

#define usToTicks(_us) ((clockCyclesPerMicrosecond() * _us) / 16)
#define ticksToUs(_ticks) (((unsigned)_ticks * 16) / clockCyclesPerMicrosecond())

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

#define WAIT_TC16_REGS_SYNC(x)           \
  while (x->COUNT16.STATUS.bit.SYNCBUSY) \
    ;

void Servo_Handler(timer16_Sequence_t timer, Tc *pTc, uint8_t channel, uint8_t intFlag);
#if defined(_useTimer1)
void HANDLER_FOR_TIMER1(void)
{
  Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, INTFLAG_BIT_FOR_TIMER_1);
}
#endif
#if defined(_useTimer2)
void HANDLER_FOR_TIMER2(void)
{
  Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, INTFLAG_BIT_FOR_TIMER_2);
}
#endif

void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel, uint8_t intFlag)
{
  if (currentServoIndex[timer] < 0)
  {
    tc->COUNT16.COUNT.reg = (uint16_t)0;
    WAIT_TC16_REGS_SYNC(tc)
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

    uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
    WAIT_TC16_REGS_SYNC(tc)

    tc->COUNT16.CC[channel].reg = (uint16_t)(tcCounterValue + SERVO(timer, currentServoIndex[timer]).ticks);
    WAIT_TC16_REGS_SYNC(tc)
  }
  else
  {

    uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
    WAIT_TC16_REGS_SYNC(tc)

    if (tcCounterValue + 4UL < usToTicks(REFRESH_INTERVAL))
    {
      tc->COUNT16.CC[channel].reg = (uint16_t)usToTicks(REFRESH_INTERVAL);
    }
    else
    {
      tc->COUNT16.CC[channel].reg = (uint16_t)(tcCounterValue + 4UL);
    }
    WAIT_TC16_REGS_SYNC(tc)

    currentServoIndex[timer] = -1;
  }

  tc->COUNT16.INTFLAG.reg = intFlag;
}

static inline void resetTC(Tc *TCx)
{

  TCx->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  WAIT_TC16_REGS_SYNC(TCx)

  TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  WAIT_TC16_REGS_SYNC(TCx)
  while (TCx->COUNT16.CTRLA.bit.SWRST)
    ;
}

static void _initISR(Tc *tc, uint8_t channel, uint32_t id, IRQn_Type irqn, uint8_t gcmForTimer, uint8_t intEnableBit)
{

  GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(gcmForTimer));
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  resetTC(tc);

  tc->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

  tc->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_NPWM;

  tc->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV16;

  tc->COUNT16.CTRLBCLR.bit.DIR = 1;
  WAIT_TC16_REGS_SYNC(tc)

  tc->COUNT16.CC[channel].reg = (uint16_t)usToTicks(1000UL);
  WAIT_TC16_REGS_SYNC(tc)

  NVIC_DisableIRQ(irqn);
  NVIC_ClearPendingIRQ(irqn);
  NVIC_SetPriority(irqn, 0);
  NVIC_EnableIRQ(irqn);

  tc->COUNT16.INTENSET.reg = intEnableBit;

  tc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  WAIT_TC16_REGS_SYNC(tc)
}

static void initISR(timer16_Sequence_t timer)
{
#if defined(_useTimer1)
  if (timer == _timer1)
    _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, IRQn_FOR_TIMER1, GCM_FOR_TIMER_1, INTENSET_BIT_FOR_TIMER_1);
#endif
#if defined(_useTimer2)
  if (timer == _timer2)
    _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, ID_TC_FOR_TIMER2, IRQn_FOR_TIMER2, GCM_FOR_TIMER_2, INTENSET_BIT_FOR_TIMER_2);
#endif
}

static void finISR(timer16_Sequence_t timer)
{
#if defined(_useTimer1)

  TC_FOR_TIMER1->COUNT16.INTENCLR.reg = INTENCLR_BIT_FOR_TIMER_1;
#endif
#if defined(_useTimer2)

  TC_FOR_TIMER2->COUNT16.INTENCLR.reg = INTENCLR_BIT_FOR_TIMER_2;
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
