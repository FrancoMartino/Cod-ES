// Incluye la librería Cod-ES
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
 *                 Ejemplo: Barrido LED
 *
 * - LED conectado al pin 9 con resistencia de 220 ohms -
 *
 */

entero valor = 0; // Variable para almacenar el valor de la intensidad del LED.

inicio
  // Pon tu código de configuración aquí, para que se ejecute una vez
  
fin

bucle
  repetir(valor, valor <= 255, valor += 5) // Repetir de 0 a 255.
    salidaAnalogica(9, valor); // Ajustar intensidad del LED del pin 9.
    esperar(30, MILISEGUNDOS); // Pausa de 30 milisegundos en la ejecución del programa.
  cerrar
  repetir(valor, valor >= 0, valor -= 5) // Repetir de 255 a 0.
    salidaAnalogica(9, valor); // Ajustar intensidad del LED del pin 9.
    esperar(30, MILISEGUNDOS); // Pausa de 30 milisegundos en la ejecución del programa.
  cerrar
fin