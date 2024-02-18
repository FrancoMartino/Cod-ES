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
 *                   Ejemplo: Parpadeo
 */

inicio
  configurarSalida(LED_INTERNO); // Configurar un pin determinado como salida para controlar un dispositivo, en este caso, el LED interno.
fin

bucle
  salidaDigital(LED_INTERNO, ALTO); // Encender el LED interno.
  esperar(1, SEGUNDOS); // Pausa de 1 segundo en la ejecución del programa.
  salidaDigital(LED_INTERNO, BAJO); // Apagar el LED interno.
  esperar(1, SEGUNDOS); // Pausa de 1 segundo en la ejecución del programa.
fin