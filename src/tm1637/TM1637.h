#ifndef __TM1637__
#define __TM1637__

#include <inttypes.h>

#define SEG_A 0b00000001
#define SEG_B 0b00000010
#define SEG_C 0b00000100
#define SEG_D 0b00001000
#define SEG_E 0b00010000
#define SEG_F 0b00100000
#define SEG_G 0b01000000

#define DEFAULT_BIT_DELAY 100

class TM1637
{

public:
  TM1637(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY);

  void ajustarBrillo(uint8_t brightness, bool on = true);

  void ajustarSegmentos(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

  void limpiar();

  void mostrarDecimales(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  void mostrarDecimalesConPuntos(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  void mostrarHexadecimalesConPuntos(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  uint8_t codificarDigito(uint8_t digit);

protected:
  void esperarBits();

  void iniciar();

  void parar();

  bool escribirByte(uint8_t b);

  void mostrarPuntos(uint8_t dots, uint8_t *digits);

  void mostrarNumeroBaseConPuntos(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

private:
  uint8_t m_pinClk;
  uint8_t m_pinDIO;
  uint8_t m_brightness;
  unsigned int m_bitDelay;
};

#endif
