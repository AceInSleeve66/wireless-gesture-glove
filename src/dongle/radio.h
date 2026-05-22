/**
 * Wireless Gesture Glove - radio module (dongle).
 *
 * Receives GesturePackets from the glove over the nRF24L01 link.
 * See protocol.h for the packet layout and the shared RF settings.
 */
#pragma once

#include <protocol.h>

// Starts the nRF24L01 as a receiver. Returns false if it is not detected.
bool radioBegin();

// Reads the next packet into `packet`. Returns false if none has arrived.
bool radioReceive(GesturePacket& packet);
