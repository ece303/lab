#include <Arduino.h>

// Pin assignments for the LED and the photoresistor
int ledPin = 6; 
int photoresistorPin = A0; 

// Variables used for PWM control and reading the photoresistor's value
float currentDutyCycle = 0.0; // Current duty cycle, ranging from 0 to 1
int photoResistorValue;  // Placeholder for future use if needed
int fixedResistorValue = 10000;  // Resistance value of the 10kΩ resistor used in voltage divider
float dutyCyclePercentage; 

void setup() {
  Serial.begin(9600); 
  Serial.println("Duty Cycle(%), Voltage across 10kΩ Resistor, Voltage across Photoresistor"); 

  pinMode(ledPin, OUTPUT);  
  pinMode(photoresistorPin, INPUT); 
  
  // Timer configuration
  noInterrupts();
  TCCR4A = 0b10000010; 
  TCCR4B = 0b00010010; 
  TCNT4 = 0; 
  OCR4A = 0; 
  ICR4 = 400; 
  interrupts(); 
}

void loop() {
  // Adjust the PWM signal's duty cycle
  if (currentDutyCycle > 1.01) {
    currentDutyCycle = 0.05;  // Reset duty cycle if it exceeds 1
  } 
  OCR4A = (ICR4 * currentDutyCycle);  // Set the duty cycle for the PWM signal
  dutyCyclePercentage = currentDutyCycle * 100;  // Convert duty cycle to percentage
  currentDutyCycle += 0.05;  // Increment duty cycle

  delay(2000);
  
  // Read the voltage across the photoresistor
  float voltageAcrossFixedResistor;
  float currentThroughFixedResistor;
  float voltageAcrossPhotoresistor;
  float resistanceOfPhotoresistor;

  // Print the data to the serial monitor
  Serial.print(String(dutyCyclePercentage) + ", " + String(voltageAcrossFixedResistor) + ", " + String(voltageAcrossPhotoresistor) + "\n");
}
