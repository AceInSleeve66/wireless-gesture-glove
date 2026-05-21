#include <Arduino.h>

const int button2 = 2;
const int button3 = 3;

void setup() {
  Serial.begin(115200);

  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
}

void loop() {

  if (digitalRead(button2) == LOW) {
    Serial.println("Button 2 pressed");
    delay(200);
  }

  if (digitalRead(button3) == LOW) {
    Serial.println("Button 3 pressed");
    delay(200);
  }
}
