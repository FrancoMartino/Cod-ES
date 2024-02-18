#ifndef Cod-ES_h
#define Cod-ES_h

#include <Arduino.h>

// Incluye los archivos de la librería Cod-ES que contienen traducciones de funciones y constantes comunes de Arduino.
#include "arduino/bits.h"
#include "arduino/characters.h"
#include "arduino/constants.h"
#include "arduino/control.h"
#include "arduino/data.h"
#include "arduino/interrupts.h"
#include "arduino/math.h"
#include "arduino/pins.h"
#include "arduino/time.h"

#ifdef INCLUIR_SERIE
#include "arduino/serial.h"
#endif

#ifdef INCLUIR_SERVO
#include "servo/Servo.h"
#endif

#ifdef INCLUIR_TM1637
#include "tm1637/TM1637.h"
#endif

#ifdef INCLUIR_LED_RGB
#include "led_rgb/LedRGB.h"
#endif

#ifdef INCLUIR_MOTOR_PAP
#include "stepper/MotorPAP.h"
#endif

#ifdef INCLUIR_EEPROM
#include "eeprom/EEPROM.h"
#endif

#ifdef INCLUIR_LCD_I2C
#include <Wire.h> 
#include "lcd_i2c/LCD_I2C.h"
#endif

#ifdef INCLUIR_ULTRASONICO
#include "ultrasonic/Ultrasonico.h"
#endif

#endif
