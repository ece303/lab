#include <Arduino.h>

// Class to manage operations of digital pins on an Arduino, particularly for handling PWM outputs and pin state control.
class DigitalPin 
{
    public:
        DigitalPin(int pin); // Constructor: initializes the digital pin.
        void set_TCCRA(byte control); // Sets the Timer/Counter Control Register A for the pin.
        void set_TCCRB(byte control); // Sets the Timer/Counter Control Register B for the pin.
        void set_TCNT(byte control); // Sets the Timer/Counter Register for counting.
        void set_OCR(unsigned int ocr_val); // Sets the Output Compare Register for PWM control.
        void factor_OCR(); // Halves the value in the Output Compare Register to double the frequency.
        void set_TIMSK(byte control); // Sets the Timer Interrupt Mask to control interrupts.
        void run_setup(byte control_vect[4], unsigned int ocr_val); // Configures the pin with a set of control vectors and a specific OCR value.
        void set_pin(); // Establishes the pin, port, and data direction configurations.
        void set_output_mode(); // Configures the pin to output mode.
        void set_input_mode(); // Configures the pin to input mode.
        void on(); // Turns the pin on by setting it high.
        void off(); // Turns the pin off by setting it low.
        void invert_pin(); // Toggles the pin state based on its current state.

    private:
        int _pin; // Pin number associated with this instance of DigitalPin.
        volatile byte *_PORT; // Pointer to the PORT register associated with _pin.
        volatile byte *_PIN; // Pointer to the PIN register for reading pin state.
        volatile byte *_DDR; // Pointer to the Data Direction Register for configuring pin direction.
        byte _opportc; // Opposite port condition, used for toggling.
        int read; // Temporary variable for reading pin state.
        volatile byte *_TCCRA; // Pointer to Timer/Counter Control Register A.
        volatile byte *_TCCRB; // Pointer to Timer/Counter Control Register B.
        volatile byte *_TCNT; // Pointer to Timer/Counter Register.
        volatile uint16_t *_OCR; // Pointer to Output Compare Register.
        volatile byte *_TIMSK; // Pointer to Timer Interrupt Mask Register.   
};

// Constructor implementation initializing the digital pin with specific configuration.
DigitalPin::DigitalPin(int pin){
    _pin = pin;
}

// Sets the Timer/Counter Control Register A for PWM and other timer configurations.
void DigitalPin::set_TCCRA(byte control){
    switch(_pin){
        case 5:
            _TCCRA = (volatile byte*)0x90;
            break;
        case 11:
            _TCCRA = (volatile byte*)0x80;
            break;
    }
    *_TCCRA = control;
}

// Sets the Timer/Counter Control Register B to configure the mode of operation for the timer.
void DigitalPin::set_TCCRB(byte control){
    switch(_pin){
        case 5:
            _TCCRB = (volatile byte*)0x91;
            break;
        case 11:
            _TCCRB = (volatile byte*)0x81;
            break;
    }
    *_TCCRB = control;
}

// Sets the timer count for operations requiring precise timing.
void DigitalPin::set_TCNT(byte control){
    switch(_pin){
        case 5:
            _TCNT = (volatile byte*)0x94;
            break;
        case 11:
            _TCNT = (volatile byte*)0x84;
            break;
    }
    *_TCNT = control;
}

// Configures interrupt behavior for the timer associated with the pin.
void DigitalPin::set_TIMSK(byte control){
    switch(_pin){
        case 5:
            _TIMSK = (volatile byte*)0x71;
            break;
        case 11:
            _TIMSK = (volatile byte*)0x6F;
            break;
    }
    *_TIMSK = control;
}

// Sets the Output Compare Register value to control PWM waveforms.
void DigitalPin::set_OCR(uint16_t ocr_val){
    switch(_pin){
        case 5:
            _OCR = (volatile uint16_t*)0x98;
            break;
        case 11:
            _OCR = (volatile uint16_t*)0x88;
            break;
    }
    *_OCR = ocr_val;
}

