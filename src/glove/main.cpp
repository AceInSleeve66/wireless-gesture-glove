/**
 * Wireless Gesture Glove — glove firmware (ESP32).
 *
 * Reads the IMU and finger-touch inputs and streams them to the dongle
 * over the nRF24L01 radio link. Modules are added per phase — see README.md.
 */
#include <Arduino.h>

#include <protocol.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
}
