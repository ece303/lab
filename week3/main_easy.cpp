#include <Arduino.h>

int user;
int user_arr[2];
int pass[2];

int error = 0;

#define led0_bit PE3  // Pin 5 on Mega 2560 (Correct as it is linked to Timer 3)
#define led1_bit PB5  // Pin 11 on Mega 2560, actually connected to Timer 1

int tog0, tog1, password;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));
  password = random(10,99);
  Serial.print("password: ");
  Serial.println(password);
  pass[1] = password % 10; password /= 10;
  pass[0] = password % 10;

  // Set pin to only output and low
  DDRE |= (1 << led0_bit);  // Set pin 5 as output
  PORTE &= ~(1 << led0_bit);  // Set pin 5 low
  DDRB |= (1 << led1_bit);  // Set pin 11 as output
  PORTB &= ~(1 << led1_bit);  // Set pin 11 low

  noInterrupts();
  //Timer 3 init to 1Hz for LED on Pin 5
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 15624;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS32) | (0 << CS31) | (1 << CS30);
  TIMSK3 |= (1 << OCIE3A);  // Enable interrupt for OCR3A

  //Timer 1 init to 1Hz for LED on Pin 11
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);  // Enable interrupt for OCR1A

  interrupts();
}


int attempt = 5;

void loop() {
    while (attempt > 0 && Serial.available()) {
        Serial.print("In the while loop, remaining attempt: ");
        Serial.println(attempt);

        String inputStr = Serial.readStringUntil('\n');
        int userInput = inputStr.toInt();

        Serial.println(userInput);
        attempt -= 1;

        for (int j = 1; j >= 0; j--) {
            user_arr[j] = userInput % 10;
            userInput /= 10;
        }        

        if ((user_arr[0] == pass[0]) && (user_arr[1] == pass[1])) {
            tog0 = 1;
            tog1 = 1;
            Serial.println("UNLOCKED");
            attempt = 0;
            break;
        }

        Serial.print("REMAINING attempt: ");
        Serial.println(attempt);

        if (attempt == 0){
            error = 1;
            tog0 = 0;
            tog1 = 0;
            Serial.println("ERROR: No attempt left");
        } else {
            // Check each digit of the password
            if (user_arr[0] == pass[0]) {
                tog0 = 1; // Correct guess for first digit
            } else {
                tog0 = 0; // Incorrect guess for first digit
                if (attempt == 2) { // Increase blinking speed on next wrong attempt
                    OCR3A = 3124; // Timer for Pin 5
                }
            }

            if (user_arr[1] == pass[1]) {
                tog1 = 1; // Correct guess for second digit
            } else {
                tog1 = 0; // Incorrect guess for second digit
                if (attempt == 2) { // Increase blinking speed on next wrong attempt
                    OCR1A = 3124; // Timer for Pin 11
                }
            }
        }
    }
}


// Loop and other functions remain the same

ISR(TIMER3_COMPA_vect) {
  if (tog0 == 1) PORTE &= ~(1 << led0_bit);
  else if (tog0 == 0) {
    if (error) PORTE |= (1 << led0_bit);
    else PORTE ^= (1 << led0_bit);
  }
}

ISR(TIMER1_COMPA_vect) {
  if (tog1 == 1) PORTB &= ~(1 << led1_bit);
  else if (tog1 == 0) {
    if (error) PORTB |= (1 << led1_bit);
    else PORTB ^= (1 << led1_bit);
  }
}
