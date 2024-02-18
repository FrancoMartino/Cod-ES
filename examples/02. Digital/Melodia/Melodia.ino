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
 *                  Ejemplo: Melodia
 *
 *         - Buzzer pasivo conectado al pin 8 -
 *
 */

#include "notas.h" // Incluir el archivo de encabezado que contiene las definiciones de las notas musicales.

entero melodia[] = {
  NOTA_DO4, NOTA_SOL3, NOTA_SOL3, NOTA_LA3, NOTA_SOL3, 0, NOTA_SI3, NOTA_C4
};

entero duraciones[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

inicio
  repetir (entero nota = 0; nota < 8; nota++) {
    entero duracion = 1000 / duraciones[nota]; // Calcular la duración de la nota en milisegundos.
    
    tono(8, melodia[nota], duracion); // Generar un tono con la nota actual y su duración.
    esperar(duracion * 1.30, MILISEGUNDOS); // Esperar un tiempo ligeramente mayor que la duración de la nota.
    apagarTono(8); // Apagar el tono después de la duración especificada.
  }
fin

bucle
  // Coloca tu código principal aquí para que se ejecute repetidamente.
  
fin
