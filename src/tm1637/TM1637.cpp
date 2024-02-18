extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include "TM1637.h"
#include <Arduino.h>

#define TM1637_I2C_COMM1 0x40
#define TM1637_I2C_COMM2 0xC0
#define TM1637_I2C_COMM3 0x80

const uint8_t digitToSegment[] = {

  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
  0b01110111,
  0b01111100,
  0b00111001,
  0b01011110,
  0b01111001,
  0b01110001};

  static const uint8_t minusSegments = 0b01000000;

  TM1637::TM1637(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay)
  {

    m_pinClk = pinClk;
    m_pinDIO = pinDIO;
    m_bitDelay = bitDelay;

    pinMode(m_pinClk, INPUT);
    pinMode(m_pinDIO, INPUT);
    digitalWrite(m_pinClk, LOW);
    digitalWrite(m_pinDIO, LOW);
  }

  void TM1637::ajustarBrillo(uint8_t brightness, bool on)
  {
    m_brightness = (brightness & 0x7) | (on ? 0x08 : 0x00);
  }

  void TM1637::ajustarSegmentos(const uint8_t segments[], uint8_t length, uint8_t pos)
  {

    iniciar();
    escribirByte(TM1637_I2C_COMM1);
    parar();

    iniciar();
    escribirByte(TM1637_I2C_COMM2 + (pos & 0x03));

    for (uint8_t k = 0; k < length; k++)
      escribirByte(segments[k]);

    parar();

    iniciar();
    escribirByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
    parar();
  }

  void TM1637::limpiar()
  {
    uint8_t data[] = {0, 0, 0, 0};
    ajustarSegmentos(data);
  }

  void TM1637::mostrarDecimales(int num, bool leading_zero, uint8_t length, uint8_t pos)
  {
    mostrarDecimalesConPuntos(num, 0, leading_zero, length, pos);
  }

  void TM1637::mostrarDecimalesConPuntos(int num, uint8_t dots, bool leading_zero,
   uint8_t length, uint8_t pos)
  {
    mostrarNumeroBaseConPuntos(num < 0 ? -10 : 10, num < 0 ? -num : num, dots, leading_zero, length, pos);
  }

  void TM1637::mostrarHexadecimalesConPuntos(uint16_t num, uint8_t dots, bool leading_zero,
   uint8_t length, uint8_t pos)
  {
    mostrarNumeroBaseConPuntos(16, num, dots, leading_zero, length, pos);
  }

  void TM1637::mostrarNumeroBaseConPuntos(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
    uint8_t length, uint8_t pos)
  {
    bool negative = false;
    if (base < 0)
    {
      base = -base;
      negative = true;
    }

    uint8_t digits[4];

    if (num == 0 && !leading_zero)
    {

      for (uint8_t i = 0; i < (length - 1); i++)
        digits[i] = 0;
      digits[length - 1] = codificarDigito(0);
    }
    else
    {

      for (int i = length - 1; i >= 0; --i)
      {
        uint8_t digit = num % base;

        if (digit == 0 && num == 0 && leading_zero == false)

          digits[i] = 0;
        else
          digits[i] = codificarDigito(digit);

        if (digit == 0 && num == 0 && negative)
        {
          digits[i] = minusSegments;
          negative = false;
        }

        num /= base;
      }

      if (dots != 0)
      {
        mostrarPuntos(dots, digits);
      }
    }
    ajustarSegmentos(digits, length, pos);
  }

  void TM1637::esperarBits()
  {
    delayMicroseconds(m_bitDelay);
  }

  void TM1637::iniciar()
  {
    pinMode(m_pinDIO, OUTPUT);
    esperarBits();
  }

  void TM1637::parar()
  {
    pinMode(m_pinDIO, OUTPUT);
    esperarBits();
    pinMode(m_pinClk, INPUT);
    esperarBits();
    pinMode(m_pinDIO, INPUT);
    esperarBits();
  }

  bool TM1637::escribirByte(uint8_t b)
  {
    uint8_t data = b;

    for (uint8_t i = 0; i < 8; i++)
    {

      pinMode(m_pinClk, OUTPUT);
      esperarBits();

      if (data & 0x01)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);

      esperarBits();

      pinMode(m_pinClk, INPUT);
      esperarBits();
      data = data >> 1;
    }

    pinMode(m_pinClk, OUTPUT);
    pinMode(m_pinDIO, INPUT);
    esperarBits();

    pinMode(m_pinClk, INPUT);
    esperarBits();
    uint8_t ack = digitalRead(m_pinDIO);
    if (ack == 0)
      pinMode(m_pinDIO, OUTPUT);

    esperarBits();
    pinMode(m_pinClk, OUTPUT);
    esperarBits();

    return ack;
  }

  void TM1637::mostrarPuntos(uint8_t dots, uint8_t *digits)
  {
    for (int i = 0; i < 4; ++i)
    {
      digits[i] |= (dots & 0x80);
      dots <<= 1;
    }
  }

  uint8_t TM1637::codificarDigito(uint8_t digit)
  {
    return digitToSegment[digit & 0x0f];
  }
