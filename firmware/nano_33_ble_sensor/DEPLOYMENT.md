# Deployment Guide for Sensor Nodes

## Overview
This guide explains how to deploy the firmware to all 16 Arduino Nano 33 BLE sensor nodes in the pothole detection system.

## Sensor Node Mapping

### Right Side (R1-R8)
| Sensor | BLE Name | Giga Pin | Position |
|--------|----------|----------|----------|
| R1 | NanoTFluna-R1 | 23 | Rightmost |
| R2 | NanoTFluna-R2 | 25 | |
| R3 | NanoTFluna-R3 | 27 | |
| R4 | NanoTFluna-R4 | 29 | |
| R5 | NanoTFluna-R5 | 31 | |
| R6 | NanoTFluna-R6 | 33 | |
| R7 | NanoTFluna-R7 | 35 | |
| R8 | NanoTFluna-R8 | 37 | Inner right |

### Left Side (L8-L1)
| Sensor | BLE Name | Giga Pin | Position |
|--------|----------|----------|----------|
| L8 | NanoTFluna-L8 | 39 | Inner left |
| L7 | NanoTFluna-L7 | 41 | |
| L6 | NanoTFluna-L6 | 43 | |
| L5 | NanoTFluna-L5 | 45 | |
| L4 | NanoTFluna-L4 | 47 | |
| L3 | NanoTFluna-L3 | 49 | |
| L2 | NanoTFluna-L2 | 51 | |
| L1 | NanoTFluna-L1 | 53 | Leftmost |

## Deployment Procedure

### Prerequisites
1. Arduino IDE installed with all required libraries
2. Arduino Mbed OS Nano Boards support installed
3. USB cable for connecting Nano 33 BLE
4. 16 Arduino Nano 33 BLE boards
5. 16 TFLuna LIDAR sensors
6. Wiring for I2C connections

### Step-by-Step Deployment

#### For Each Sensor Node (Repeat 16 times)

1. **Open the Firmware**
   - Open `nano_33_ble_sensor.ino` in Arduino IDE

2. **Configure Sensor Position**
   - Locate the line: `#define SENSOR_POSITION "R1"`
   - Change it to the appropriate sensor position (R1-R8 or L1-L8)
   - Example for R1: `#define SENSOR_POSITION "R1"`
   - Example for L8: `#define SENSOR_POSITION "L8"`

3. **Select Board and Port**
   - Connect Arduino Nano 33 BLE via USB
   - Tools → Board → Arduino Mbed OS Nano Boards → Arduino Nano 33 BLE
   - Tools → Port → (select the appropriate port)

4. **Upload Firmware**
   - Click Upload button (→) or Sketch → Upload
   - Wait for upload to complete
   - Look for "Done uploading" message

5. **Verify Deployment**
   - Open Serial Monitor (Tools → Serial Monitor)
   - Set baud rate to 115200
   - Look for initialization messages:
     ```
     === Arduino Nano 33 BLE Sensor Node ===
     Sensor Position: [POSITION]
     Configuration loaded from flash:
       Zero Reference: 0 cm
       Threshold: 5 cm
     BLE initialized. Device name: NanoTFluna-[POSITION]
     Advertising started.
     Setup complete. Starting main loop...
     ```

6. **Test BLE Advertising**
   - Use a BLE scanner app on your smartphone (e.g., nRF Connect)
   - Scan for devices
   - Verify device appears as `NanoTFluna-[POSITION]`

7. **Label the Board**
   - Use a label maker or marker
   - Clearly mark the board with its position (R1-R8 or L1-L8)
   - This prevents confusion during installation

8. **Disconnect and Move to Next Board**
   - Disconnect USB
   - Set aside the programmed board
   - Connect next board and repeat from step 1

### Batch Configuration Script

For efficiency, you can create a script to automate the position change. Here's a Python script example:

```python
#!/usr/bin/env python3
import sys

positions = ["R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8",
             "L8", "L7", "L6", "L5", "L4", "L3", "L2", "L1"]

def update_position(position):
    with open('nano_33_ble_sensor.ino', 'r') as f:
        content = f.read()
    
    # Replace the SENSOR_POSITION line
    lines = content.split('\n')
    for i, line in enumerate(lines):
        if line.strip().startswith('#define SENSOR_POSITION'):
            lines[i] = f'#define SENSOR_POSITION "{position}"'
            break
    
    with open('nano_33_ble_sensor.ino', 'w') as f:
        f.write('\n'.join(lines))
    
    print(f"Updated SENSOR_POSITION to {position}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <POSITION>")
        print(f"Valid positions: {', '.join(positions)}")
        sys.exit(1)
    
    position = sys.argv[1].upper()
    if position not in positions:
        print(f"Error: Invalid position '{position}'")
        print(f"Valid positions: {', '.join(positions)}")
        sys.exit(1)
    
    update_position(position)
```

