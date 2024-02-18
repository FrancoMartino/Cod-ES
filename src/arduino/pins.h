// Define traducciones de funciones para manipulación de pines en español.

#define salidaDigital digitalWrite  // Función para establecer la salida digital de un pin (equivalente a digitalWrite)
#define salidaAnalogica analogWrite // Función para establecer la salida analógica de un pin (equivalente a analogWrite)

#define referenciaAnalogica analogReference                // Función para establecer la referencia para lecturas analógicas (equivalente a analogReference)
#define resolucionLecturaAnalogica analogReadResolution    // Función para establecer la resolución de lectura analógica (equivalente a analogReadResolution)
#define resolucionEscrituraAnalogica analogWriteResolution // Función para establecer la resolución de escritura analógica (equivalente a analogWriteResolution)

#define tono tone                    // Función para generar un tono en un pin específico (equivalente a tone)
#define apagarTono noTone            // Función para apagar un tono en un pin específico (equivalente a noTone)
#define pulsacionEn pulseIn          // Función para medir la duración de un pulso en un pin (equivalente a pulseIn)
#define leerPulsoPreciso pulseInLong // Función para medir la duración precisa de un pulso en un pin (equivalente a pulseInLong)

void configurarEntrada(int pin, bool pullup = FALSO)
{
  pinMode(pin, pullup ? ENTRADA_POLARIZADA : ENTRADA); // Configura un pin como entrada con opción de pull-up (equivalente a pinMode)
}

void configurarSalida(int pin)
{
  pinMode(pin, SALIDA); // Configura un pin como salida (equivalente a pinMode)
}

int leerPin(int pin, int tipo)
{
  return tipo == DIGITAL ? digitalRead(pin) : analogRead(pin); // Lee el valor de un pin digital o analógico (equivalente a digitalRead o analogRead)
}

void encender(int pin)
{
  digitalWrite(pin, HIGH); // Establece el nivel alto en un pin (equivalente a digitalWrite)
}

void apagar(int pin)
{
  digitalWrite(pin, LOW); // Establece el nivel bajo en un pin (equivalente a digitalWrite)
}