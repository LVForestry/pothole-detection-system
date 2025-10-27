# Quick Start Guide

## For First-Time Users

This guide will help you get your first sensor node up and running quickly.

## What You Need

### Hardware
- 1x Arduino Nano 33 BLE board
- 1x TFLuna LIDAR sensor
- 1x USB cable (for programming)
- Jumper wires for connections

### Software
- Arduino IDE (1.8.13 or later, or Arduino IDE 2.x)
- USB driver for Arduino (usually auto-installed)

## Step 1: Install Arduino IDE

1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install following the installer instructions
3. Launch Arduino IDE

## Step 2: Install Board Support

1. Open Arduino IDE
2. Go to **Tools** → **Board** → **Boards Manager**
3. Search for "Arduino Mbed OS Nano Boards"
4. Click **Install** (this may take a few minutes)
5. Wait for installation to complete

## Step 3: Install Required Libraries

1. Go to **Tools** → **Manage Libraries**
2. Search for "ArduinoBLE"
   - Install **ArduinoBLE by Arduino**
3. Search for "FlashStorage"
   - Install **FlashStorage by Arduino**

## Step 4: Connect Hardware

### Connect TFLuna to Arduino

| TFLuna Pin | Arduino Pin |
|------------|-------------|
| VCC (Red) | 5V |
| GND (Black) | GND |
| SDA (Green) | SDA |
| SCL (Blue) | SCL |

### Test Setup (Optional)
For testing, you can connect an LED to pin 2 to see the output signal:
- LED positive (long leg) → Pin 2 (through 220Ω resistor)
- LED negative (short leg) → GND

## Step 5: Open and Configure Firmware

### Option A: Use Pre-configured Example (Easiest)

1. Navigate to `examples/R1/` directory
2. Open `nano_33_ble_sensor_R1.ino` in Arduino IDE
3. **Skip to Step 6** (no configuration needed!)

### Option B: Configure Main Sketch

1. Open `nano_33_ble_sensor.ino` in Arduino IDE
2. Find this line near the top:
   ```cpp
   #define SENSOR_POSITION "R1"
   ```
3. Change "R1" to your desired position (R1-R8 or L1-L8)
4. Save the file

## Step 6: Upload Firmware

1. Connect Arduino Nano 33 BLE via USB
2. In Arduino IDE, select board:
   - **Tools** → **Board** → **Arduino Mbed OS Nano Boards** → **Arduino Nano 33 BLE**
3. Select port:
   - **Tools** → **Port** → (select the port with "Arduino Nano 33 BLE" or similar)
   - On Windows: Usually COM3 or higher
   - On Mac: Usually /dev/cu.usbmodem*
   - On Linux: Usually /dev/ttyACM0
4. Click the **Upload** button (→) at the top left
5. Wait for "Done uploading" message

## Step 7: Verify It's Working

1. Open Serial Monitor:
   - **Tools** → **Serial Monitor**
   - Set baud rate to **115200** (bottom right dropdown)

2. You should see output like:
   ```
   === Arduino Nano 33 BLE Sensor Node ===
   Sensor Position: R1
   Configuration loaded from flash:
     Zero Reference: 0 cm
     Threshold: 5 cm
   BLE initialized. Device name: NanoTFluna-R1
   Advertising started.
   Setup complete. Starting main loop...
   ```

3. Test BLE (optional):
   - Install "nRF Connect" app on smartphone
   - Open app and scan for devices
   - Look for "NanoTFluna-R1" (or your position)
   - Device should appear in scan results

## Step 8: Test Distance Reading

1. With Serial Monitor open, you should see periodic updates
2. Move your hand or an object in front of the TFLuna sensor
3. Watch the distance values change in the Serial Monitor
4. Distance is reported in centimeters

## Step 9: Test Pothole Detection

1. Keep Serial Monitor open
2. Place the sensor so it's pointing down at a flat surface
3. The sensor will measure the distance continuously
4. To simulate a pothole:
   - Quickly move the sensor upward (or move the target surface down)
   - If the distance change exceeds 5cm, you'll see:
     ```
     POTHOLE DETECTED! Distance: 25 cm, Depth: 15 cm
     ```
   - Pin 2 will go HIGH for 2 seconds
   - If you connected an LED, it will light up

## Troubleshooting

### "Board not found" or "Port not available"
- Make sure USB cable is connected
- Try different USB cable (some are charge-only)
- Press reset button twice quickly on Arduino
- Check Device Manager (Windows) or `ls /dev/tty*` (Mac/Linux)

### "Library not found" errors
- Go back to Step 3 and install required libraries
- Make sure you installed ArduinoBLE and FlashStorage

### No Serial Output
- Check baud rate is set to 115200
- Press reset button on Arduino
- Close and reopen Serial Monitor

### TFLuna not responding
- Check wiring connections
- Verify TFLuna has power (should have LED indicator)
- Check I2C connections (SDA, SCL)
- Make sure TFLuna is configured for I2C mode (default)

### BLE not advertising
- Arduino Nano 33 BLE has built-in BLE, no external module needed
- Check Serial Monitor for error messages
- Try resetting the board
- Make sure you selected "Arduino Nano 33 BLE" board (not "Arduino Nano")

## Next Steps

### Configure for Production
1. Set appropriate zero reference:
   - Connect via BLE
   - Read current distance on level surface
   - Write this value to zero reference characteristic
   
2. Adjust threshold if needed:
   - Default is 5cm
   - Adjust based on your detection requirements

### Deploy Multiple Sensors
- Follow DEPLOYMENT.md for deploying all 16 sensors
- Each sensor needs a unique position (R1-R8, L1-L8)
- Use examples directory for pre-configured versions

### Connect to Central Controller
- See main project documentation for Giga R1 integration
- Connect pin 2 to Giga R1 input pin (see pin mapping)
- Set up BLE communication for monitoring

## Understanding the Output

### Serial Monitor Messages

```
Current distance: 50 cm          ← Normal distance reading
POTHOLE DETECTED! ...            ← Pothole detected
Output pin reset to LOW          ← Output signal completed
Zero reference updated to: 45 cm ← Config changed via BLE
Threshold updated to: 8 cm       ← Config changed via BLE
```

### BLE Characteristics

| Characteristic | Type | What it does |
|---------------|------|--------------|
| Distance | Read/Notify | Current distance in cm |
| Zero Reference | Read/Write | Calibration reference point |
| Threshold | Read/Write | Detection threshold in cm |
| Status | Read | Status messages |

## Tips for Success

1. **Test indoors first**: Use a table or floor as test surface
2. **Stable mounting**: Ensure sensor is mounted securely
3. **Clean lens**: Keep TFLuna lens clean for accurate readings
4. **Power supply**: Use stable 5V power supply
5. **Label boards**: Mark each board with its position
6. **Test BLE range**: Verify BLE works at expected distance
7. **Backup config**: Record zero reference and threshold values

## Getting Help

- Check README.md for detailed documentation
- Review DEPLOYMENT.md for multi-sensor setup
- Check troubleshooting sections
- Verify hardware connections
- Check Serial Monitor for error messages

## Common Use Cases

### Testing/Development
- Use pre-configured R1 example
- Monitor via Serial Monitor
- Adjust settings via BLE app

### Calibration
- Place on level surface
- Read current distance
- Set as zero reference via BLE

### Production Deployment
- Use examples directory
- Upload to all 16 sensors
- Connect to Giga R1
- Calibrate each sensor

---

**Congratulations!** You now have a working sensor node. Proceed to the full documentation for advanced features and multi-sensor deployment.
