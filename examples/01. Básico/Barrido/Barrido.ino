// Incluye la librería Cod-ES
#define INCLUIR_SERVO

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
 *                   Ejemplo: Barrido
 *
 *         - Servomotor conectado al pin PWM 9 -
 *
 */

Servo servo; // Crear un objeto con la clase Servo.

entero angulo = 0; // Variable para almacenar la posicion del servomotor.

inicio
  servo.adjuntar(9); // Configurar el pin 9 como salida para el servomotor.
fin

bucle
  repetir(angulo, angulo <= 180, angulo++) // Repetir de 0 a 180 grados.
    servo.escribir(angulo); // Posicionar el servomotor en el ángulo elegido.
    esperar(15, MILISEGUNDOS); // Pausa de 15 milisegundos en la ejecución del programa.
  cerrar
  repetir(angulo, angulo >= 0, angulo--) // Repetir de 180 a 0 grados.
    servo.escribir(angulo); // Posicionar el servomotor en el ángulo elegido.
    esperar(15, MILISEGUNDOS); // Pausa de 15 milisegundos en la ejecución del programa.
  cerrar
fin
