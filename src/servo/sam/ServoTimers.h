#define _useTimer1
#define _useTimer2
#define _useTimer3
#define _useTimer4
#define _useTimer5



#if defined (_useTimer1)
#define TC_FOR_TIMER1       TC1
#define CHANNEL_FOR_TIMER1  0
#define ID_TC_FOR_TIMER1    ID_TC3
#define IRQn_FOR_TIMER1     TC3_IRQn
#define HANDLER_FOR_TIMER1  TC3_Handler
#endif
#if defined (_useTimer2)
#define TC_FOR_TIMER2       TC1
#define CHANNEL_FOR_TIMER2  1
#define ID_TC_FOR_TIMER2    ID_TC4
#define IRQn_FOR_TIMER2     TC4_IRQn
#define HANDLER_FOR_TIMER2  TC4_Handler
#endif
#if defined (_useTimer3)
#define TC_FOR_TIMER3       TC1
#define CHANNEL_FOR_TIMER3  2
#define ID_TC_FOR_TIMER3    ID_TC5
#define IRQn_FOR_TIMER3     TC5_IRQn
#define HANDLER_FOR_TIMER3  TC5_Handler
#endif
#if defined (_useTimer4)
#define TC_FOR_TIMER4       TC0
#define CHANNEL_FOR_TIMER4  2
#define ID_TC_FOR_TIMER4    ID_TC2
#define IRQn_FOR_TIMER4     TC2_IRQn
#define HANDLER_FOR_TIMER4  TC2_Handler
#endif
#if defined (_useTimer5)
#define TC_FOR_TIMER5       TC0
#define CHANNEL_FOR_TIMER5  0
#define ID_TC_FOR_TIMER5    ID_TC0
#define IRQn_FOR_TIMER5     TC0_IRQn
#define HANDLER_FOR_TIMER5  TC0_Handler
#endif

typedef enum { _timer1, _timer2, _timer3, _timer4, _timer5, _Nbr_16timers } timer16_Sequence_t ;
