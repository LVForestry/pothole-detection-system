# Arduino Giga R1 Central Controller Firmware

## Overview

This firmware implements the central controller for the Pothole Detection System. It runs on an Arduino Giga R1 WiFi board and provides:

1. **Digital Pin Monitoring**: Monitors 16 digital input pins for HIGH signals from sensor nodes
2. **Serial Logging**: Logs pothole detection events to the serial monitor
3. **BLE Communication**: Scans, connects to, and communicates with sensor nodes via BLE
4. **WiFi Access Point**: Creates an open WiFi network for dashboard access
5. **Web Dashboard**: Serves a real-time web interface for monitoring and configuration

## Hardware Requirements

- **Arduino Giga R1 WiFi** board
- 16 sensor nodes (Arduino Nano 33 BLE with TFLuna LIDAR sensors)
- Connections from sensor nodes' digital output pins to Giga R1 input pins

## Pin Mapping

The firmware monitors the following digital input pins, mapped to sensor positions:

| Sensor | Position | Pin | BLE Device Name |
|--------|----------|-----|----------------|
| R1 | Right 1 | 23 | NanoTFluna-R1 |
| R2 | Right 2 | 25 | NanoTFluna-R2 |
| R3 | Right 3 | 27 | NanoTFluna-R3 |
| R4 | Right 4 | 29 | NanoTFluna-R4 |
| R5 | Right 5 | 31 | NanoTFluna-R5 |
| R6 | Right 6 | 33 | NanoTFluna-R6 |
| R7 | Right 7 | 35 | NanoTFluna-R7 |
| R8 | Right 8 | 37 | NanoTFluna-R8 |
| L8 | Left 8 | 39 | NanoTFluna-L8 |
| L7 | Left 7 | 41 | NanoTFluna-L7 |
| L6 | Left 6 | 43 | NanoTFluna-L6 |
| L5 | Left 5 | 45 | NanoTFluna-L5 |
| L4 | Left 4 | 47 | NanoTFluna-L4 |
| L3 | Left 3 | 49 | NanoTFluna-L3 |
| L2 | Left 2 | 51 | NanoTFluna-L2 |
| L1 | Left 1 | 53 | NanoTFluna-L1 |

## Required Libraries

Install these libraries through the Arduino IDE Library Manager:

1. **WiFi** (built-in with Arduino Giga R1 WiFi)
2. **ArduinoBLE** (version 1.3.0 or higher)

## Installation

1. Open the Arduino IDE (version 2.0 or higher recommended)
2. Install the Arduino Mbed OS Giga Boards package:
   - Go to **Tools** → **Board** → **Boards Manager**
   - Search for "Arduino Mbed OS Giga Boards"
   - Install the package
3. Install required libraries:
   - Go to **Tools** → **Manage Libraries**
   - Search for and install "ArduinoBLE"
4. Open `giga_r1_central_controller.ino` in Arduino IDE
5. Select **Tools** → **Board** → **Arduino Mbed OS Giga Boards** → **Arduino Giga R1**
6. Select the correct port: **Tools** → **Port**
7. Click **Upload** to flash the firmware

## Usage

### Serial Monitor

1. Open the Serial Monitor (115200 baud)
2. Monitor real-time events as sensors detect potholes
3. View system initialization and connection status

Example output:
```
Arduino Giga R1 Central Controller
Pothole Detection System
================================
Initializing sensor input pins...
Pin 23 (R1) configured as INPUT
...
BLE initialized successfully
WiFi Access Point created successfully
SSID: Pothole-Detection-AP
IP Address: 192.168.4.1
System initialized successfully

================================
POTHOLE DETECTED!
Sensor: R3 (Pin 27)
Timestamp: 45230 ms
================================
```

### Web Dashboard

1. Connect to the WiFi network:
   - **SSID**: `Pothole-Detection-AP`
   - **Password**: None (open network)

