# A0221AT ESPHome Integration

![Version](https://img.shields.io/badge/version-v1.0.30-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-green)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

Trigger-based UART integration for the A0221AT ultrasonic sensor (A02YYUW Manual UART). Designed for ESPHome external component use with full Home Assistant telemetry.

---

## 📖 Overview

This integration sends a manual UART trigger to the A0221AT sensor and parses its binary response (`0xFF HH LL`) into distance readings. It supports:

- Raw depth in centimetres
- Fill percentage conversion
- Optional binary fill state logic

---

## 🔍 Sensor Model

- **Model:** A0221AT (Manual UART version of A02YYUW)
- **Baud rate:** 9600
- **Wiring:** VCC, GND, TX, RX
- **Trigger:** Manual polling required (`R\r\n`)
- ⚠️ Rated for 5 V, but works reliably at 3.3 V on ESP32

Avoid:
- PWM version (outputs pulse-width, not serial)
- Auto-UART version (auto-transmits, not compatible)

---

## 🖼️ Hardware

![A0221AT Sensor Module (ESPBoards)](https://www.espboards.dev/img/GZGsogluph-1000.avif)  
![A02YYUW Ultrasonic Sensor](https://m.media-amazon.com/images/I/61TlvhztvKL._AC_SL1500_.jpg)

---

## 📦 ESPHome Installation

Add this to your ESPHome YAML:

```yaml
external_components:
  - source: github://flamedodger/a0221at@main
    components: [a0221at]

uart:
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

sensor:
  - platform: a0221at
    name: "Fill Level"
    id: tank_level_cm
    unit_of_measurement: "cm"
    accuracy_decimals: 1
    update_interval: 1s

  - platform: template
    name: "Fill Level %"
    unit_of_measurement: "%"
    accuracy_decimals: 1
    lambda: |-
      float depth = 20.0;
      float cm = id(tank_level_cm).state;
      if (isnan(cm)) return NAN;
      if (cm > depth) cm = depth;
      if (cm < 0) cm = 0;
      return (cm / depth) * 100.0;
    update_interval: 1s
```
---

## ⚙️ Pinout

| ESP32 Pin | A02YYUW Wire | Function        |
|-----------|--------------|-----------------|
| `RX2`     | `TX`         | Sensor data out |
| `TX2`     | `RX`         | Manual polling  |
| `GND`     | `GND`        | Ground          |
| `VIN`     | `VCC`        | 3.3V power        |

- Sensor must be polled via `TX2` to trigger a response
- Response is read on `RX2` as a 4-byte packet: `0xFF 0xXX 0xYY checksum`

## ⚙️ Setup

Connect A0221AT (A02YYUW) ultrasonic sensor to ESP32 UART:

    RX → GPIO16

    TX → GPIO17

    VCC → 3.3 V

    GND → GND (Sensor operates reliably at 3.3 V for short-range detection up to ~350 cm)

Flash tank-level.yaml using ESPHome via Home Assistant or CLI

Define tank depth constant in template sensor logic (e.g. depth = 20.0)

Expose two sensors:

    sensor.tank_level_cm → raw depth in centimetres

    sensor.tank_level_percent → converted fill percentage (0–100%)

Optional binary sensor for fill state logic:

    "low" → < 25%

    "medium" → 25–75%

    "full" → > 75%

📂 Folder Structure

a0221at/
├── __init__.py
├── sensor.py
├── const.py
├── uart_handler.py
├── README.md


## Notes

    Sensor requires a UART trigger ("R\r\n") before responding

    Response format is binary: 0xFF HH LL → distance in millimetres

    Parsed value is published as centimetres with 1 decimal precision

    Works reliably at 3.3 V on ESP32, despite 5 V spec — monitor for range or cold-boot issues

    No external logic shifter required when using native ESP32 UART

# A02YUUW/a0221at
