/**
 * Wireless Gesture Glove - HID module (dongle).
 *
 * Wraps the ATmega32U4's native USB HID mouse: cursor movement and the
 * left/right buttons. The dongle enumerates as a standard mouse so the
 * host PC needs no drivers.
 */
#pragma once

// Starts the USB HID mouse interface. Call once from setup().
void hidBegin();

// Moves the cursor by a relative offset in pixels.
void hidMove(int dx, int dy);

// Holds or releases the left mouse button.
void hidLeftButton(bool pressed);

// Holds or releases the right mouse button.
void hidRightButton(bool pressed);
