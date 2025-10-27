# Arduino Nano 33 BLE Sensor Node Firmware

## Overview
This firmware is designed for the Arduino Nano 33 BLE board to serve as a sensor node in the pothole detection system. Each sensor node combines a TFLuna LIDAR sensor with BLE communication capabilities.

## Features

### 1. LIDAR Distance Measurement
- Communicates with TFLuna LIDAR sensor via I2C
- Measurement rate: 10Hz (configurable)
- I2C address: 0x10
- I2C speed: 400kHz

### 2. Pothole Detection
- Detects potholes deeper than a configurable threshold (default: 5cm)
- Compares current distance to a zero reference point
- Detection logic: `depth = currentDistance - zeroReference`
- Pothole detected when `depth > threshold`

### 3. Digital Output Signal
- Outputs HIGH on pin 2 when pothole is detected
- Signal duration: 2 seconds (configurable via `OUTPUT_DURATION_MS`)
- Automatically resets to LOW after duration

### 4. BLE Communication
The firmware provides a BLE service with the following characteristics:

| Characteristic | UUID | Properties | Description |
|---------------|------|------------|-------------|
| Distance | `12340001-...` | Read, Notify | Current distance in cm (real-time) |
| Zero Reference | `12340002-...` | Read, Write | Zero reference point in cm |
| Threshold | `12340003-...` | Read, Write | Detection threshold in cm |
| Status | `12340004-...` | Read | Status messages |

### 5. Flash Memory Storage
- Stores zero reference and threshold values in flash memory
- Configuration persists across power cycles
- Automatic initialization with default values on first boot

### 6. Unique BLE Naming
- Each sensor advertises with a unique BLE name
- Format: `NanoTFluna-{POSITION}`
- Examples: `NanoTFluna-R1`, `NanoTFluna-R2`, ..., `NanoTFluna-L8`

## Hardware Requirements

### Components
- Arduino Nano 33 BLE board
- TFLuna LIDAR sensor
- Connection wires for I2C

### Pin Connections

#### TFLuna LIDAR (I2C)
- SDA: Connect to Arduino SDA pin
- SCL: Connect to Arduino SCL pin
- VCC: 5V
- GND: GND

#### Output Signal
- Pin 2: Digital output (connects to Giga R1 input)

## Software Requirements

### Arduino IDE
- Version 1.8.13 or later (or Arduino IDE 2.x)

### Required Libraries
Install these libraries via Arduino Library Manager:

1. **ArduinoBLE** (by Arduino)
   - For BLE communication
   - Version 1.3.0 or later

2. **FlashStorage** (by Arduino)
   - For storing configuration in flash memory
   - Version 1.0.0 or later

3. **Wire** (included with Arduino)
   - For I2C communication

### Board Support
Install Arduino Mbed OS Nano Boards support:
1. Open Arduino IDE
2. Go to Tools → Board → Boards Manager
3. Search for "Arduino Mbed OS Nano Boards"
4. Install the package

## Configuration

### Setting Sensor Position
Before uploading the firmware to each sensor, modify the `SENSOR_POSITION` definition:

```cpp
// In nano_33_ble_sensor.ino
#define SENSOR_POSITION "R1"  // Change to R1-R8 or L1-L8
```

### Configurable Parameters
You can modify these constants in the code:

```cpp
// Detection Configuration
#define DEFAULT_ZERO_REFERENCE 0      // Default zero reference in cm
#define DEFAULT_THRESHOLD_CM 5         // Default detection threshold in cm

// Pin Configuration
#define OUTPUT_PIN 2                   // Digital output pin
#define OUTPUT_DURATION_MS 2000        // Output HIGH duration in milliseconds

// Measurement Configuration
const unsigned long MEASUREMENT_INTERVAL_MS = 100;  // 10Hz measurement rate
```

## Installation

### 1. Install Prerequisites
1. Install Arduino IDE
2. Install required libraries (see Software Requirements)
3. Install board support for Arduino Nano 33 BLE

### 2. Configure the Firmware
1. Open `nano_33_ble_sensor.ino` in Arduino IDE
2. Set the `SENSOR_POSITION` for the specific sensor node
3. Optionally adjust other configuration parameters

