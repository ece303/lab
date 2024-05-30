#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 8, 9, 10, 11);

void setup() {
    pinMode(26, OUTPUT);
    analogWrite(26, 120);
    lcd.begin(16, 2);
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.write("Hello");
    lcd.setCursor(0, 1);
    lcd.write("ECE303 Students!");
}
