#if defined(ARDUINO_ARCH_STM32F4)

#include "ServoTimers.h"

#include "boards.h"
#include "io.h"
#include "pwm.h"
#include "math.h"

#define MAX_OVERFLOW ((1 << 16) - 1)
#define CYC_MSEC (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC 20
#define TAU_USEC (TAU_MSEC * 1000)
#define TAU_CYC (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW ((uint16)round((double)TAU_CYC / SERVO_PRESCALER))

#define US_TO_COMPARE(us) ((uint16)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c) ((uint32)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a) ((uint16)(map((a), this->minAngle, this->maxAngle, \
                                     this->minPW, this->maxPW)))
#define US_TO_ANGLE(us) ((int16)(map((us), this->minPW, this->maxPW, \
                                     this->minAngle, this->maxAngle)))

Servo::Servo()
{
    this->resetFields();
}

bool Servo::adjuntar(uint8 pin, uint16 minPW, uint16 maxPW, int16 minAngle, int16 maxAngle)
{

    timer_dev *tdev = PIN_MAP[pin].timer_device;

    analogWriteResolution(16);

    int prescaler = 6;
    int overflow = 65400;
    int minPW_correction = 300;
    int maxPW_correction = 300;

    pinMode(pin, OUTPUT);

    if (tdev == NULL)
    {

        return false;
    }

    if ((tdev == TIMER1) || (tdev == TIMER8) || (tdev == TIMER10) || (tdev == TIMER11))
    {
        prescaler = 54;
        overflow = 65400;
        minPW_correction = 40;
        maxPW_correction = 50;
    }

    if ((tdev == TIMER2) || (tdev == TIMER3) || (tdev == TIMER4) || (tdev == TIMER5))
    {
        prescaler = 6;
        overflow = 64285;
        minPW_correction = 370;
        maxPW_correction = 350;
    }

    if ((tdev == TIMER6) || (tdev == TIMER7))
    {
        prescaler = 6;
        overflow = 65400;
        minPW_correction = 0;
        maxPW_correction = 0;
    }

    if ((tdev == TIMER9) || (tdev == TIMER12) || (tdev == TIMER13) || (tdev == TIMER14))
    {
        prescaler = 6;
        overflow = 65400;
        minPW_correction = 30;
        maxPW_correction = 0;
    }

    if (this->adjunto())
    {
        this->desvincular();
    }

    this->pin = pin;
    this->minPW = (minPW + minPW_correction);
    this->maxPW = (maxPW + maxPW_correction);
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;

    timer_pause(tdev);
    timer_set_prescaler(tdev, prescaler);
    timer_set_reload(tdev, overflow);
    timer_generate_update(tdev);
    timer_resume(tdev);

    return true;
}

bool Servo::desvincular()
{
    if (!this->adjunto())
    {
        return false;
    }

    timer_dev *tdev = PIN_MAP[this->pin].timer_device;
    uint8 tchan = PIN_MAP[this->pin].timer_channel;
    timer_set_mode(tdev, tchan, TIMER_DISABLED);

    this->resetFields();

    return true;
}

void Servo::escribir(int degrees)
{
    degrees = constrain(degrees, this->minAngle, this->maxAngle);
    this->escribirMicrosegundos(ANGLE_TO_US(degrees));
}

int Servo::leer() const
{
    int a = US_TO_ANGLE(this->leerMicrosegundos());

    return a == this->minAngle || a == this->maxAngle ? a : a + 1;
}

void Servo::escribirMicrosegundos(uint16 pulseWidth)
{
    if (!this->adjunto())
    {
        ASSERT(0);
        return;
    }
    pulseWidth = constrain(pulseWidth, this->minPW, this->maxPW);
    analogWrite(this->pin, US_TO_COMPARE(pulseWidth));
}

uint16 Servo::leerMicrosegundos() const
{
    if (!this->adjunto())
    {
        ASSERT(0);
        return 0;
    }

    stm32_pin_info pin_info = PIN_MAP[this->pin];
    uint16 compare = timer_get_compare(pin_info.timer_device,
                                       pin_info.timer_channel);

    return COMPARE_TO_US(compare);
}

void Servo::resetFields(void)
{
    this->pin = NOT_ATTACHED;
    this->minAngle = MIN_ANGLE;
    this->maxAngle = MAX_ANGLE;
    this->minPW = MIN_PULSE_WIDTH;
    this->maxPW = MAX_PULSE_WIDTH;
}

#endif
