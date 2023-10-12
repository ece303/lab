#include "DigiPin.h"

DigitalPin::DigitalPin(int pin) : pinNumber(pin) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
}

void DigitalPin::setTCCRA(uint8_t value) { TCCR1A = value; }
void DigitalPin::setTCCRB(uint8_t value) { TCCR1B = value; }
void DigitalPin::setTCNT(uint16_t value) { TCNT1 = value; }
void DigitalPin::setOCR(uint16_t value) { OCR1A = value; }
void DigitalPin::factorOCR(float factor) { OCR1A = (uint16_t)(OCR1A * factor); }
void DigitalPin::setTIMSK(uint8_t value) { TIMSK1 = value; }
int DigitalPin::getPin() const {
    return pinNumber;
}