/**
 * Wireless Gesture Glove — glove firmware (ESP32).
 *
 * Reads the IMU and finger-touch inputs and streams them to the dongle
 * over the nRF24L01 radio link. Modules are added per phase — see README.md.
 */
#include <Arduino.h>

#include "imu.h"
#include <protocol.h>

void setup() {
    Serial.begin(115200);

    while (!imuBegin()) {
        Serial.println("BNO055 not detected - check I2C wiring");
        delay(1000);
    }
    Serial.println("BNO055 ready");
}

void loop() {
    Orientation o = imuRead();
    Serial.printf("orientation  x=%6.1f  y=%6.1f  z=%6.1f\n", o.x, o.y, o.z);
    delay(100);
}