### 3. Upload to Board
1. Connect Arduino Nano 33 BLE via USB
2. Select Board: Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
3. Select Port: Tools → Port → (select the appropriate COM/tty port)
4. Click Upload button (or Sketch → Upload)

### 4. Verify Operation
1. Open Serial Monitor (Tools → Serial Monitor)
2. Set baud rate to 115200
3. Check for initialization messages
4. Verify BLE advertising is active

## Usage

### Initial Setup
1. Power on the sensor node
2. The device will load configuration from flash or use defaults
3. BLE advertising starts automatically
4. Device is ready for measurements and BLE connections

### Via BLE (from Central Controller)
1. Scan for BLE devices with name `NanoTFluna-{POSITION}`
2. Connect to the device
3. Read distance characteristic for real-time measurements
4. Write to zero reference characteristic to set calibration point
5. Write to threshold characteristic to adjust detection sensitivity

### Via Serial Monitor
Open Serial Monitor (115200 baud) to view:
- Initialization messages
- Configuration values
- Real-time distance readings
- Pothole detection events
- BLE connection status
- Configuration changes

## Operation Modes

### Normal Operation
1. Continuously measures distance from TFLuna sensor at 10Hz
2. Compares distance to zero reference and threshold
3. Triggers output pin HIGH for 2 seconds when pothole detected
4. Updates BLE distance characteristic with each measurement
5. Responds to BLE read/write requests

### Configuration Mode (via BLE)
1. Central controller connects via BLE
2. Can read current configuration values
3. Can write new zero reference value (saved to flash)
4. Can write new threshold value (saved to flash)
5. Configuration persists across power cycles

## Troubleshooting

### TFLuna Not Responding
- Check I2C connections (SDA, SCL)
- Verify I2C address (default: 0x10)
- Check power supply (5V, GND)
- View Serial Monitor for error messages

### BLE Not Advertising
- Ensure ArduinoBLE library is installed
- Check Serial Monitor for BLE initialization errors
- Verify Arduino Nano 33 BLE board is selected in IDE
- Try resetting the board

### Output Pin Not Triggering
- Verify pothole detection is occurring (check Serial Monitor)
- Check if threshold and zero reference are properly configured
- Verify wiring to pin 2
- Test with multimeter or LED

### Configuration Not Saving
- Ensure FlashStorage library is installed
- Check Serial Monitor for save confirmation messages
- Verify sufficient flash memory is available

## Technical Details

### BLE Service Structure
```
Service UUID: 12340000-1234-5678-9abc-def012345678
├── Distance Characteristic (12340001-...)
│   ├── Properties: Read, Notify
│   └── Type: Int16 (cm)
├── Zero Reference Characteristic (12340002-...)
│   ├── Properties: Read, Write
│   └── Type: Int16 (cm)
├── Threshold Characteristic (12340003-...)
│   ├── Properties: Read, Write
│   └── Type: Int16 (cm)
└── Status Characteristic (12340004-...)
    ├── Properties: Read
    └── Type: String (64 bytes max)
```

### Flash Storage Structure
```cpp
struct SensorConfig {
  int16_t zeroReference;  // Zero reference distance in cm
  int16_t thresholdCm;    // Pothole detection threshold in cm
  bool valid;             // Validation flag
};
```

### Detection Algorithm
1. Read distance from TFLuna sensor
2. Calculate depth: `depth = currentDistance - zeroReference`
3. Compare: if `depth > threshold`, pothole detected
4. On detection:
   - Set output pin HIGH
   - Log event to serial
   - Maintain HIGH for 2 seconds
   - Reset to LOW automatically

## Performance Characteristics

- **Measurement Rate**: 10Hz (100ms interval)
- **I2C Speed**: 400kHz
- **BLE Update Rate**: Per measurement (10Hz)
- **Detection Latency**: <100ms
- **Output Signal Duration**: 2000ms
- **Flash Write Cycles**: ~10,000 (typical for configuration updates)

## License
Refer to the main repository LICENSE file.

## Support
For issues or questions, please refer to the main project repository.
