/**
 * Wireless Gesture Glove - dongle firmware (Arduino Leonardo).
 *
 * Receives gesture packets from the glove over the nRF24L01 radio link
 * and prints them. USB HID mouse output is added in a later phase -
 * see README.md.
 */
#include <Arduino.h>

#include "radio.h"
#include <protocol.h>

void setup() {
    Serial.begin(115200);

    while (!radioBegin()) {
        Serial.println("nRF24L01 not detected - check wiring");
        delay(1000);
    }
    Serial.println("nRF24L01 ready");
}

void loop() {
    GesturePacket packet;
    if (!radioReceive(packet)) {
        return;
    }

    // Reject packets from firmware built against a different protocol.
    if (packet.version != PROTOCOL_VERSION) {
        Serial.println("dropped packet - protocol version mismatch");
        return;
    }

    // The AVR core's Serial has no printf - print the fields one by one.
    Serial.print("rx  x=");
    Serial.print(packet.eulerX, 1);
    Serial.print("  y=");
    Serial.print(packet.eulerY, 1);
    Serial.print("  z=");
    Serial.print(packet.eulerZ, 1);
    Serial.print("  contacts=0x");
    Serial.println(packet.contacts, HEX);
}
