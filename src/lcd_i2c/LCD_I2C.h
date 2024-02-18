#ifndef LCD_I2C_h
#define LCD_I2C_h

#include <inttypes.h>
#include "Print.h" 
#include <Wire.h>

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  
#define Rw B00000010  
#define Rs B00000001  

class LCD_I2C : public Print {
public:
  LCD_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
  void iniciar(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
  void limpiar();
  void regresarAlInicio();
  void apagarDisplay();
  void encenderDisplay();
  void apagarParpadeo();
  void encenderParpadeo();
  void apagarCursor();
  void encenderCursor();
  void deslizarDisplayIzquierda();
  void deslizarDisplayDerecha();
  void imprimirIzquierda();
  void imprimirDerecha();
  void izquierdaADerecha();
  void derechaAIzquierda();
  void shiftIncrement();
  void shiftDecrement();
  void apagarLuz();
  void encenderLuz();
  void deslizarAutomaticamente();
  void noDeslizarAutomaticamente(); 
  void crearCaracter(uint8_t, uint8_t[]);
  void ajustarCursor(uint8_t, uint8_t); 

#if defined(ARDUINO) && ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void write(uint8_t);
#endif
  void comando(uint8_t);
  void inicializar();

  void blink_on();            
  void blink_off();                 
  void cursor_on();                 
  void cursor_off();                
  void ajustarLuz(uint8_t new_val);       
  void cargarCaracterPersonalizado(uint8_t char_num, uint8_t *rows);  
  void imprimirCadena(const char[]);

  uint8_t status();
  void setContrast(uint8_t new_val);
  uint8_t keypad();
  void setDelay(int,int);
  void on();
  void off();
  uint8_t init_bargraph(uint8_t graphtype);
  void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
  void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);

private:
  void init_priv();
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);
  uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
};

#endif