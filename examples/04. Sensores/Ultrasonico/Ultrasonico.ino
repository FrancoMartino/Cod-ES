// Incluye la librería Cod-ES
#define INCLUIR_ULTRASONICO
#define INCLUIR_SERIE
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
 *                 Ejemplo: Ultrasónico
 *
 *      - Sensor ultrasónico - TRIG:9 y ECHO:10 -
 *
 */

// Define los pines para el sensor ultrasónico.
#define trigPin 9
#define echoPin 10

// Crea una instancia de la clase Ultrasonico.
Ultrasonico sensor(trigPin, echoPin);

inicio
  Serie.iniciar(9600); // Iniciar la comunicación serial a 9600 baudios.
fin

bucle
  flotante distancia = sensor.obtenerDistancia(); // Leer la distancia del objeto al sensor en centimetros.

  // Enviar un mensaje por la comunicación serial.
  Serie.imprimir("Distancia: ");
  Serie.imprimir(distancia);
  Serie.imprimirEnNuevaLinea(" cm");

  esperar(1, SEGUNDOS); // Esperar 1 segundo para repetir.
fin
