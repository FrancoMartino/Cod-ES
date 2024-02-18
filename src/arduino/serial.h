// Define traducciones en español para configuraciones y funciones relacionadas con la comunicación serie.

#define SERIE_BINARIO SERIAL_BINARY  // Configuración de la comunicación serie en formato binario (equivalente a SERIAL_BINARY)
#define SERIE_OCTAL SERIAL_OCTAL     // Configuración de la comunicación serie en formato octal (equivalente a SERIAL_OCTAL)
#define SERIE_DECIMAL SERIAL_DECIMAL // Configuración de la comunicación serie en formato decimal (equivalente a SERIAL_DECIMAL)
#define SERIE_HEXADECIMAL SERIAL_HEX // Configuración de la comunicación serie en formato hexadecimal (equivalente a SERIAL_HEX)

class SerieClass
{
public:
    void iniciar(unsigned long baudRate)
    {
        Serial.begin(baudRate); // Inicia la comunicación serie con la velocidad especificada (equivalente a Serial.begin)
    }

    void terminar()
    {
        Serial.end(); // Finaliza la comunicación serie (equivalente a Serial.end)
    }

    int disponible()
    {
        return Serial.available(); // Retorna la cantidad de bytes disponibles para leer (equivalente a Serial.available)
    }

    int disponibleParaEscritura()
    {
        return Serial.availableForWrite(); // Retorna la cantidad de bytes disponibles para escribir (equivalente a Serial.availableForWrite)
    }

    void vaciar()
    {
        Serial.flush(); // Vacía el búfer de salida de la comunicación serie (equivalente a Serial.flush)
    }

    float leerFlotante()
    {
        return Serial.parseFloat(); // Lee un valor flotante desde la comunicación serie (equivalente a Serial.parseFloat)
    }

    int leerEntero()
    {
        return Serial.parseInt(); // Lee un valor entero desde la comunicación serie (equivalente a Serial.parseInt)
    }

    int espiar()
    {
        return Serial.peek(); // Espía el siguiente byte disponible sin consumirlo (equivalente a Serial.peek)
    }

    template <class T>
    void imprimir(T s)
    {
        Serial.print(s); // Imprime una cadena en la comunicación serie (equivalente a Serial.print)
    }

    template <class T>
    void imprimirEnNuevaLinea(T s)
    {
        Serial.println(s); // Imprime una cadena seguida de un salto de línea en la comunicación serie (equivalente a Serial.println)
    }

    template <class T>
    void escribir(T val)
    {
        Serial.write(val); // Escribe un byte en la comunicación serie (equivalente a Serial.write)
    }

    int leer()
    {
        return Serial.read(); // Lee un byte desde la comunicación serie (equivalente a Serial.read)
    }

    size_t leerBytes(char *buffer, size_t length)
    {
        return Serial.readBytes(buffer, length); // Lee una cantidad específica de bytes en un buffer desde la comunicación serie (equivalente a Serial.readBytes)
    }

    size_t leerBytesHastaEncontrar(char terminator, char *buffer, size_t length)
    {
        return Serial.readBytesUntil(terminator, buffer, length); // Lee bytes hasta encontrar un carácter terminador en la comunicación serie (equivalente a Serial.readBytesUntil)
    }

    String leerCadena()
    {
        return Serial.readString(); // Lee una cadena desde la comunicación serie (equivalente a Serial.readString)
    }

    String leerCadenaHastaEncontrar(char terminator)
    {
        return Serial.readStringUntil(terminator); // Lee una cadena hasta encontrar un carácter terminador en la comunicación serie (equivalente a Serial.readStringUntil)
    }

    void establecerTiempoDeEspera(unsigned long timeout)
    {
        Serial.setTimeout(timeout); // Establece el tiempo de espera para operaciones de lectura desde la comunicación serie (equivalente a Serial.setTimeout)
    }
};

SerieClass Serie; // Instancia de la clase SerieClass para manipulación simplificada de la comunicación serie.