2. Open a web browser and navigate to:
   - `http://192.168.4.1/` (default IP for Giga R1 AP mode)

3. Dashboard features:
   - **Real-time Status**: View HIGH/LOW state of all 16 sensors
   - **Auto-refresh**: Page automatically refreshes every 5 seconds
   - **Poll Sensor**: Click "Poll Sensor" to get real-time distance reading via BLE
   - **Configure**: Click "Configure" to update zero reference and threshold values

## API Endpoints

The web server provides the following REST API endpoints:

### GET /
Returns the HTML dashboard interface

### GET /api/sensors
Returns JSON with all sensor states
```json
{
  "sensors": [
    {
      "index": 0,
      "name": "R1",
      "pin": 23,
      "state": false,
      "ble_name": "NanoTFluna-R1"
    },
    ...
  ]
}
```

### GET /api/poll/{index}
Polls a specific sensor via BLE for current distance reading

Parameters:
- `index`: Sensor index (0-15)

Response:
```json
{
  "success": true,
  "name": "R1",
  "ble_device": "NanoTFluna-R1",
  "distance": 125.50,
  "status": "connected",
  "pin_state": "LOW"
}
```

### POST /api/configure/{index}
Configures a sensor's zero reference and threshold values via BLE

Parameters:
- `index`: Sensor index (0-15)

Request body:
```json
{
  "zero": 150.0,
  "threshold": 5.0
}
```

Response:
```json
{
  "success": true,
  "message": "Configuration updated successfully",
  "sensor": "R1",
  "zero": 150.00,
  "threshold": 5.00
}
```

## BLE Communication

The firmware uses the following BLE service and characteristics to communicate with sensor nodes:

- **Service UUID**: `19b10000-e8f2-537e-4f6c-d104768a1214`
- **Distance Characteristic**: `19b10001-e8f2-537e-4f6c-d104768a1214` (Read)
- **Zero Reference Characteristic**: `19b10002-e8f2-537e-4f6c-d104768a1214` (Write)
- **Threshold Characteristic**: `19b10003-e8f2-537e-4f6c-d104768a1214` (Write)

All values are transmitted as 4-byte float values.

## Troubleshooting

### WiFi AP not visible
- Verify the Giga R1 board has WiFi capability enabled
- Check serial output for initialization errors
- Restart the board

### BLE connection fails
- Ensure sensor nodes are powered on and advertising
- Check that BLE device names match expected format
- Verify BLE is initialized (check serial output)
- Try moving devices closer together

### Web dashboard not loading
- Verify you're connected to the `Pothole-Detection-AP` network
- Try accessing `http://192.168.4.1/`
- Check serial monitor for web server errors

### Sensor states not updating
- Verify physical connections to input pins
- Check that sensor nodes are correctly wired
- Use multimeter to verify voltage levels on pins

## System Architecture

```
┌─────────────────────────────────────────┐
│   Arduino Giga R1 Central Controller    │
│                                         │
│  ┌──────────┐  ┌──────────┐  ┌───────┐ │
│  │ Pin Mon. │  │   BLE    │  │  WiFi │ │
│  │ (16 pins)│  │  Client  │  │   AP  │ │
│  └────┬─────┘  └────┬─────┘  └───┬───┘ │
│       │             │             │     │
└───────┼─────────────┼─────────────┼─────┘
        │             │             │
    Digital      BLE Link      Web Access
     Inputs    (Poll/Config)   (Dashboard)
        │             │             │
        ↓             ↓             ↓
   16 Sensor      16 Sensor      User's
     Nodes          Nodes        Device
```

## Future Enhancements

Potential improvements for future versions:

- Data logging to SD card
- Historical event tracking
- Email/SMS notifications
- Cloud connectivity
- Multi-language dashboard
- Advanced analytics and visualization
- MQTT integration for IoT platforms

## License

See LICENSE file in the repository root.

## Support

For issues, questions, or contributions, please refer to the main repository documentation.
