#if defined(ARDUINO_ARCH_NRF52)

#include <Arduino.h>
#include "../Servo.h"

static servo_t servos[MAX_SERVOS];

uint8_t ServoCount = 0;

uint32_t group_pins[3][NRF_PWM_CHANNEL_COUNT] = {{NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED}, {NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED}, {NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED, NRF_PWM_PIN_NOT_CONNECTED}};
static uint16_t seq_values[3][NRF_PWM_CHANNEL_COUNT] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

Servo::Servo()
{
	if (ServoCount < MAX_SERVOS)
	{
		this->servoIndex = ServoCount++;
	}
	else
	{
		this->servoIndex = INVALID_SERVO;
	}
}

uint8_t Servo::adjuntar(int pin)
{

	return this->adjuntar(pin, 0, 2500);
}

uint8_t Servo::adjuntar(int pin, int min, int max)
{
	int servo_min, servo_max;
	if (this->servoIndex < MAX_SERVOS)
	{
		pinMode(pin, OUTPUT);
		servos[this->servoIndex].Pin.nbr = pin;

		if (min < servo_min)
			min = servo_min;
		if (max > servo_max)
			max = servo_max;
		this->min = min;
		this->max = max;

		servos[this->servoIndex].Pin.isActive = true;
	}
	return this->servoIndex;
}

void Servo::desvincular()
{
	servos[this->servoIndex].Pin.isActive = false;
}

void Servo::escribir(int value)
{
	if (value < 0)
		value = 0;
	else if (value > 180)
		value = 180;
	value = map(value, 0, 180, MIN_PULSE, MAX_PULSE);

	escribirMicrosegundos(value);
}

void Servo::escribirMicrosegundos(int value)
{
	uint8_t channel, instance;
	uint8_t pin = servos[this->servoIndex].Pin.nbr;

	instance = (g_APinDescription[pin].ulPWMChannel & 0xF0) / 16;

	channel = g_APinDescription[pin].ulPWMChannel & 0x0F;
	group_pins[instance][channel] = g_APinDescription[pin].ulPin;
	NRF_PWM_Type *PWMInstance = instance == 0 ? NRF_PWM0 : (instance == 1 ? NRF_PWM1 : NRF_PWM2);

	seq_values[instance][channel] = value | 0x8000;
	nrf_pwm_sequence_t const seq = {
		seq_values[instance],
		NRF_PWM_VALUES_LENGTH(seq_values),
		0,
		0};
	nrf_pwm_pins_set(PWMInstance, group_pins[instance]);
	nrf_pwm_enable(PWMInstance);
	nrf_pwm_configure(PWMInstance, NRF_PWM_CLK_125kHz, NRF_PWM_MODE_UP, 2500);
	nrf_pwm_decoder_set(PWMInstance, NRF_PWM_LOAD_INDIVIDUAL, NRF_PWM_STEP_AUTO);
	nrf_pwm_sequence_set(PWMInstance, 0, &seq);
	nrf_pwm_loop_set(PWMInstance, 0UL);
	nrf_pwm_task_trigger(PWMInstance, NRF_PWM_TASK_SEQSTART0);
}

int Servo::leer()
{
	return map(leerMicrosegundos(), MIN_PULSE, MAX_PULSE, 0, 180);
}

int Servo::leerMicrosegundos()
{
	uint8_t channel, instance;
	uint8_t pin = servos[this->servoIndex].Pin.nbr;
	instance = (g_APinDescription[pin].ulPWMChannel & 0xF0) / 16;
	channel = g_APinDescription[pin].ulPWMChannel & 0x0F;

	return seq_values[instance][channel] & 0x7FFF;
}

bool Servo::adjunto()
{
	return servos[this->servoIndex].Pin.isActive;
}

#endif
