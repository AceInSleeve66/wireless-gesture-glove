/**
 * Wireless Gesture Glove - radio module (dongle).
 *
 * Wraps the nRF24L01 as a receiver. See radio.h for the interface.
 */
#include "radio.h"

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

static RF24 radio(RF_CE_PIN, RF_CSN_PIN);

bool radioBegin() {
    if (!radio.begin()) {
        return false;
    }

    // Settings the glove must match - see protocol.h for the shared values.
    radio.setPALevel(RF24_PA_LOW);
    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF24_1MBPS);

    radio.openReadingPipe(0, RF_ADDRESS);
    radio.startListening();
    return true;
}

bool radioReceive(GesturePacket& packet) {
    if (!radio.available()) {
        return false;
    }

    radio.read(&packet, sizeof(packet));
    return true;
}
