#include <Arduino.h>

//pin setting
#define TRIGGER_PIN  2  // trigger pin
#define ECHO_PIN     4  // echo pin

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);  // trigger pin as output
  pinMode(ECHO_PIN, INPUT);      // echo pin as input
  Serial.begin(9600);        
  pinMode(6, OUTPUT);
}

void loop() {
  // send sonar signal
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // get signal from pin
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distanceCm = duration * 0.034 / 2; // distance(cm) (using sonar)
  float distanceIn = distanceCm / 2.54; // distance(inch)

  // print distance
  Serial.print("distance: ");
  Serial.print(distanceIn);
  Serial.println(" in");
  if( distanceIn < 5){
    Serial.print("distance is below 5 in");
    digitalWrite(6, HIGH);
  }
  else{
    digitalWrite(6, 0);
  }

  delay(1000*0.5); // delay 0.5second
}
