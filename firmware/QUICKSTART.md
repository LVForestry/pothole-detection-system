# Quick Start Guide

Get your Pothole Detection System up and running in 15 minutes!

## What You'll Need

- Arduino Giga R1 WiFi board
- USB-C cable
- Computer with Arduino IDE installed
- (Optional) Sensor nodes for full system testing

## Step-by-Step Setup

### 1. Install Arduino IDE (5 minutes)

If you don't have Arduino IDE:

1. Download from https://www.arduino.cc/en/software
2. Install the application
3. Launch Arduino IDE

### 2. Install Board Support (3 minutes)

1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/arduino/ArduinoCore-mbed/main/package_mbed_index.json
   ```
4. Click **OK**
5. Go to **Tools** → **Board** → **Boards Manager**
6. Search for "Arduino Mbed OS Giga"
7. Click **Install** (this may take a few minutes)

### 3. Install Library (2 minutes)

1. Go to **Tools** → **Manage Libraries**
2. Search for "ArduinoBLE"
3. Click **Install**

### 4. Upload Firmware (5 minutes)

1. Connect your Giga R1 to computer via USB-C
2. In Arduino IDE, go to **File** → **Open**
3. Navigate to `firmware/giga_r1_central_controller/`
4. Open `giga_r1_central_controller.ino`
5. Go to **Tools** → **Board** → **Arduino Mbed OS Giga Boards** → **Arduino Giga R1**
6. Go to **Tools** → **Port** → Select your Giga R1 port
7. Click the **Upload** button (→)
8. Wait for "Done uploading" message

### 5. Verify Operation (2 minutes)

1. Open **Tools** → **Serial Monitor**
2. Set baud rate to **115200**
3. You should see:
   ```
   Arduino Giga R1 Central Controller
   Pothole Detection System
   ================================
   Initializing sensor input pins...
   ...
   System initialized successfully
   ```

## First Test

### Test 1: WiFi Access Point

1. On your phone/laptop, scan for WiFi networks
2. Look for "Pothole-Detection-AP"
3. Connect (no password required)

### Test 2: Web Dashboard

1. While connected to the AP, open a browser
2. Go to: `http://192.168.4.1/`
3. You should see the dashboard with 16 sensors

### Test 3: Pin Detection

1. Connect a jumper wire to pin 23 (first sensor)
2. Touch the other end to 5V pin
3. Check Serial Monitor for "POTHOLE DETECTED!" message
4. Check dashboard - sensor R1 should show "ALERT - HIGH"

## What's Next?

### Connect Sensor Nodes

Once you have sensor nodes ready:

1. Wire each sensor node's output pin to the corresponding Giga R1 input pin
2. Power on the sensor nodes
3. Watch the dashboard for real-time detections

### Configure Sensors

To calibrate a sensor:

1. Open the dashboard
2. Click "Configure" on any sensor
3. Enter zero reference (normal distance in cm)
4. Enter threshold (detection depth in cm, e.g., 5)
5. Click OK

### Monitor System

Keep the Serial Monitor open to see:
- System status messages
- Real-time detection events
- BLE connection logs
- Web server activity

## Common First-Time Issues

### "Board not found"
**Solution**: Install Arduino Mbed OS Giga Boards package (Step 2)

### "Library not found"
**Solution**: Install ArduinoBLE library (Step 3)

### "Upload failed"
**Solution**: 
- Check USB connection
- Try a different USB port
- Press reset button on Giga R1 twice quickly (enters bootloader)

### "WiFi AP not visible"
**Solution**: 
- Wait 30 seconds after upload
- Press reset button on Giga R1
- Check Serial Monitor for error messages

### "Dashboard not loading"
**Solution**:
- Verify you're connected to "Pothole-Detection-AP"
- Try http://192.168.4.1/ (not https)
- Clear browser cache

## Daily Operation

### Starting Up
1. Power on the Giga R1
2. Wait 10-15 seconds for initialization
3. WiFi AP will be available
4. Dashboard is accessible

### Monitoring
- Keep Serial Monitor open for real-time logs
- Access dashboard from any device on the WiFi
- Dashboard auto-refreshes every 5 seconds

### Shutting Down
- Simply disconnect power
- No special shutdown procedure required
- Settings are preserved (stored on sensor nodes)

## Tips for Success

✅ **Do:**
- Keep the Serial Monitor open during testing
- Test each pin individually before connecting all sensors
- Use appropriate wire gauge for all connections
- Document your sensor-to-pin mappings
- Make regular backups of your configuration

❌ **Don't:**
- Connect pins to voltages higher than 5V
- Block the WiFi antenna area
- Expose electronics to moisture
- Forget to ground all devices to common ground
- Upload firmware while sensors are actively triggering

## Getting Help

If you encounter issues:

1. **Check Serial Monitor** - Most errors are reported here
2. **Review Documentation**:
   - `README.md` - Detailed feature documentation
   - `INSTALLATION.md` - Complete installation guide
   - `ARCHITECTURE.md` - System design details
3. **Verify Connections** - Double-check all wiring
4. **Test Incrementally** - Test each component separately

## Example Use Case

### Scenario: Testing Road Section

1. **Setup Phase** (one-time)
   - Install firmware on Giga R1
   - Deploy 16 sensor nodes along road section
   - Connect each node's output to Giga R1 input pin
   - Power everything on

2. **Calibration Phase** (one-time per sensor)
   - Access dashboard via WiFi
   - For each sensor:
     - Click "Poll Sensor" to get current distance
     - Click "Configure"
     - Set zero reference to current distance
     - Set threshold to 5 cm (adjustable)

3. **Operation Phase** (continuous)
   - System automatically monitors all sensors
   - Dashboard shows real-time status
   - Serial Monitor logs all events
   - No user interaction required

4. **Maintenance Phase** (as needed)
   - Check dashboard for sensor health
   - Recalibrate if needed
   - Review event logs via Serial Monitor

## Dashboard Features Quick Reference

### Sensor Card
- **Green Background**: Normal operation (LOW)
- **Red Background**: Pothole detected (HIGH)
- **Poll Sensor**: Get live distance reading via BLE
- **Configure**: Set zero reference and threshold

### Buttons
- **Refresh Status**: Manual refresh (auto-refreshes every 5s)
- **Poll Sensor**: Connects via BLE to read current distance
- **Configure**: Opens dialog to set calibration values

### Information Displayed
- Sensor name (R1-R8, L8-L1)
- Current state (HIGH/LOW)
- Pin number (23-53)
- BLE device name

## Next Steps

After completing this quick start:

1. ✅ Read the full [README.md](giga_r1_central_controller/README.md) for detailed features
2. ✅ Review [ARCHITECTURE.md](ARCHITECTURE.md) to understand the system design
3. ✅ Check [INSTALLATION.md](INSTALLATION.md) for advanced configuration
4. ✅ Deploy your sensor nodes
5. ✅ Calibrate each sensor
6. ✅ Start monitoring!

## Success!

You now have a fully operational Pothole Detection System central controller!

The system is:
- ✅ Monitoring 16 digital input pins
- ✅ Logging events to Serial Monitor
- ✅ Broadcasting WiFi Access Point
- ✅ Serving web dashboard
- ✅ Ready for BLE communication

Enjoy your deployment! 🎉
