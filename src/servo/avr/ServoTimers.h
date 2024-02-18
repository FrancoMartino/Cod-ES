#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
#define _useTimer1
#define _useTimer3
#define _useTimer4
typedef enum
{
  _timer5,
  _timer1,
  _timer3,
  _timer4,
  _Nbr_16timers
} timer16_Sequence_t;

#elif defined(__AVR_ATmega32U4__)
#define _useTimer1
typedef enum
{
  _timer1,
  _Nbr_16timers
} timer16_Sequence_t;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
#define _useTimer1
typedef enum
{
  _timer3,
  _timer1,
  _Nbr_16timers
} timer16_Sequence_t;

#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2561__)
#define _useTimer3
#define _useTimer1
typedef enum
{
  _timer3,
  _timer1,
  _Nbr_16timers
} timer16_Sequence_t;

#else
#define _useTimer1
typedef enum
{
  _timer1,
  _Nbr_16timers
} timer16_Sequence_t;
#endif
