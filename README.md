# Cod-ES: Librería Arduino en Español

**Cod-ES** es una librería que traduce la sintaxis de Arduino al español, permitiendo a los usuarios programar en su idioma nativo. Esta librería es ideal para principiantes, educadores o cualquier persona que prefiera trabajar con una sintaxis en español.

## Características principales

*   **Sintaxis en español**: Todas las funciones, constantes y estructuras de control están traducidas al español.
    
*   **Compatibilidad**: Funciona con la mayoría de las placas Arduino.
    
*   **Fácil de usar**: Simplemente incluye la librería y comienza a programar en español.
    
*   **Ejemplos incluidos**: La librería viene con ejemplos prácticos para facilitar el aprendizaje.
    

## Instalación

1.  Descarga este repositorio como un archivo ZIP.
    
2.  En el IDE de Arduino, ve a `Sketch` > `Include Library` > `Add .ZIP Library...`.
    
3.  Selecciona el archivo ZIP descargado y haz clic en "Abrir".
    
4.  La librería estará disponible para ser incluida en tus proyectos.
    

## Uso

Para utilizar la librería, simplemente incluye el archivo principal en tu sketch:

cpp

Copy

#include <Cod-ES.h>

A partir de ahí, podrás utilizar las funciones y constantes traducidas al español en tu código.

### Ejemplo básico: Parpadeo de un LED

cpp

Copy

#include <Cod-ES.h>

inicio
  configurarSalida(LED\_INTERNO); // Configurar el LED interno como salida.
fin

bucle
  salidaDigital(LED\_INTERNO, ALTO); // Encender el LED interno.
  esperar(1, SEGUNDOS); // Esperar 1 segundo.
  salidaDigital(LED\_INTERNO, BAJO); // Apagar el LED interno.
  esperar(1, SEGUNDOS); // Esperar 1 segundo.
fin

## Ejemplos incluidos

### 1\. **Barrido de un servomotor**

Este ejemplo muestra cómo controlar un servomotor para que realice un barrido de 0 a 180 grados.

cpp

Copy

#include <Cod-ES.h>

Servo servo; // Crear un objeto con la clase Servo.
entero angulo \= 0; // Variable para almacenar la posición del servomotor.

inicio
  servo.adjuntar(9); // Configurar el pin 9 como salida para el servomotor.
fin

bucle
  repetir(angulo, angulo <= 180, angulo++) // Repetir de 0 a 180 grados.
    servo.escribir(angulo); // Posicionar el servomotor en el ángulo elegido.
    esperar(15, MILISEGUNDOS); // Pausa de 15 milisegundos.
  cerrar
  repetir(angulo, angulo \>= 0, angulo\--) // Repetir de 180 a 0 grados.
    servo.escribir(angulo); // Posicionar el servomotor en el ángulo elegido.
    esperar(15, MILISEGUNDOS); // Pausa de 15 milisegundos.
  cerrar
fin

### 2\. **Barrido de intensidad de un LED**

Este ejemplo muestra cómo variar la intensidad de un LED utilizando PWM.

cpp

Copy

#include <Cod-ES.h>

entero valor \= 0; // Variable para almacenar el valor de la intensidad del LED.

inicio
  // Configuración inicial
fin

bucle
  repetir(valor, valor <= 255, valor += 5) // Repetir de 0 a 255.
    salidaAnalogica(9, valor); // Ajustar intensidad del LED del pin 9.
    esperar(30, MILISEGUNDOS); // Pausa de 30 milisegundos.
  cerrar
  repetir(valor, valor \>= 0, valor \-= 5) // Repetir de 255 a 0.
    salidaAnalogica(9, valor); // Ajustar intensidad del LED del pin 9.
    esperar(30, MILISEGUNDOS); // Pausa de 30 milisegundos.
  cerrar
fin

### 3\. **Entrada polarizada con resistencia pull-up**

Este ejemplo muestra cómo leer un botón con resistencia pull-up y controlar un LED.

cpp

Copy

#include <Cod-ES.h>

inicio
  Serie.iniciar(9600); // Iniciar la comunicación serial a 9600 baudios.
  configurarEntrada(2, VERDADERO); // Configurar el pin 2 como entrada con pull-up.
  configurarSalida(LED\_INTERNO); // Configurar el LED interno como salida.
fin

bucle
  entero estadoBoton \= leerPin(2, DIGITAL); // Leer el estado del botón.
  Serie.imprimirEnNuevaLinea(estadoBoton); // Imprimir el estado del botón.

  si estadoBoton es\_igual ALTO entonces
    apagar(LED\_INTERNO); // Apagar el LED interno.
  si\_no
    encender(LED\_INTERNO); // Encender el LED interno.
  cerrar
fin

### 4\. **Detección de golpe con un sensor**

Este ejemplo muestra cómo detectar un golpe utilizando un sensor analógico.

cpp

Copy

#include <Cod-ES.h>

#define pinSensor A0
#define sensibilidad 100

entero lecturaSensor \= 0;
entero estadoLED \= BAJO;

inicio
  configurarSalida(LED\_INTERNO); // Configurar el LED interno como salida.
  Serie.iniciar(9600); // Iniciar la comunicación serial a 9600 baudios.
fin

bucle
  lecturaSensor \= leerPin(pinSensor, ANALOGICO); // Leer el valor del sensor.

  si lecturaSensor \>= sensibilidad entonces
    estadoLED \= no estadoLED; // Cambiar el estado del LED.
    salidaDigital(LED\_INTERNO, estadoLED); // Controlar el LED.
    Serie.imprimirEnNuevaLinea("¡Golpe!"); // Enviar un mensaje por serial.
  cerrar

  esperar(100, MILISEGUNDOS); // Esperar 100 milisegundos.
fin

### 5\. **Control de un motor paso a paso**

Este ejemplo muestra cómo controlar un motor paso a paso.

cpp

Copy

#include <Cod-ES.h>

constante entero PASOS \= 100;
MotorPAP motor(PASOS, 8, 9, 10, 11); // Crear un objeto con la clase MotorPAP.
entero previo \= 0; // Variable para almacenar la última posición del motor.

inicio
  motor.ajustarVelocidad(30); // Configurar la velocidad del motor.
fin

bucle
  entero valor \= leerPin(0, ANALOGICO); // Leer el valor analógico.
  motor.paso(valor \- previo); // Mover el motor.
  previo \= valor; // Actualizar la posición previa.
fin

## Estructura de la librería

La librería está organizada en varios archivos de encabezado (`*.h`), cada uno dedicado a una categoría específica de funciones:

*   **bits.h**: Funciones para manipulación de bits.
    
*   **characters.h**: Funciones para verificación de caracteres.
    
*   **constants.h**: Constantes comunes y modos de pin.
    
*   **control.h**: Estructuras de control de flujo.
    
*   **data.h**: Tipos de datos y modificadores.
    
*   **interrupts.h**: Funciones relacionadas con interrupciones.
    
*   **math.h**: Funciones matemáticas.
    
*   **pins.h**: Funciones para manipulación de pines.
    
*   **serial.h**: Funciones para comunicación serie.
    
*   **time.h**: Funciones relacionadas con el tiempo.
    

## Contribuciones

Si deseas contribuir a esta librería, ¡eres bienvenido! Puedes hacerlo mediante pull requests o reportando issues en el repositorio.

## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.
