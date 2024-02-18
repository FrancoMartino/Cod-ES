// Incluye la librería Cod-ES
#define INCLUIR_LCD_I2C

#include <Cod-ES.h>

/*      
 * _________            .___        ___________ _________
 * \_   ___ \  ____   __| _/        \_   _____//   _____/
 * /    \  \/ /  _ \ / __ |  ______  |    __)_ \_____  \ 
 * \     \___(  <_> ) /_/ | /_____/  |        \/        \
 *  \______  /\____/\____ |         /_______  /_______  /
 *         \/            \/                 \/        \/ 
 *
 *                2024 - Franco Martino
 *
 *
 *                    Ejemplo: LCD
 *
 *           - LCD I2C - SDA:A4 y SCL:A5 -
 *
 */

LCD_I2C lcd(0x27,16,2); // Crear un objeto con la clase LCD_I2C.

inicio
  lcd.inicializar(); // Iniciar la comunicación con el display LCD.
  lcd.encenderLuz(); // Encender luz del display LCD.
  lcd.ajustarCursor(1,0); // Posiciono el cursor en x:1, y:0.
  lcd.imprimirCadena("!Hola, mundo!"); // Imprimo en el display "!Hola, mundo!".
fin

bucle
fin