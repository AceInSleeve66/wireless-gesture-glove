/**
 * Wireless Gesture Glove - touch module (glove).
 *
 * Wraps the fingertip contacts. See touch.h for the interface.
 */
#include "touch.h"

#include <Arduino.h>

// Contact wiring: each fingertip pad is read with INPUT_PULLUP and the
// thumb is tied to GND, so a closed contact pulls the pin LOW. These
// GPIOs all support an internal pull-up and avoid the ESP32 strapping
// pins. Change them here to match how the glove is wired.
constexpr uint8_t TOUCH_PINS[FINGER_COUNT] = {
    25, // FINGER_MIDDLE
    26, // FINGER_RING
    27, // FINGER_PINKY
};

// A contact must read steady this long to count as a state change -
// rejects switch-bounce and skin-contact chatter.
constexpr unsigned long DEBOUNCE_MS = 35;

// A contact held at least this long is a hold. Shorter ones are taps.
constexpr unsigned long HOLD_MS = 1600;

// Per-finger debounce and tap/hold tracking, kept between touchRead() calls.
struct FingerState {
    bool          contact   = false;  // last debounced level
    bool          lastRaw   = false;  // last raw sample, for bounce timing
    unsigned long changedAt = 0;      // millis() of the last raw change
    unsigned long closedAt  = 0;      // millis() the contact closed
    bool          holdFired = false;  // GESTURE_HOLD already reported this contact
};

static FingerState fingers[FINGER_COUNT];

void touchBegin() {
    for (int i = 0; i < FINGER_COUNT; i++) {
        pinMode(TOUCH_PINS[i], INPUT_PULLUP);
    }
}

TouchState touchRead() {
    TouchState state;
    unsigned long now = millis();

    for (int i = 0; i < FINGER_COUNT; i++) {
        FingerState& f = fingers[i];

        // INPUT_PULLUP: a closed fingertip-to-thumb contact pulls the pin LOW.
        bool raw = (digitalRead(TOUCH_PINS[i]) == LOW);

        // Restart the debounce timer whenever the raw sample changes.
        if (raw != f.lastRaw) {
            f.lastRaw = raw;
            f.changedAt = now;
        }

        Gesture gesture = GESTURE_NONE;

        // Accept the raw level once it has held steady past the bounce window.
        if (raw != f.contact && (now - f.changedAt) >= DEBOUNCE_MS) {
            f.contact = raw;
            if (raw) {
                // Contact just closed - start timing for tap vs hold.
                f.closedAt = now;
                f.holdFired = false;
            } else if (!f.holdFired) {
                // Released before the hold threshold - a tap.
                gesture = GESTURE_TAP;
            }
        }

        // Report the hold once, the moment the threshold is crossed so the
        // user gets feedback without having to release first.
        if (f.contact && !f.holdFired && (now - f.closedAt) >= HOLD_MS) {
            f.holdFired = true;
            gesture = GESTURE_HOLD;
        }

        state.contact[i] = f.contact;
        state.gesture[i] = gesture;
    }

    return state;
}
