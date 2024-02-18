// Incluye la librería Cod-ES
#define INCLUIR_MOTOR_PAP

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
 *                  Ejemplo: Motor PAP
 *
 *    - Motor PAP conectado a los pines 8, 9, 10 y 11 -
 *
 */

constante entero PASOS = 100;

MotorPAP motor(PASOS, 8, 9, 10, 11); // Crear un objeto con la clase MotorPAP.

entero previo = 0; // Variable para almacenar la ultima posición del motor PAP.

inicio
  motor.ajustarVelocidad(30); // Configurar la velocidad del motor PAP.
fin

bucle
  entero valor = leerPin(0, ANALOGICO); // Variable para almacenar el valor enviado desde el motor PAP.

  motor.paso(valor - previo); // Hacer un paso del motor.
  previo = valor; // Almacenar el valor para tener el valor previo en el proximo ciclo.
fin