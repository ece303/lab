#include <Arduino.h>

int password;  // store the generated password
int led0 = 8;  // pin number for the LED
int tog0;      // status of LED status

void setup() {
  
  Serial.begin(9600); 

  // Initialize the random seed and generate a password between 0 to 9
  randomSeed(analogRead(0));
  password = random(10);  
  Serial.print("password: ");
  Serial.println(password);  

  // Set up the LED pin and turn it off initially
  pinMode(led0, OUTPUT);
  digitalWrite(led0, LOW);

  noInterrupts();  

  // Initialize Timer 1 for 1Hz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  
  TCCR1B |= (1 << WGM12);  
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A); 

  interrupts();  
}

int attempt = 3;  // number of attempts 

void loop() {  
  
  // continue to check for input while there are remaining attempts and serial data is available
  while(attempt > 0 && Serial.available()){
    Serial.print("in the while loop, remaining attempt: ");
    Serial.println(attempt);   
    
    String inputStr = Serial.readStringUntil('\n');  // read user's input from serial
    int userInput = inputStr.toInt();  
    
    Serial.println(userInput);   
    attempt--;  // decrease the number of attempts

    // if user's input matches the password
    if (userInput == password) {
      Serial.println("in the Second if statement");
      tog0 = 1;  
      Serial.println("UNLOCKED\n");
      attempt = 0; 
      break;
    }  
    
    Serial.print("REMAINING attempt: ");
    Serial.println(attempt);  

    // change Timer1 frequency and LED status based on remaining attempts
    if (attempt == 2) {
      OCR1A = 3124;
      tog0 = 0;
    }
    if (attempt == 1) {
      OCR1A = 624;
      tog0 = 0;
    }      
    if (attempt == 0) Serial.println("ERROR: No attempt left");  
  } 
} 

// interrupt service routine for Timer1 compare match
ISR(TIMER1_COMPA_vect) {
  if (tog0 == 1) digitalWrite(led0, LOW);  // turn off the LED if password is correct
  else digitalWrite(led0, !digitalRead(led0));  // blink the LED if password is wrong
}
