/**
 * Wireless Gesture Glove - HID module (dongle).
 *
 * Wraps the Arduino Mouse library. See hid.h for the interface.
 */
#include "hid.h"

#include <Arduino.h>
#include <Mouse.h>

void hidBegin() {
    Mouse.begin();
}

void hidMove(int dx, int dy) {
    // Mouse.move takes a signed char per axis, clamp so a large delta
    // cannot wrap around. Skip an empty move to avoid idle USB traffic.
    dx = constrain(dx, -127, 127);
    dy = constrain(dy, -127, 127);
    if (dx != 0 || dy != 0) {
        Mouse.move(dx, dy);
    }
}

void hidLeftButton(bool pressed) {
    // Mouse.press/release only emit a report when the state changes, so
    // mirroring the contact on every packet is safe.
    if (pressed) {
        Mouse.press(MOUSE_LEFT);
    } else {
        Mouse.release(MOUSE_LEFT);
    }
}

void hidRightButton(bool pressed) {
    if (pressed) {
        Mouse.press(MOUSE_RIGHT);
    } else {
        Mouse.release(MOUSE_RIGHT);
    }
}
