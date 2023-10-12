#include <Arduino.h>

int password;
int led0_bit = 5; 
int tog0;

void setup() {
  
  Serial.begin(9600);

  randomSeed(analogRead(0));
  password = random(10);  
  Serial.print("password: ");
  Serial.println(password);

  DDRH |= (1 << led0_bit);  // Set as output
  PORTH &= ~(1 << led0_bit);  // Set LOW

  noInterrupts();  


  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);  

  interrupts();  
}
int attempt = 3;

void loop() {  
  
  while( attempt > 0 && Serial.available()){
    Serial.print("in the while loop, remaining attempt: ");
    Serial.println(attempt);   
    
    String inputStr = Serial.readStringUntil('\n');
    int userInput = inputStr.toInt();
    
    Serial.println(userInput);
    attempt-=1;

    if (userInput == password) {
      Serial.println("in the Second if statement");
      tog0 = 1;
      Serial.println("UNLOCKED\n");
      attempt = 0;
      break;
    }  
    
    Serial.print("REMAINING attempt: ");
    Serial.println(attempt);

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


ISR(TIMER1_COMPA_vect) {
  if (tog0 == 1) PORTH &= ~(1 << led0_bit);  //
  else PORTH ^= (1 << led0_bit);  //
}