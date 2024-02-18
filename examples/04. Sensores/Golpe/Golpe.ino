// Incluye la librería Cod-ES
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
 *                   Ejemplo: Golpe
 *
 *       - Sensor de golpe conectado al pin A0 -
 *
 */

#define pinSensor A0
#define sensibilidad 100

entero lecturaSensor = 0;
entero estadoLED = LOW;

inicio
  configurarSalida(LED_INTERNO); // Configurar el pin del LED interno como salida.
  Serie.iniciar(9600); // Iniciar la comunicación serial a 9600 baudios.
fin

bucle
  lecturaSensor = leerPin(pinSensor, ANALOGICO); // Leer el valor analógico del sensor conectado al pin A0.

  si lecturaSensor >= sensibilidad entonces // Verificar si el valor del sensor supera la sensibilidad establecida.
    estadoLED = no estadoLED; // Cambiar el estado del LED interno.
    salidaDigital(LED_INTERNO, estadoLED); // Controlar el LED interno según el nuevo estado.
    Serie.imprimirEnNuevaLinea("¡Golpe!"); // Enviar un mensaje por la comunicación serial.
  cerrar

  esperar(100, MILISEGUNDOS); // Esperar 100 milisegundos para evitar lecturas rápidas consecutivas.
fin
