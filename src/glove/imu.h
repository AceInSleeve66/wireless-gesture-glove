/**
 * Wireless Gesture Glove - IMU module (glove).
 *
 * Wraps the BNO055 9-axis IMU: I2C start-up and absolute-orientation
 * read-out as Euler angles. The glove uses this to sense hand pose.
 */
#pragma once

// Absolute hand orientation from the BNO055 - Euler angles, in degrees.
// Axes are the sensor's own and match GesturePacket's euler fields.
struct Orientation {
    float x;  // heading / yaw
    float y;  // roll
    float z;  // pitch
};

// Starts the BNO055 over I2C. Returns false if the sensor is not detected.
bool imuBegin();

// Reads the current absolute orientation as Euler angles, in degrees.
Orientation imuRead();
