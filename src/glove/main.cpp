/**
 * Wireless Gesture Glove — glove firmware (ESP32).
 *
 * Reads the IMU and finger-touch inputs and streams them to the dongle
 * over the nRF24L01 radio link. Modules are added per phase — see README.md.
 */
#include <Arduino.h>

#include "imu.h"
#include "touch.h"
#include <protocol.h>

void setup() {
    Serial.begin(115200);

    touchBegin();

    while (!imuBegin()) {
        Serial.println("BNO055 not detected - check I2C wiring");
        delay(1000);
    }
    Serial.println("BNO055 ready");
}

void loop() {
    TouchState t = touchRead();

    // Report taps and holds the instant they are classified.
    for (int i = 0; i < FINGER_COUNT; i++) {
        if (t.gesture[i] == GESTURE_TAP) {
            Serial.printf("finger %d  tap\n", i);
        } else if (t.gesture[i] == GESTURE_HOLD) {
            Serial.printf("finger %d  hold\n", i);
        }
    }

    // Stream orientation and contact levels at a steady readable rate.
    static unsigned long lastReport = 0;
    if (millis() - lastReport >= 100) {
        lastReport = millis();
        Orientation o = imuRead();
        Serial.printf("orientation  x=%6.1f  y=%6.1f  z=%6.1f   "
                      "contacts  M%d R%d P%d\n",
                      o.x, o.y, o.z,
                      t.contact[FINGER_MIDDLE],
                      t.contact[FINGER_RING],
                      t.contact[FINGER_PINKY]);
    }
}
