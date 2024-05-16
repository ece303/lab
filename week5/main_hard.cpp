#include <Arduino.h>

// Class to handle digital pin operations and ADC
class DigitalPin 
{
    public:
        DigitalPin(int pin);  // Constructor to initialize the digital pin.
        void startADC(); // Method to configure ADC.
        int readADC(); // Method to read ADC values.

    private:
        int _pin; // Pin number associated with this instance.
};

// Class to handle PWM control for the LED
class PWMPin 
{
    public:
        PWMPin(int pin, int timer);  // Constructor to initialize the PWM pin.
        void startPWM(); // Method to configure PWM.
        void my_analogWrite(int value); // Custom method to set the duty cycle for PWM.

    private:
        int _pin; // Pin number associated with this instance.
        int _timer; // Timer number associated with this instance.
        volatile uint16_t *_OCR; // Pointer to Output Compare Register for PWM control.
};

DigitalPin::DigitalPin(int pin) : _pin(pin) {}

void DigitalPin::startADC() {
    // Configure ADC registers based on the microcontroller specifications.
    ADCSRA |= (1 << ADEN);  // Enable ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Set ADC prescaler to 128
    ADMUX |= (1 << REFS0);  // Set reference voltage to AVcc
    ADMUX = (ADMUX & 0xF8) | (_pin - A0);  // Select ADC channel corresponding to pin
}

int DigitalPin::readADC() {
    ADCSRA |= (1 << ADSC);  // Start conversion
    while (ADCSRA & (1 << ADSC));  // Wait for conversion to finish
    return ADC;  // Return the read value
}

PWMPin::PWMPin(int pin, int timer) : _pin(pin), _timer(timer) {}

void PWMPin::startPWM() {
    pinMode(_pin, OUTPUT);

    if (_timer == 4) {
        // Configure Timer4 for Fast PWM mode
        TCCR4A = 0;
        TCCR4B = 0;
        TCNT4 = 0;
        
        TCCR4A = (1 << COM4A1) | (1 << WGM41); // Fast PWM, clear OC4A on compare match
        TCCR4B = (1 << WGM43) | (1 << WGM42) | (1 << CS41); // Fast PWM, prescaler 8
        ICR4 = 39999; // Set top value for 50Hz (20ms) PWM
        _OCR = &OCR4A;
    }
    // Add configuration for other timers if necessary
}

void PWMPin::my_analogWrite(int value) {
    if (_timer == 4) {
        // Map the value from 0-255 to 0-39999 (ICR4 value)
        int dutyCycle = map(value, 0, 255, 0, ICR4);
        *_OCR = dutyCycle;
    }
    // Add handling for other timers if necessary
}

// Create DigitalPin and PWMPin objects
DigitalPin photoresistorPin(A0); // Photoresistor connected to pin A0
PWMPin ledPin(6, 4); // LED connected to pin 6 with timer 4

unsigned int counter = 0;

void setup() {
    Serial.begin(9600);
    photoresistorPin.startADC(); // Configure ADC
    ledPin.startPWM(); // Configure PWM
}

void loop() {
    if (Serial.available() > 0) {
        unsigned int val = Serial.parseInt();
        ledPin.my_analogWrite(counter);  // Set the duty cycle for the PWM signal
        delay(500);

        int val1 = photoresistorPin.readADC();
        Serial.println(val1);
        counter += 1;
        if (counter > 255) {
            counter = 0;
        }
    }
}