// Halves the current value in the Output Compare Register, effectively doubling the frequency of the PWM signal.
void DigitalPin::factor_OCR(){
    *_OCR = *_OCR / 2;
}

// Runs a full setup routine using provided timer settings and the OCR value.
void DigitalPin::run_setup(byte control_vect[4], uint16_t ocr_val){
    set_TCCRA(control_vect[0]);
    set_TCCRB(control_vect[1]);
    set_TCNT(control_vect[2]);
    set_TIMSK(control_vect[3]);
    set_OCR(ocr_val);
    set_pin();
    set_output_mode();
}

// Configures pin, port, and direction, and sets the initial state.
void DigitalPin::set_pin(){
    switch(_pin){
        case 5:
            _PIN = (volatile byte*)(0x2C);
            _PORT = (volatile byte*)(0x2E);
            _DDR = (volatile byte*)(0x2D);
            *_PORT = 0b00001000;
            _opportc = ~*_PORT;
            break;
        case 11:
            _PIN = (volatile byte*)(0X23);
            _PORT = (volatile byte*)(0x25);
            _DDR = (volatile byte*)(0x24);
            *_PORT = 0b00100000;
            _opportc = ~*_PORT;
            break;
    }
}

// Sets the pin to input mode, disabling digital output.
void DigitalPin::set_input_mode(){
    *_DDR = 0b00000000;
}

// Sets the pin to output mode, enabling digital output.
void DigitalPin::set_output_mode(){
    switch(_pin){
        case 5:
            *_DDR = 0b00001000;
            break;
        case 11:
            *_DDR = 0b00100000;
            break;
    }
}

// Turns the digital output high, enabling current flow if configured as output.
void DigitalPin::on(){
    switch(_pin){
        case 5:
            *_PORT |= 0b00001000;
            break;
        case 11:
            *_PORT |= 0b00100000;
            break;
    }
}

// Turns the digital output low, stopping current flow if configured as output.
void DigitalPin::off(){
    *_PORT &= _opportc;
}

// Inverts the current state of the pin, toggling between high and low.
void DigitalPin::invert_pin(){
    switch(_pin){
        case 5:
            read = (*_PIN >> 3 & 0b00001000 >> 3);
            if(read == 0){
                *_PORT |= 0b00001000;
            }else{
                *_PORT &= _opportc;
            }
            break;
        case 11:
            read = (*_PIN >> 5 & 0b00100000 >> 5);
            if(read == 0){
                *_PORT |= 0b00100000;
            }else{
                *_PORT &= _opportc;
            }
            break;
    }
}

class Safe 
{
    public:
        Safe();
        void code_generator(int final_code[], int code_length);
};

Safe::Safe(){
}

void Safe::code_generator(int final_code[], int code_length){
    int i = 0;
    while(i < code_length){
        final_code[i] = random(9);
        i++;
    }
}

// Set up DigitalPin and Safe classes
DigitalPin Pin1(11);
DigitalPin Pin2(5);
Safe safe;
int password[2]; // Stores the generated password digits
byte timerControlSettings[4] = {0b00000000, 0b00001100, 0b00000000, 0b00000010}; // Timer settings for DigitalPin
uint16_t timerCompareValue = 62499; // Compare value for timer interrupt
String serialInput; // String to store serial input
char lastCharRead; // Last character read from serial for clearing buffer
// Flags to trigger change of OCR value based on conditions
bool increaseSpeedFlagPin1 = false;
bool increaseSpeedFlagPin2 = false;
// State variables for managing password input logic
int attemptsRemaining = 5; // Total attempts allowed
bool initializationPrinted = false; // Flag to check if initial prompt is printed
bool systemLocked = false; // Flag to indicate system is locked
bool pin1Stopped = false; // Indicates if pin1 actions should stop
bool pin2Stopped = false; // Indicates if pin2 actions should stop
bool passwordCorrect = false; // Indicates if the entered password is correct

