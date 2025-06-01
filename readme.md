# Inside IoT: RGB LED Controller with Blynk (ESP32-S3DEVKITC-1)

## Project Overview

This project implements a non-blocking architecture for the ESP32-S3DEVKITC-1_V1.1 using Arduino, with remote control via the **Blynk IoT** platform. It supports customizable RGB LED blinking, persistent configuration, and a flexible logging system.

### Key Features:
- **Blynk Integration**: Control LED behavior from your smartphone via a custom Blynk UI.
- **Non-Blocking Architecture**: Uses timers and state machines for asynchronous execution.
- **Persistent Configuration**: EEPROM/NVS stores blink parameters across reboots.
- **Logging Options**: Choose between plain messages, JSON, or Arduino Plotter output.
- **Demo/Test Modes**: Switch between real-world and simulated behaviors.

---

## Blynk Mobile UI – "Inside IoT"

<img src="blynk-led-iot.jpg" alt="Blynk App Screenshot" width="300">

| Widget                | Type            | Virtual Pin | Range / Values              | Description                                 |
|-----------------------|------------------|--------------|------------------------------|---------------------------------------------|
| Color selector         | Dropdown          | V1           | azul, rojo, verde, etc.      | Sets LED color (RGB values mapped)          |
| Blink quantity         | Slider            | V2           | 1–10                         | Number of times the LED should blink        |
| Blink time             | Slider            | V3           | 0.1–5 sec                    | Time between blinks                         |
| Start experiment       | Button            | V4           | ON / OFF                     | Begins blinking cycle                       |
| Demo mode              | Switch            | V5           | ON / OFF                     | Enables alternate behavior or data sets     |
| Show configuration     | Button/Switch     | V6           | ON / OFF                     | Toggles config/log display                  |
| Logging level          | Segmented Switch  | V7           | disable / msj / json / plotter | Sets verbosity of serial output         |
| Firmware version       | Text              | —            | v2.0.0                       | Displayed on UI, not tied to code logic     |

---

## Why Blynk Logic Was Not Modularized

Due to structural limitations in Blynk's Arduino library:

- Callbacks like `BLYNK_WRITE(Vx)` are tied to the global execution context.
- Separating logic into modules would require artificial layers, reducing clarity and introducing unnecessary overhead.

References:
- [GitHub Issue: Multiple Definitions in Blynk](https://github.com/blynkkk/blynk-library/issues/312)
- [Community Post: Blynk modularity limitations](https://community.blynk.cc/t/blynk-multiple-definitions/31450)

---

## Project Structure

| File                  | Description                                 |
|-----------------------|---------------------------------------------|
| `led.h / led.cpp`     | Handles RGB LED control logic               |
| `cfg_iot.h / .cpp`    | Configuration storage in EEPROM/NVS         |
| `log.h`               | Logging utilities for various output modes  |
| `timer.h`             | Custom non-blocking timers                  |
| `precompilation.h`    | Build-time constants and macros             |
| `.ino` file           | Blynk setup, Wi-Fi, and state coordination  |

---

## Usage

1. Open the project in **Arduino IDE 2.3.6 or later**.
2. Set your Blynk credentials in the `.ino`:
   ```cpp
   #define BLYNK_TEMPLATE_ID   "..."
   #define BLYNK_TEMPLATE_NAME "Inside IoT"
   #define BLYNK_AUTH_TOKEN    "..."

