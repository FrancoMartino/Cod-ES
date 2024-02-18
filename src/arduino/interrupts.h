// Define traducciones de funciones relacionadas con interrupciones y eventos en español.

#define habilitarEventos interrupts()             // Habilita las interrupciones globales (equivalente a interrupts())
#define deshabilitarEventos noInterrupts()        // Deshabilita las interrupciones globales (equivalente a noInterrupts())
#define habilitarEventoExterno attachInterrupt    // Habilita un evento de interrupción externa (equivalente a attachInterrupt)
#define deshabilitarEventoExterno detachInterrupt // Deshabilita un evento de interrupción externa (equivalente a detachInterrupt)
#define pinDigitalAEvento digitalPinToInterrupt   // Convierte un pin digital a la configuración de interrupción correspondiente.