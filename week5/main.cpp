// for Experiment 
#include <Arduino.h>

int photo_pin = A0;
int val1 = 0;
unsigned int val = 0;
unsigned int counter = 0;
void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    val = Serial.parseInt();
    analogWrite(5, counter);
    delay(500);

    val1 = analogRead(photo_pin);
    Serial.println(val1);
    counter += 1;
  }



}




// // for test
// # include <Arduino.h>

// void setup(){
//   Serial.begin(9600);
//   pinMode(5,OUTPUT);
//   pinMode(6, OUTPUT);
//   pinMode(7, OUTPUT);
//   digitalWrite(5, LOW);
//   digitalWrite(6, LOW);
//   digitalWrite(7, LOW);

// }

// void loop(){
//   // digitalWrite(7, !digitalRead(7));
//   //   digitalWrite(5, !digitalRead(5));
//   //   digitalWrite(6, !digitalRead(6));
//   if( Serial.available() > 0){

    
//     char incomingByte = Serial.read();
//     Serial.print(incomingByte);
//     switch (incomingByte){
//       case 'g':
//       digitalWrite(7, !digitalRead(7));
//       break;
//       case 'r':
//       digitalWrite(5, !digitalRead(5));
//       break;
//       case 'w':
//       digitalWrite(6, !digitalRead(6));
//       break;
//     }
//   delay(500);
//   }
// }


