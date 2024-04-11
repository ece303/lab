#include <Arduino.h>
class Morse
{
    public:
        Morse(int pin);
        void dot();
        void dash();
    private:
        int _pin;
};
Morse::Morse(int pin)
{
    pinMode(pin, OUTPUT);
    _pin = pin;

}

void Morse::dot()
{
    digitalWrite(_pin, HIGH);
    delay(250);
    digitalWrite(_pin, LOW);
    delay(250);
}

void Morse::dash()
{
    digitalWrite(_pin, HIGH);
    delay(1000);
    digitalWrite(_pin, LOW);
    delay(250);

}

// put function declarations here:
// int myFunction(int, int);
int pin = 8;//11; //8; //Pin LED is attached to 
int pin2 = 7;//10;
int pin3 = 6;//9;
int pin4 = 5;//8;
char temp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

Morse morse(pin);
Morse morse2(pin2);
Morse morse3(pin3);
Morse morse4(pin4);

int blinkMorse(String);
int turnLED(int);

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command.startsWith("blink")) {//if (command == "blink") {
            int spaceIndex = command.indexOf(' ');
            
            if(spaceIndex != -1) {
                String valueStr = command.substring(spaceIndex + 1);                
                blinkMorse(valueStr);
            }
        } else if (command.startsWith("turn")) {
            int spaceIndex = command.indexOf(' ');
            if(spaceIndex != -1) {
                String valueStr = command.substring(spaceIndex + 1);
                int value = valueStr.toInt();
                turnLED(value);
            }
        }
    }
}

int blinkMorse(String value) {
    if( value.startsWith("red") ){
        morse = morse;
        Serial.println("blink red");
        Serial.println(value);
    }
    else if( value.startsWith("yellow") ){
        morse = morse2;
        Serial.println("blink yellow");
        
    }
    else if( value.startsWith("green") ){
        morse = morse3;
        Serial.println("blink green");
    }
    else{
        morse = morse4;
        Serial.println("blink blue");
    }
    morse.dot(); morse.dot(); morse.dot();
    morse.dash(); morse.dash(); morse.dash();
    morse.dot(); morse.dot(); morse.dot();
    delay(2000);

    return 1;
}

int turnLED(int value) {
    analogWrite(pin4, value);
    String sm = "Received Input: " + String(value);
    Serial.println(sm);
    delay(2000);

    return 1;
}


