Usage:
```bash
python3 update_position.py R1
# Upload firmware
python3 update_position.py R2
# Upload firmware
# ... and so on
```

## Hardware Assembly

### For Each Sensor Node

1. **Connect TFLuna to Arduino**
   - TFLuna VCC → Arduino 5V
   - TFLuna GND → Arduino GND
   - TFLuna SDA → Arduino SDA
   - TFLuna SCL → Arduino SCL

2. **Connect Output to Giga**
   - Arduino Pin 2 → Giga R1 corresponding input pin (see mapping table)
   - Arduino GND → Giga R1 GND (shared ground)

3. **Power Supply**
   - Connect appropriate power supply to Arduino
   - Ensure stable 5V supply for reliable operation

4. **Mounting**
   - Mount securely in designated position
   - Ensure LIDAR has clear line of sight to road surface
   - Protect from weather (if outdoor deployment)

## Initial Calibration

After all sensors are deployed:

1. **Connect to Each Sensor via BLE**
   - Use central controller or smartphone app
   - Connect to each `NanoTFluna-[POSITION]` device

2. **Set Zero Reference**
   - Position vehicle/platform on level road surface (no pothole)
   - Read current distance from sensor
   - Write this value to Zero Reference characteristic
   - Value is stored in flash memory

3. **Adjust Threshold (if needed)**
   - Default is 5cm
   - Write new threshold value to Threshold characteristic
   - Recommended: 5-10cm depending on road conditions

4. **Test Detection**
   - Drive over a known pothole or test depression
   - Verify output pin goes HIGH for 2 seconds
   - Check Giga R1 receives signal on corresponding input pin

## Troubleshooting During Deployment

### Compilation Errors
- Ensure all required libraries are installed
- Check Arduino Mbed OS Nano Boards package is installed
- Verify board is selected as "Arduino Nano 33 BLE"

### Upload Errors
- Press reset button twice quickly to enter bootloader mode
- Check USB cable connection
- Try different USB port
- Verify correct port is selected

### No Serial Output
- Check Serial Monitor baud rate is 115200
- Press reset button on Arduino
- Check USB connection

### BLE Not Advertising
- Verify ArduinoBLE library is installed correctly
- Check antenna is not damaged
- Try resetting the board
- Check for error messages in Serial Monitor

### TFLuna Not Reading
- Verify I2C connections (SDA, SCL)
- Check power supply (5V, GND)
- Use I2C scanner to verify device address (should be 0x10)
- Check Serial Monitor for error messages

## Verification Checklist

Before final deployment, verify each sensor:

- [ ] Firmware uploaded successfully
- [ ] Correct sensor position configured
- [ ] BLE name matches expected format
- [ ] Serial output shows initialization
- [ ] BLE device visible in scan
- [ ] TFLuna reading distance values
- [ ] Output pin responds to test (if testable)
- [ ] Board labeled with position
- [ ] Hardware connections secure

## Maintenance

### Regular Checks
- Monitor for communication errors via Giga R1
- Check BLE connectivity periodically
- Verify distance readings are reasonable
- Clean LIDAR sensor lenses as needed

### Firmware Updates
- Can be done over USB (requires physical access)
- Configuration values persist in flash (no recalibration needed)
- Update one sensor at a time to minimize system downtime

### Configuration Backups
- Read and record configuration values from each sensor
- Store in central database or configuration file
- Enables quick restoration if sensor needs replacement

## Safety Considerations

- Disconnect power before making hardware connections
- Use appropriate voltage levels (5V for Arduino and TFLuna)
- Ensure proper grounding to prevent damage
- Protect electronics from moisture and extreme temperatures
- Follow electrical safety guidelines during deployment

## Support

For deployment issues:
1. Check Serial Monitor output for error messages
2. Verify hardware connections
3. Review this guide and main README.md
4. Check BLE scanner to verify advertising
5. Test I2C communication separately if needed

## Post-Deployment Testing

1. **System Integration Test**
   - Power on all 16 sensors
   - Verify Giga R1 receives signals from all sensors
   - Test BLE connectivity from Giga R1 to each sensor

2. **Functional Test**
   - Drive test vehicle over known features
   - Verify all sensors detect appropriately
   - Check timing of output signals (2-second duration)

3. **Communication Test**
   - Poll each sensor for distance via BLE
   - Update configuration on several sensors
   - Verify persistence after power cycle

4. **Load Test**
   - Operate system continuously for extended period
   - Monitor for communication failures
   - Check for memory leaks or performance degradation
