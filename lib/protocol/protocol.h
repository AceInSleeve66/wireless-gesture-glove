/**
 * Wireless Gesture Glove - radio protocol.
 *
 * Single source of truth for the nRF24L01 link: the packet the glove
 * transmits, the packet the dongle receives, and the radio settings both
 * ends must agree on. Included unchanged by the glove and dongle firmware.
 *
 * The nRF24L01 carries at most 32 bytes per payload - GesturePacket must
 * stay within that, enforced by the static_assert below.
 */
#pragma once

#include <stdint.h>

// ---- Radio configuration (nRF24L01) -------------------------------------
// Both devices wire CE/CSN identically and must share channel and address.
// Data rate (1 Mbps) and PA level (LOW) are applied in radio.cpp, which
// owns the RF24 library - that keeps this header dependency-free so it can
// be included before RF24 is ever pulled in.

constexpr uint8_t RF_CE_PIN      = 4;
constexpr uint8_t RF_CSN_PIN     = 5;
constexpr uint8_t RF_CHANNEL     = 76;
constexpr uint8_t RF_ADDRESS[6]  = "00001";
constexpr uint8_t RF_MAX_PAYLOAD = 32;

// ---- Protocol version ---------------------------------------------------
// Bumped whenever GesturePacket's layout changes. The dongle drops packets
// whose version does not match, so mismatched firmware can't move the cursor.

constexpr uint8_t PROTOCOL_VERSION = 1;

// ---- Fingertip contacts (GesturePacket.contacts bitmask) ----------------
// Each fingertip is read with INPUT_PULLUP; its bit is set while that
// fingertip touches the grounded thumb contact.

constexpr uint8_t CONTACT_MIDDLE = 1 << 0;  // middle + thumb - left click
constexpr uint8_t CONTACT_RING   = 1 << 1;  // ring + thumb   - right click
constexpr uint8_t CONTACT_PINKY  = 1 << 2;  // pinky + thumb  - DPI / sleep

// ---- Radio packet: glove -> dongle --------------------------------------
// The glove only senses: it sends raw orientation and contact state. All
// interpretation (calibration, cursor mapping, gestures) happens on the
// dongle.
//
// `packed` keeps the byte layout identical on the ESP32 glove and the
// AVR dongle - without it the ESP32 pads the struct to align float on
// 4 bytes, shifting every field the dongle reads.

struct __attribute__((packed)) GesturePacket {
    uint8_t version;   // == PROTOCOL_VERSION
    uint8_t contacts;  // CONTACT_* bitmask of fingertip contacts
    float   eulerX;    // BNO055 Euler angle, degrees - heading / yaw
    float   eulerY;    // BNO055 Euler angle, degrees - roll
    float   eulerZ;    // BNO055 Euler angle, degrees - pitch
};

static_assert(sizeof(GesturePacket) <= RF_MAX_PAYLOAD,
              "GesturePacket exceeds the nRF24L01 32-byte payload limit");
