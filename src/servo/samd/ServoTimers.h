#ifndef __SERVO_TIMERS_H__
#define __SERVO_TIMERS_H__

#define _useTimer1

#if defined(_useTimer1)
#define TC_FOR_TIMER1 TC4
#define CHANNEL_FOR_TIMER1 0
#define INTENSET_BIT_FOR_TIMER_1 TC_INTENSET_MC0
#define INTENCLR_BIT_FOR_TIMER_1 TC_INTENCLR_MC0
#define INTFLAG_BIT_FOR_TIMER_1 TC_INTFLAG_MC0
#define ID_TC_FOR_TIMER1 ID_TC4
#define IRQn_FOR_TIMER1 TC4_IRQn
#define HANDLER_FOR_TIMER1 TC4_Handler
#define GCM_FOR_TIMER_1 GCM_TC4_TC5
#endif
#if defined(_useTimer2)
#define TC_FOR_TIMER2 TC4
#define CHANNEL_FOR_TIMER2 1
#define INTENSET_BIT_FOR_TIMER_2 TC_INTENSET_MC1
#define INTENCLR_BIT_FOR_TIMER_2 TC_INTENCLR_MC1
#define ID_TC_FOR_TIMER2 ID_TC4
#define IRQn_FOR_TIMER2 TC4_IRQn
#define HANDLER_FOR_TIMER2 TC4_Handler
#define GCM_FOR_TIMER_2 GCM_TC4_TC5
#endif

typedef enum
{
#if defined(_useTimer1)
    _timer1,
#endif
#if defined(_useTimer2)
    _timer2,
#endif
    _Nbr_16timers
} timer16_Sequence_t;

#endif
