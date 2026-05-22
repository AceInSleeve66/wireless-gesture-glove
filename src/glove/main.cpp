/**
 * Wireless Gesture Glove - glove firmware (ESP32).
 *
 * Reads the IMU and finger-touch inputs, assembles a GesturePacket and
 * transmits it to the dongle over the nRF24L01 radio link.
 */
#include <Arduino.h>

#include "imu.h"
#include "touch.h"
#include "radio.h"
#include <protocol.h>

// nRF24L01 transmit cadence. 50 Hz is smooth for pointing and leaves the
// radio idle between packets; the contacts are still sampled every loop.
constexpr unsigned long TX_INTERVAL_MS = 20;

void setup() {
    Serial.begin(115200);

    touchBegin();

    while (!imuBegin()) {
        Serial.println("BNO055 not detected - check I2C wiring");
        delay(1000);
    }
    Serial.println("BNO055 ready");

    while (!radioBegin()) {
        Serial.println("nRF24L01 not detected - check wiring");
        delay(1000);
    }
    Serial.println("nRF24L01 ready");
}

void loop() {
    // Sample the contacts every pass so debouncing stays crisp.
    TouchState t = touchRead();

    static unsigned long lastTx = 0;
    if (millis() - lastTx < TX_INTERVAL_MS) {
        return;
    }
    lastTx = millis();

    Orientation o = imuRead();

    // main.cpp assembles the wire packet from the sensor modules' output.
    GesturePacket packet;
    packet.version  = PROTOCOL_VERSION;
    packet.contacts = 0;
    if (t.contact[FINGER_MIDDLE]) {
        packet.contacts |= CONTACT_MIDDLE;
    }
    if (t.contact[FINGER_RING]) {
        packet.contacts |= CONTACT_RING;
    }
    if (t.contact[FINGER_PINKY]) {
        packet.contacts |= CONTACT_PINKY;
    }
    packet.eulerX = o.x;
    packet.eulerY = o.y;
    packet.eulerZ = o.z;

    bool ok = radioSend(packet);

    // Report transmit status at a readable rate.
    static unsigned long lastReport = 0;
    if (millis() - lastReport >= 250) {
        lastReport = millis();
        Serial.printf("tx %s  x=%6.1f y=%6.1f z=%6.1f  contacts=0x%02X\n",
                      ok ? "ok" : "--",
                      packet.eulerX, packet.eulerY, packet.eulerZ,
                      packet.contacts);
    }
}
