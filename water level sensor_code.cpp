const int sensorPin = A0; // Pin connected to the water level sensor
int sensorValue = 0;      // Variable to store the sensor value

void setup() {
    Serial.begin(9600);   // Start serial communication
    pinMode(sensorPin, INPUT); // Set the sensor pin as input
}

void loop() {
    sensorValue = analogRead(sensorPin); // Read the sensor value
    Serial.print("Water Level: ");
    Serial.println(sensorValue); // Print the value to the serial monitor
    delay(1000); // Wait for 1 second
}
