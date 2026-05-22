/**
 * Wireless Gesture Glove - IMU module (glove).
 *
 * Wraps the BNO055 over I2C. See imu.h for the interface.
 */
#include "imu.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// BNO055 wiring: I2C on SDA 21 / SCL 22, address 0x28.
// The sensor ID is an arbitrary tag the Adafruit driver attaches to events.
constexpr int     BNO055_SENSOR_ID = 55;
constexpr uint8_t BNO055_I2C_ADDR  = 0x28;
constexpr int     I2C_SDA_PIN      = 21;
constexpr int     I2C_SCL_PIN      = 22;

static Adafruit_BNO055 bno(BNO055_SENSOR_ID, BNO055_I2C_ADDR);

bool imuBegin() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!bno.begin()) {
        return false;
    }

    // Let the sensor settle then switch to the board's external crystal
    // for a steadier clock and better orientation accuracy.
    delay(1000);
    bno.setExtCrystalUse(true);
    return true;
}

Orientation imuRead() {
    sensors_event_t event;
    bno.getEvent(&event);
    return Orientation{event.orientation.x,
                       event.orientation.y,
                       event.orientation.z};
}
