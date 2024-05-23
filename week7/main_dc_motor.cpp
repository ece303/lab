const int forward = 5;
const int backward = 7;
int time_delay = 3000;
int val = 0;

void setup() {
    pinMode(forward, OUTPUT);
    pinMode(backward, OUTPUT);
    digitalWrite(backward, LOW);
    digitalWrite(forward, HIGH);
}

void loop() {
    delay(time_delay);
    brake();
    delay(time_delay);
    go_backward();
    delay(time_delay);
    brake();
    delay(time_delay);
    go_forward();
}

void go_forward() {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
}

void go_backward() {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
}

void brake() {
    digitalWrite(forward, LOW);
    digitalWrite(backward, LOW);
}
