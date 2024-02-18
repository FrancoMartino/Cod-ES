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
 *             Ejemplo: Entrada Polarizada
 *
 *  - Interruptor sin retención conectado entre GND y D2 -
 *
 */

inicio
  Serie.iniciar(9600); // Iniciar la comunicación serial a 9600 baudios.
  configurarEntrada(2, VERDADERO); // Configurar el pin 2 como entrada digital con resistencia pull-up.
  configurarSalida(LED_INTERNO); // Configurar el LED interno como salida digital.
fin

bucle
  entero estadoBoton = leerPin(2, DIGITAL); // Leer el estado del botón conectado al pin 2.
  Serie.imprimirEnNuevaLinea(estadoBoton); // Imprimir el estado del botón en una nueva línea.

  si estadoBoton es_igual ALTO entonces
    apagar(LED_INTERNO); // Apagar el LED interno.
  si_no
    encender(LED_INTERNO); // Encender el LED interno.
  cerrar
fin