void setup() {
    noInterrupts(); // Disable interrupts during setup
    Serial.begin(9600); // Start serial communication at 9600 baud rate
    Pin1.run_setup(timerControlSettings, timerCompareValue); // Setup Pin1
    Pin2.run_setup(timerControlSettings, timerCompareValue); // Setup Pin2
    safe.code_generator(password, 2); // Generate a random 2-digit password
    interrupts(); // Re-enable interrupts after setup
}

void loop() {
    // Check if the initial message has been printed.
    if (!initializationPrinted) {
        // Print the initial password prompt and the remaining attempts.
        Serial.println("password:");
        Serial.println(String(password[0]) + String(password[1])); // Combine and print the password digits.
        Serial.print("In the while loop, remaining attempt: ");
        Serial.println(attemptsRemaining);
        initializationPrinted = true; // Mark that the initial message has been printed.
    }

    // Check if there is incoming data on the serial port.
    while (Serial.available() > 0) {
        serialInput = Serial.readString(); // Read the incoming data as a string.
        // Clear any remaining characters in the serial buffer.
        while (Serial.available() > 0) {
            lastCharRead = Serial.read();
        }

        // Convert the string input into a character array.
        char inputArray[serialInput.length()];
        serialInput.toCharArray(inputArray, serialInput.length());

        // Compare the first character of input against the first digit of the password.
        if ((inputArray[0] - '0') != password[0]) {
            increaseSpeedFlagPin1 = true; // Set flag to potentially increase the speed of pin 1.
        } else {
            Pin1.off(); // Turn off pin 1 if the first digit is correct.
            pin1Stopped = true; // Mark that pin 1 does not need to toggle.
        }

        // Compare the second character of input against the second digit of the password.
        if ((inputArray[1] - '0') != password[1]) {
            increaseSpeedFlagPin2 = true; // Set flag to potentially increase the speed of pin 2.
        } else {
            Pin2.off(); // Turn off pin 2 if the second digit is correct.
            pin2Stopped = true; // Mark that pin 2 does not need to toggle.
        }

        // Check if both pins are stopped which means password is correct.
        passwordCorrect = pin1Stopped && pin2Stopped;
        attemptsRemaining--; // Decrement the number of remaining attempts.

        // Handle the different cases based on the remaining attempts and password correctness.
        if (attemptsRemaining == 0 && !passwordCorrect) {
            Pin1.on(); // Turn on pin 1 to indicate the system is locked.
            Pin2.on(); // Turn on pin 2 as well.
            systemLocked = true; // Set the system locked flag.
            Serial.println("LOCKED"); // Inform the user that the system is locked.
        } else if (passwordCorrect) {
            Serial.println("UNLOCKED"); // Inform the user that the password is correct and the system is unlocked.
        } else {
            Serial.print("REMAINING attempt: "); // Prompt the user with the number of remaining attempts.
            Serial.println(attemptsRemaining);
        }
    }
}


// Interrupt service routine for TIMER1 compare match.
ISR(TIMER1_COMPA_vect) {
    // Execute only if pin 1 is not stopped and the system is not locked.
    if (!pin1Stopped && !systemLocked) {
        Pin1.invert_pin(); // Toggle the state of pin 1.
        // If the flag to increase the speed is set, adjust the timer accordingly.
        if (increaseSpeedFlagPin1) {
            increaseSpeedFlagPin1 = false; // Reset the flag.
            Pin1.factor_OCR(); // Adjust the OCR value to increase the frequency.
        }
    }
}

// Interrupt service routine for TIMER3 compare match.
ISR(TIMER3_COMPA_vect) {
    // Execute only if pin 2 is not stopped and the system is not locked.
    if (!pin2Stopped && !systemLocked) {
        Pin2.invert_pin(); // Toggle the state of pin 2.
        // If the flag to increase the speed is set, adjust the timer accordingly.
        if (increaseSpeedFlagPin2) {
            increaseSpeedFlagPin2 = false; // Reset the flag.
            Pin2.factor_OCR(); // Adjust the OCR value to increase the frequency.
        }
    }
}
