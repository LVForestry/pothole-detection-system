# Firmware Installation Guide

This guide covers the installation and setup of the Arduino Giga R1 Central Controller firmware for the Pothole Detection System.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Hardware Setup](#hardware-setup)
3. [Software Setup](#software-setup)
4. [Compilation and Upload](#compilation-and-upload)
5. [Initial Configuration](#initial-configuration)
6. [Testing](#testing)
7. [Troubleshooting](#troubleshooting)

## Prerequisites

### Hardware Requirements

- **Arduino Giga R1 WiFi** board
- USB-C cable for programming
- 16 sensor nodes (Arduino Nano 33 BLE with TFLuna LIDAR)
- Jumper wires for connections
- Power supply (appropriate for your deployment)

### Software Requirements

Choose one of the following development environments:

#### Option 1: Arduino IDE (Recommended for beginners)
- Arduino IDE 2.0 or higher
- Download from: https://www.arduino.cc/en/software

#### Option 2: PlatformIO (Recommended for advanced users)
- Visual Studio Code
- PlatformIO IDE extension
- Download VS Code from: https://code.visualstudio.com/
- Install PlatformIO: https://platformio.org/install/ide?install=vscode

## Hardware Setup

### Pin Connections

Connect the digital output pins from each sensor node to the corresponding input pins on the Giga R1:

| Sensor | Nano Pin | Giga R1 Pin |
|--------|----------|-------------|
| R1 | D2 | 23 |
| R2 | D2 | 25 |
| R3 | D2 | 27 |
| R4 | D2 | 29 |
| R5 | D2 | 31 |
| R6 | D2 | 33 |
| R7 | D2 | 35 |
| R8 | D2 | 37 |
| L8 | D2 | 39 |
| L7 | D2 | 41 |
| L6 | D2 | 43 |
| L5 | D2 | 45 |
| L4 | D2 | 47 |
| L3 | D2 | 49 |
| L2 | D2 | 51 |
| L1 | D2 | 53 |

### Wiring Notes

1. Connect all sensor node grounds to the Giga R1 ground
2. Ensure proper power distribution to all sensor nodes
3. Keep BLE antenna areas clear for optimal wireless communication
4. Use appropriate wire gauge for current requirements

## Software Setup

### Option 1: Using Arduino IDE

#### Step 1: Install Board Support

1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/arduino/ArduinoCore-mbed/main/package_mbed_index.json
   ```
4. Go to **Tools** → **Board** → **Boards Manager**
5. Search for "Arduino Mbed OS Giga Boards"
6. Install the latest version

#### Step 2: Install Required Libraries

1. Go to **Tools** → **Manage Libraries** (or Ctrl+Shift+I)
2. Search for and install:
   - **ArduinoBLE** (version 1.3.6 or higher)

The WiFi library is included with the board package.

#### Step 3: Open the Firmware

1. Navigate to `firmware/giga_r1_central_controller/`
2. Open `giga_r1_central_controller.ino`

#### Step 4: Configure Board Settings

1. Go to **Tools** → **Board**
2. Select **Arduino Mbed OS Giga Boards** → **Arduino Giga R1**
3. Set the following options:
   - **Port**: Select the port where Giga R1 is connected
   - **Optimize**: Default (or choose based on your needs)

### Option 2: Using PlatformIO

#### Step 1: Open Project

1. Open Visual Studio Code
2. Install PlatformIO IDE extension if not already installed
3. Go to **PlatformIO Home** → **Open Project**
4. Navigate to `firmware/giga_r1_central_controller/`
5. Open the folder

#### Step 2: Build Configuration

The `platformio.ini` file is already configured with:
- Board: Arduino Giga R1 WiFi
- Framework: Arduino
- Libraries: ArduinoBLE
- Serial monitor speed: 115200

No additional configuration is needed.

## Compilation and Upload

### Using Arduino IDE

1. Click the **Verify** button (✓) to compile the code
2. Check for any compilation errors in the output window
3. If successful, connect the Giga R1 via USB-C
4. Select the correct port: **Tools** → **Port**
5. Click the **Upload** button (→) to flash the firmware
6. Wait for upload to complete

### Using PlatformIO

1. Click the **Build** button (✓) in the PlatformIO toolbar
   - Or use the command: `pio run`
2. Check for any compilation errors
3. Connect the Giga R1 via USB-C
4. Click the **Upload** button (→) in the PlatformIO toolbar
   - Or use the command: `pio run --target upload`
5. Wait for upload to complete

## Initial Configuration

### Step 1: Verify Installation

1. Open the Serial Monitor:
   - **Arduino IDE**: Tools → Serial Monitor (set to 115200 baud)
   - **PlatformIO**: Click the Serial Monitor icon
2. Press the reset button on the Giga R1
3. You should see initialization messages:
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
   ```

### Step 2: Configure WiFi Settings (Optional)

If you need to change the WiFi AP name or add a password:

1. Open `config.h`
2. Modify these lines:
   ```cpp
   #define WIFI_AP_SSID      "Your-Custom-SSID"
   #define WIFI_AP_PASSWORD  "your-password"  // or "" for open
   ```
3. Recompile and upload

### Step 3: Customize BLE Settings (Optional)

To change BLE UUIDs (must match sensor nodes):

1. Open `config.h`
2. Modify the UUID definitions:
   ```cpp
   #define BLE_SERVICE_UUID       "your-service-uuid"
   #define BLE_DISTANCE_CHAR_UUID "your-distance-uuid"
   // etc.
   ```
3. Recompile and upload

## Testing

### Test 1: Pin Monitoring

1. Connect a jumper wire to one of the sensor input pins (e.g., pin 23)
2. Touch the other end to 3.3V or 5V
3. Check Serial Monitor for detection message:
   ```
   ================================
   POTHOLE DETECTED!
   Sensor: R1 (Pin 23)
   Timestamp: 45230 ms
   ================================
   ```
4. Repeat for other pins to verify all connections

### Test 2: WiFi Access Point

1. On your phone or laptop, scan for WiFi networks
2. Look for "Pothole-Detection-AP" (or your custom SSID)
3. Connect to the network (no password required for default setup)
4. Note the connection success

### Test 3: Web Dashboard

1. Connect to the WiFi AP
2. Open a web browser
3. Navigate to: `http://192.168.4.1/`
4. Verify the dashboard loads and displays all 16 sensors
5. Click "Refresh Status" to update the display
6. Try clicking "Poll Sensor" on a sensor (requires sensor node to be active)

### Test 4: BLE Communication

1. Ensure at least one sensor node is powered on and advertising
2. From the web dashboard, click "Poll Sensor" for that sensor
3. Check if distance reading is returned
4. Try clicking "Configure" to test configuration updates
5. Monitor Serial Monitor for BLE connection logs

## Troubleshooting

### Compilation Errors

**Error**: "WiFi.h: No such file or directory"
- **Solution**: Ensure Arduino Mbed OS Giga Boards package is installed

**Error**: "ArduinoBLE.h: No such file or directory"
- **Solution**: Install ArduinoBLE library via Library Manager

**Error**: "Sketch too big"
- **Solution**: Go to Tools → Optimize → Smallest (-Os)

### Upload Errors

**Error**: "Port not found"
- **Solution**: 
  1. Check USB cable connection
  2. Try a different USB port
  3. Install USB drivers if needed

**Error**: "Upload failed"
- **Solution**:
  1. Double-tap reset button on Giga R1 to enter bootloader mode
  2. Select the bootloader port
  3. Try upload again

### Runtime Issues

**Issue**: WiFi AP not visible
- **Check**: Serial output for initialization errors
- **Try**: Restart the board
- **Verify**: Board has WiFi capability enabled in tools menu

**Issue**: BLE not connecting to sensors
- **Check**: Sensor nodes are powered and advertising
- **Verify**: BLE device names match expected format
- **Try**: Move devices closer together
- **Check**: Serial Monitor for BLE scan results

**Issue**: Dashboard not loading
- **Verify**: Connected to correct WiFi network
- **Try**: Different browser or clear cache
- **Check**: Access the IP shown in Serial Monitor

**Issue**: Pins not detecting HIGH signals
- **Verify**: Proper wiring and connections
- **Test**: Use multimeter to check voltage levels
- **Check**: Pin numbers in code match physical connections

### Getting Help

If you encounter issues not covered here:

1. Check the Serial Monitor output for error messages
2. Review the README.md in the firmware directory
3. Verify all hardware connections
4. Check that sensor nodes are properly configured
5. Consult the project repository issues page

## Next Steps

After successful installation:

1. Deploy sensor nodes in target locations
2. Calibrate zero reference values for each sensor
3. Set appropriate threshold values (default: 5cm)
4. Monitor system operation via web dashboard
5. Review event logs via Serial Monitor

## Maintenance

### Regular Checks

- Monitor Serial output for errors
- Check WiFi AP connectivity
- Verify all sensors are responding to polls
- Test BLE communication periodically

### Firmware Updates

To update the firmware:

1. Download the latest firmware version
2. Open in Arduino IDE or PlatformIO
3. Compile and upload as described above
4. Verify all functionality after update

### Configuration Backup

Important settings are stored in `config.h`. Keep a backup of this file if you've made custom modifications.

## Safety Notes

- Ensure proper power supply ratings
- Avoid exposing electronics to moisture
- Use appropriate enclosures for outdoor deployment
- Follow electrical safety guidelines
- Test thoroughly before production deployment

## License

This firmware is part of the Pothole Detection System project. See the LICENSE file in the repository root for details.
