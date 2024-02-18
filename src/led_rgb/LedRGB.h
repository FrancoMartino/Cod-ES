#ifndef LedRGB_h
#define LedRGB_h

#include <Arduino.h>

class LedRGB {
  private:
    int rPin;
    int gPin;
    int bPin;
    int rValue;
    int gValue;
    int bValue;
    
  public:
    LedRGB(int rPin, int gPin, int bPin);
    void encender();
    void apagar();
    void cambiarColor(int red, int green, int blue);
};

#endif