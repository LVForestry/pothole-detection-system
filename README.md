# RoadScanner

RoadScanner is an embedded system based on Arduino Giga R1 WiFi for real-time monitoring of road sensors (L1 to L8, R1 to R8). It uses BLE to communicate with Nano 33 BLE Sense sensors, GNSS for geolocation, and an SD card for logging alarms. Ideal for road or agricultural monitoring.

## Features

- **Alarm Monitoring**: Automatic detection of alarms on 16 sensors (L1-L8, R1-R8) with SD logging of data (date, time, lat/lon, speed, direction).
- **BLE Communication**: Central connection to BLE peripherals (Nano 33 BLE Sense) to read distances and calibrate thresholds.
- **Integrated GNSS**: GPS positioning with u-blox Zed F9P for precise logs (outdoor use).
- **SD Logging**: Saves alarms in short files (e.g., 271025.txt) for FAT16/FAT32 compatibility.
- **Serial Interface**: User commands for BLE connection, reading, and calibration.

## Required Hardware

- Arduino Giga R1 WiFi
- u-blox Zed F9P GNSS module (connected via I2C: SDA D20, SCL D21, 3.3V, GND)
- SD card (FAT32 formatted, connected via SPI: CS D10, MOSI D11, MISO D12, SCK D13)
- Nano 33 BLE Sense sensors (BLE peripherals)
- Cables for alarms (16 digital pins: D53, D51, D49, D47, D45, D43, D41, D39, D37, D35, D33, D31, D29, D27, D25, D23)

## Required Software

- Arduino IDE 2.x
- Libraries:
  - ArduinoBLE
  - SparkFun u-blox GNSS Arduino Library
  - NanoBLEFlashPrefs (for Nano)
  - TFLI2C (for Nano)

## Installation

1. Clone this repo: `git clone https://github.com/LVForestry/pothole-detection-system.git`
2. Open the sketches in Arduino IDE:
   - `RSngGigav4.ino` for Giga R1 (central)
   - `RSngNanoV4.ino` for Nano (peripheral)
3. Install libraries via Library Manager.
4. Flash the codes onto the boards.
5. Connect hardware as described.

## Usage

### Startup
- Power the Giga R1.
- Open Serial Monitor (115200 baud).
- System initializes GNSS, SD, and BLE.
- Indoors: "No GPS fix after 10s" (normal).
- Outdoors: GPS fix obtained.

### BLE Connection
- Type "L1" in serial to connect to sensor L1.
- Once connected:
  - "READ": Reads distance.
  - "CALxxx": Calibrates threshold (e.g., CAL1000 for 1000mm).

### Alarms
- Alarms trigger on LOW on pins.
- Automatic log: DDMMYY.txt file with GPS data.

### Debugging
- Serial shows: GNSS connection, SD OK, alarms.
- No alarms at init due to inhibition.

## Code Structure

- `RSngGigav6.ino`: Main loop (BLE central, alarms).
- `BLEManager.cpp/h`: BLE management.
- `AlarmeManager.cpp/h`: Alarm monitoring and SD logging.
- `RSngNanoV4.ino`: BLE peripheral with TFLuna for distance.

## SD Logs Example

```
L2,27-10-2025,14:30:00,45.1234567,-75.9876543,50.00,180.00
```

## Contribution

Issues and PRs welcome! For bugs or features, open an issue.

## License

MIT License - see LICENSE.

## Author

LVForestry
