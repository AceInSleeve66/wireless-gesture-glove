/**
 * Wireless Gesture Glove - radio module (glove).
 *
 * Wraps the nRF24L01 as a transmitter. See radio.h for the interface.
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

    // Settings the dongle must match - see protocol.h for the shared values.
    radio.setPALevel(RF24_PA_LOW);
    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF24_1MBPS);

    radio.openWritingPipe(RF_ADDRESS);
    radio.stopListening();
    return true;
}

bool radioSend(const GesturePacket& packet) {
    return radio.write(&packet, sizeof(packet));
}
