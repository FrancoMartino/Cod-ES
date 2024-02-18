#ifndef _SERVO_H_
#define _SERVO_H_

#include "types.h"
#include "timer.h"

#include "wiring.h"

#define NOT_ATTACHED (-1)

#define _Nbr_16timers 14
#define SERVOS_PER_TIMER 4

#define MIN_ANGLE 0
#define MAX_ANGLE 180

#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400

class Servo
{
public:
    Servo();

    bool adjuntar(uint8 pin,
                  uint16 minPulseWidth = MIN_PULSE_WIDTH,
                  uint16 maxPulseWidth = MAX_PULSE_WIDTH,
                  int16 minAngle = MIN_ANGLE,
                  int16 maxAngle = MAX_ANGLE);
    bool desvincular();

    void escribir(int angle);

    void escribirMicrosegundos(uint16 pulseWidth);

    int leer() const;

    uint16 leerMicrosegundos() const;

    bool adjunto() const { return this->pin != NOT_ATTACHED; }

    int attachedPin() const { return this->pin; }

private:
    int16 pin;
    uint16 minPW;
    uint16 maxPW;
    int16 minAngle;
    int16 maxAngle;

    void resetFields(void);
};

#endif
