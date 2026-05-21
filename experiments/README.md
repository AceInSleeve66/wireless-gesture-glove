# Experiments

Standalone hardware bring-up sketches. Each one validates a single component in
isolation before that component is integrated into the product firmware.

This folder is its **own PlatformIO project** (see `platformio.ini` here), separate from the product firmware in the repository root. The sketches are reference material - they are not part of the glove/dongle build.

## Build, flash, monitor

From the repository root:

```
pio run -d experiments -e <env> # build
pio run -d experiments -e <env> -t upload # flash the connected board
pio device monitor -b 115200 # serial output
```

## Catalogue

| Environment           | Board    | Validates                                                 |
|-----------------------|----------|-----------------------------------------------------------|
| `connection_esp32`    | ESP32    | nRF24L01 wiring + SPI link - chip detected and responding |
| `connection_leonardo` | Leonardo | nRF24L01 wiring + SPI link - chip detected and responding |
| `rf_esp32`            | ESP32    | nRF24L01 transmitter - sends a test payload every second  |
| `rf_leonardo`         | Leonardo | nRF24L01 receiver - prints received payloads              |
| `imu_esp32`           | ESP32    | BNO055 IMU over I2C - prints orientation (X/Y/Z)          |
| `touch_esp32`         | ESP32    | Finger-touch contacts read via `INPUT_PULLUP`             |

Pair `rf_esp32` (transmitter) with `rf_leonardo` (receiver) to exercise the full
radio link. All sketches log over serial at 115200 baud.
