#include <Arduino.h>

class DigitalPin 
{
    public:
        DigitalPin(int pin);  // Constructor to initialize the digital pin.
        void startADC(); // Method to configure ADC.
        int readADC(); // Method to read ADC values.

    private:
        int _pin; // Pin number associated with this instance.
        volatile uint16_t *_OCR; // Pointer to Output Compare Register for PWM control.
};

DigitalPin::DigitalPin(int pin) : _pin(pin) {}

void DigitalPin::startADC() {
    // Configure ADC registers based on the microcontroller specifications.
    ADCSRA |= (1 << ADEN);  // Enable ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Set ADC prescaler to 128
    ADMUX |= (1 << REFS0);  // Set reference voltage to AVcc
    ADMUX = (ADMUX & 0xF8) | (_pin - 14);  // Select ADC channel corresponding to pin
}

int DigitalPin::readADC() {
    ADCSRA |= (1 << ADSC);  // Start conversion
    while (ADCSRA & (1 << ADSC));  // Wait for conversion to finish
    return ADC;  // Return the read value
}

// Example usage:
DigitalPin photoresistorPin(A0); // Create a DigitalPin object for pin A0

void setup() {
    Serial.begin(9600);
    photoresistorPin.startADC(); // Configure ADC
}

void loop() {
    int photoresistorValue = photoresistorPin.readADC(); // Read analog value
    float voltage = photoresistorValue * (5.0 / 1023.0); // Convert the read value to voltage
    Serial.println(voltage); // Print the voltage
    delay(1000); // Delay for 1 second
}
