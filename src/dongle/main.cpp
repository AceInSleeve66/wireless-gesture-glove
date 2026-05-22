/**
 * Wireless Gesture Glove - dongle firmware (Arduino Leonardo).
 *
 * Receives gesture packets from the glove over the nRF24L01 radio link
 * and drives the PC's cursor as a USB HID mouse.
 */
#include <Arduino.h>

#include "radio.h"
#include "hid.h"
#include <protocol.h>

// Cursor sensitivity: pixels of travel per degree of hand rotation.
constexpr float CURSOR_GAIN = 8.0f;

// Shortest signed distance between two angles, in degrees (-180..180);
// keeps the cursor from leaping when a heading wraps past 360.
static float angleDelta(float current, float previous) {
    float delta = current - previous;
    while (delta > 180.0f)  { delta -= 360.0f; }
    while (delta < -180.0f) { delta += 360.0f; }
    return delta;
}

void setup() {
    Serial.begin(115200);
    hidBegin();

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
    if (packet.version != PROTOCOL_VERSION) {
        return;  // firmware built against a different protocol
    }

    // Rough pointing: yaw drives the cursor's X, pitch drives its Y.
    // The first packet only seeds the reference angles.
    static bool  havePrev  = false;
    static float prevYaw   = 0.0f;
    static float prevPitch = 0.0f;

    if (havePrev) {
        float dx = angleDelta(packet.eulerX, prevYaw)   * CURSOR_GAIN;
        float dy = angleDelta(packet.eulerZ, prevPitch) * CURSOR_GAIN;
        hidMove((int)dx, (int)dy);
    }
    prevYaw   = packet.eulerX;
    prevPitch = packet.eulerZ;
    havePrev  = true;

    // Fingertip contacts drive the buttons.
    hidLeftButton(packet.contacts & CONTACT_MIDDLE);
    hidRightButton(packet.contacts & CONTACT_RING);
}
