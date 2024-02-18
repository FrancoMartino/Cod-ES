#include "LCD_I2C.h"
#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100

#include <Arduino.h>

#define printIIC(args)	Wire.write(args)
inline size_t LCD_I2C::write(uint8_t value) {
	send(value, Rs);
	return 1;
}

#else
#include "WProgram.h"

#define printIIC(args)	Wire.send(args)
inline void LCD_I2C::write(uint8_t value) {
	send(value, Rs);
}

#endif
#include "Wire.h"

LCD_I2C::LCD_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
{
	_Addr = lcd_Addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_backlightval = LCD_NOBACKLIGHT;
}

void LCD_I2C::inicializar(){
	init_priv();
}

void LCD_I2C::init_priv()
{
	Wire.begin();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	iniciar(_cols, _rows);  
}

void LCD_I2C::iniciar(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;

	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	delay(50); 

	expanderWrite(_backlightval);	
	delay(1000);

	write4bits(0x03 << 4);
	delayMicroseconds(4500); 

	write4bits(0x03 << 4);
	delayMicroseconds(4500); 

	write4bits(0x03 << 4); 
	delayMicroseconds(150);

	write4bits(0x02 << 4); 

	comando(LCD_FUNCTIONSET | _displayfunction);  

	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	encenderDisplay();

	limpiar();

	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	comando(LCD_ENTRYMODESET | _displaymode);

	regresarAlInicio();

}

void LCD_I2C::limpiar(){
	comando(LCD_CLEARDISPLAY);
	delayMicroseconds(2000);  
}

void LCD_I2C::regresarAlInicio(){
	comando(LCD_RETURNHOME);  
	delayMicroseconds(2000);  
}

void LCD_I2C::ajustarCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    
	}
	comando(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_I2C::apagarDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_I2C::encenderDisplay() {
	_displaycontrol |= LCD_DISPLAYON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_I2C::apagarCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_I2C::encenderCursor() {
	_displaycontrol |= LCD_CURSORON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_I2C::apagarParpadeo() {
	_displaycontrol &= ~LCD_BLINKON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD_I2C::encenderParpadeo() {
	_displaycontrol |= LCD_BLINKON;
	comando(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCD_I2C::deslizarDisplayIzquierda(void) {
	comando(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_I2C::deslizarDisplayDerecha(void) {
	comando(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_I2C::izquierdaADerecha(void) {
	_displaymode |= LCD_ENTRYLEFT;
	comando(LCD_ENTRYMODESET | _displaymode);
}

void LCD_I2C::derechaAIzquierda(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	comando(LCD_ENTRYMODESET | _displaymode);
}

void LCD_I2C::deslizarAutomaticamente(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	comando(LCD_ENTRYMODESET | _displaymode);
}

void LCD_I2C::noDeslizarAutomaticamente(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	comando(LCD_ENTRYMODESET | _displaymode);
}

void LCD_I2C::crearCaracter(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; 
	comando(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}

void LCD_I2C::apagarLuz(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LCD_I2C::encenderLuz(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}

inline void LCD_I2C::comando(uint8_t value) {
	send(value, 0);
}

void LCD_I2C::send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode); 
}

void LCD_I2C::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void LCD_I2C::expanderWrite(uint8_t _data){                                        
	Wire.beginTransmission(_Addr);
	printIIC((int)(_data) | _backlightval);
	Wire.endTransmission();   
}

void LCD_I2C::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	
	delayMicroseconds(1);		

	expanderWrite(_data & ~En);	
	delayMicroseconds(50);		
} 

void LCD_I2C::cursor_on(){
	encenderCursor();
}

void LCD_I2C::cursor_off(){
	apagarCursor();
}

void LCD_I2C::blink_on(){
	encenderParpadeo();
}

void LCD_I2C::blink_off(){
	apagarParpadeo();
}

void LCD_I2C::cargarCaracterPersonalizado(uint8_t char_num, uint8_t *rows){
	crearCaracter(char_num, rows);
}

void LCD_I2C::ajustarLuz(uint8_t new_val){
	if(new_val){
		encenderLuz();		
	}else{
		apagarLuz();		
	}
}

void LCD_I2C::imprimirCadena(const char c[]){

	print(c);
}

void LCD_I2C::off(){}
void LCD_I2C::on(){}
void LCD_I2C::setDelay (int cmdDelay,int charDelay) {}
uint8_t LCD_I2C::status(){return 0;}
uint8_t LCD_I2C::keypad (){return 0;}
uint8_t LCD_I2C::init_bargraph(uint8_t graphtype){return 0;}
void LCD_I2C::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end){}
void LCD_I2C::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end){}
void LCD_I2C::setContrast(uint8_t new_val){}