/**
 * Wireless Gesture Glove — dongle firmware (Arduino Leonardo).
 *
 * Receives gesture packets from the glove over the nRF24L01 radio link
 * and emits USB HID mouse events to the host PC. Modules are added per
 * phase — see README.md.
 */
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
}
