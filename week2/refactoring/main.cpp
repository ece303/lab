#include "DigiPin.h"
#include "Safe.h"

DigitalPin led(8);
Safe safe;

void setup() {
    Serial.begin(9600);

    led.setTCCRA(0);
    led.setTCCRB((1 << WGM12) | (1 << CS12) | (1 << CS10));
    led.setTCNT(0);
    led.setOCR(15624);  
    led.setTIMSK(1 << OCIE1A);

    noInterrupts();  
    interrupts();  
}

int attempts = 3;

void loop() {
    while(attempts > 0 && Serial.available()) {
        String inputStr = Serial.readStringUntil('\n');
        int userInput = inputStr.toInt();

        Serial.println(userInput);
        attempts--;

        if(userInput == safe.getPassword()) {
            led.setOCR(0);  
            Serial.println("UNLOCKED\n");
            attempts = 0;  
            break;
        } else {
            if (attempts == 2) led.factorOCR(0.2); 
            if (attempts == 1) led.factorOCR(0.5); 
        }

        Serial.print("REMAINING attempt: ");
        Serial.println(attempts);
        if(attempts == 0) Serial.println("ERROR: No attempt left");
    }
    delay(1000);  
}

ISR(TIMER1_COMPA_vect) {
    digitalWrite(led.getPin(), !digitalRead(led.getPin())); 
}
