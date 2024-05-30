#include <IRremote.h>
#include <IRremoteInt.h>

#define RED_LED_PIN 53
#define YELLOW_LED_PIN 51
#define BLUE_LED_PIN 49
#define GREEN_LED_PIN 47

#define ONE 16724175
#define TWO 16718055
#define THREE 16743045
#define FOUR 16716015
#define FIVE 16726215
#define SIX 16734885
#define SEVEN 16728765
#define EIGHT 16730805
#define NINE 16732845
#define ZERO 16738455

IRrecv irrecv(2);
decode_results results;

void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    irrecv.enableIRIn();  // Start the receiver
}

void loop() {
    if (irrecv.decode(&results)) {
        if (results.value == ONE) {  // Check if the key 1 is pressed
            digitalWrite(RED_LED_PIN, HIGH);  // Turn on the red LED
        } else if (results.value == TWO) {  // Check if the key 2 is pressed
            digitalWrite(BLUE_LED_PIN, HIGH);  // Turn on the blue LED
        } else if (results.value == THREE) {  // Check if the key 3 is pressed
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Turn on the yellow LED
        } else if (results.value == FOUR) {  // Check if the key 4 is pressed
            digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on the green LED
        } else if (results.value == FIVE) {  // Check if the key 5 is pressed
            digitalWrite(RED_LED_PIN, LOW);  // Turn off the red LED
        } else if (results.value == SIX) {  // Check if the key 6 is pressed
            digitalWrite(BLUE_LED_PIN, LOW);  // Turn off the blue LED
        } else if (results.value == SEVEN) {  // Check if the key 7 is pressed
            digitalWrite(YELLOW_LED_PIN, LOW);  // Turn off the yellow LED
        } else if (results.value == EIGHT) {  // Check if the key 8 is pressed
            digitalWrite(GREEN_LED_PIN, LOW);  // Turn off the green LED
        } else if (results.value == NINE) {  // Check if the key 9 is pressed
            digitalWrite(RED_LED_PIN, HIGH);  // Turn on the red LED
            digitalWrite(BLUE_LED_PIN, HIGH);  // Turn on the blue LED
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Turn on the yellow LED
            digitalWrite(GREEN_LED_PIN, HIGH);  // Turn on the green LED
        } else if (results.value == ZERO) {  // Check if the key 0 is pressed
            digitalWrite(RED_LED_PIN, LOW);  // Turn off the red LED
            digitalWrite(BLUE_LED_PIN, LOW);  // Turn off the blue LED
            digitalWrite(YELLOW_LED_PIN, LOW);  // Turn off the yellow LED
            digitalWrite(GREEN_LED_PIN, LOW);  // Turn off the green LED
        }
        irrecv.resume();  // Receive the next value
    }
}
