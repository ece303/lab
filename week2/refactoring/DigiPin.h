#ifndef DIGIPIN_H
#define DIGIPIN_H

#include <Arduino.h>

class DigitalPin {
public:
    DigitalPin(int pin);
    
    void setTCCRA(uint8_t value);
    void setTCCRB(uint8_t value);
    void setTCNT(uint16_t value);
    void setOCR(uint16_t value);
    void factorOCR(float factor);
    void setTIMSK(uint8_t value);
    int getPin() const;


private:
    int pinNumber;
};

#endif