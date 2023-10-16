#include <Arduino.h>

int user;
int user_arr[2];
int pass[2];

int error = 0;

const int led0_pin = 8;
const int led1_pin = 9;

int tog0, tog1, password;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));
  password = random(99);
  Serial.print("password: ");
  Serial.println(password);
  pass[1] = password % 10; password /= 10;
  pass[0] = password % 10;

  pinMode(led0_pin, OUTPUT);
  digitalWrite(led0_pin, LOW);
  
  pinMode(led1_pin, OUTPUT);
  digitalWrite(led1_pin, LOW);

  noInterrupts(); 
  //Timer 1 init to 1Hz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624; 
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A); 

  //Timer 3 init to 1Hz
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 15624;
  TCCR3B |= (1 << WGM32); 
  TCCR3B |= (1 << CS32) | (0 << CS31) | (1 << CS30);
  TIMSK3 |= (1 << OCIE3A); 

  interrupts(); 
}

int attempt = 3;

void loop() {
    while (attempt > 0 && Serial.available()) {
        Serial.print("in the while loop, remaining attempt: ");
        Serial.println(attempt);   

        String inputStr = Serial.readStringUntil('\n');
        int userInput = inputStr.toInt();

        Serial.println(userInput);
        attempt-=1;

        for (int j = 1; j >= 0; j--) {
            user_arr[j] = userInput % 10;
            userInput /= 10;
        }        

        if ((user_arr[0] == pass[0]) && (user_arr[1] == pass[1])) {
            tog0 = 1;
            tog1 = 1;
            Serial.print("UNLOCKED\n");
            attempt = 0;
            break;
        }

        Serial.print("REMAINING attempt: ");
        Serial.println(attempt);

        if (attempt == 0){
            error = 1;
            tog0 = 0;
            tog1 = 0;
            Serial.print("ERROR: No attempt left\n");
        }
        else {
            if (user_arr[0] == pass[0]) tog0 = 1;
            if (user_arr[1] == pass[1]) tog1 = 1;

            if (attempt == 2) {
                if (user_arr[0] != pass[0]) {
                    OCR1A = 3124;
                    tog0 = 0;
                }
                if (user_arr[1] != pass[1]) {
                    OCR3A = 3124;
                    tog1 = 0;
                }
            }
        }
    }
}

ISR(TIMER1_COMPA_vect) {
  if (tog0 == 1) digitalWrite(led0_pin, LOW);
  else if (tog0 == 0) {
    if (error) digitalWrite(led0_pin, HIGH);
    else digitalWrite(led0_pin, !digitalRead(led0_pin));
  }
}

ISR(TIMER3_COMPA_vect) {
  if (tog1 == 1) digitalWrite(led1_pin, LOW);
  else if (tog1 == 0) {
    if (error) digitalWrite(led1_pin, HIGH);
    else digitalWrite(led1_pin, !digitalRead(led1_pin));
  }
}