#ifndef __SERVO_TIMERS_H__
#define __SERVO_TIMERS_H__

#include <avr/io.h>

#define USE_TIMERB2

#if !defined(USE_TIMERB1) && !defined(USE_TIMERB2) && !defined(USE_TIMERB0)
#error "No timers allowed for Servo"

#endif

static volatile TCB_t *_timer =
#if defined(USE_TIMERB0)
    &TCB0;
#endif
#if defined(USE_TIMERB1)
&TCB1;
#endif
#if defined(USE_TIMERB2)
&TCB2;
#endif

typedef enum
{
  timer0,
  _Nbr_16timers
} timer16_Sequence_t;

#endif
