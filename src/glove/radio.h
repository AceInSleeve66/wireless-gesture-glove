/**
 * Wireless Gesture Glove - radio module (glove).
 *
 * Transmits GesturePackets to the dongle over the nRF24L01 link.
 * See protocol.h for the packet layout and the shared RF settings.
 */
#pragma once

#include <protocol.h>

// Starts the nRF24L01 as a transmitter. Returns false if it is not detected.
bool radioBegin();

// Transmits one packet. Returns false if the dongle did not acknowledge it.
bool radioSend(const GesturePacket& packet);
