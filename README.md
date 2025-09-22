# A02YYUW Water Level Sensor – Arduino + MQTT

## A02YYUW-MQTT **(A021AT)**

## Overview

Modular Arduino implementation for reading A02YYUW ultrasonic sensor data and publishing water level to MQTT. Designed for integration with Home Assistant or any MQTT consumer.

## Sensor Model

This project uses the **A021AT Manual UART version** of the A02YYUW ultrasonic sensor.

- **Model number:** A021AT
- **Do not use the PWM version** — it outputs pulse-width signals, not serial data.
- **Do not use the auto-UART version** — this setup expects manual serial polling.
- All four wires are connected: `VCC`, `GND`, `TX`, and `RX`.
- Sensor communicates at 9600 baud and must be polled manually.

## Folder Structure

a02yyuw-water-level/
 ├── components/
 │ ├── a02yyuw_sensor.cpp/h                 // UART parsing and distance conversion
 │ ├── water_level.cpp/h                    // Fill percentage and threshold logic
 │ ├── mqtt_bridge.cpp/h                    // MQTT publishing 
 ├── main.ino                               // Setup and loop
 ├── README.md
 └── LICENSE

## Pinout

| ESP32 Pin | A02YYUW Wire | Function        |
|-----------|--------------|-----------------|
| `RX2`     | `TX`         | Sensor data out |
| `TX2`     | `RX`         | Manual polling  |
| `GND`     | `GND`        | Ground          |
| `VIN`     | `VCC`        | 5V power        |

- Sensor must be polled via `TX2` to trigger a response
- Response is read on `RX2` as a 4-byte packet: `0xFF 0xXX 0xYY checksum`
- Use level shifter if ESP32 is 3.3V-only and sensor outputs 5V

## Setup

1. Connect A02YYUW sensor to ESP32 UART (e.g. RX2/TX2 eg.GPIO16/GPIO17)
2. Flash `main.ino` using Arduino IDE or PlatformIO
3. Configure MQTT broker credentials in `mqtt_bridge.cpp`
4. Adjust tank depth constants in `water_level.cpp`
5. Monitor published topics:
   - `nft/water_level` → percentage
   - `nft/water_state` → "low", "medium", "full"

## Notes

- Sensor output parsed as 0xFF 0xXX 0xYY checksum
- Fill level mapped linearly from min/max depth
- MQTT reconnect and publish handled in loop
- No BLE, no dashboard — just raw data and MQTT

## License

MIT

