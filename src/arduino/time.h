// Define traducciones en español para unidades de tiempo.

#define MILISEGUNDOS 0  // Unidad de tiempo en milisegundos.
#define SEGUNDOS 1      // Unidad de tiempo en segundos.
#define MICROSEGUNDOS 2 // Unidad de tiempo en microsegundos.

void esperar(float s, int modo = MILISEGUNDOS)
{
    switch (modo)
    {
    case MILISEGUNDOS:
        delay(s); // Espera el tiempo especificado en milisegundos (equivalente a delay)
        break;
    case SEGUNDOS:
        delay(s * 1000); // Espera el tiempo especificado en segundos (convertido a milisegundos) (equivalente a delay)
        break;
    case MICROSEGUNDOS:
        delayMicroseconds(s); // Espera el tiempo especificado en microsegundos (equivalente a delayMicroseconds)
        break;
    default:
        delay(s); // Si el modo no es reconocido, espera el tiempo en milisegundos por defecto (equivalente a delay)
        break;
    }
}