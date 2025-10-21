# Quick Start Guide

Get the Pothole Detection System Dashboard up and running in minutes!

## Option 1: Testing Without Hardware (Recommended for Development)

### Prerequisites
- Node.js installed on your computer
- A modern web browser (Chrome, Edge, or Opera)

### Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/LVForestry/pothole-detection-system.git
   cd pothole-detection-system/dashboard
   ```

2. **Start the mock server**
   ```bash
   node mock-server.js
   ```

3. **Open your browser**
   - Navigate to `http://localhost:8080`
   - You should see the dashboard with simulated sensor data

4. **Explore the features**
   - Watch sensors randomly change states (simulating pothole detection)
   - Check the events log for detection alerts
   - Click the gear icon (⚙️) to see the configuration modal
   - Resize your browser to see the responsive design

## Option 2: Using with Real Hardware

### Prerequisites
- Arduino Giga R1 WiFi configured as Access Point
- 16 Arduino Nano 33 BLE devices with TFLuna LIDAR sensors
- Device with WiFi and Bluetooth capabilities
- Chrome, Edge, or Opera browser

### Steps

1. **Deploy to Arduino Giga R1**
   - Copy `index.html`, `styles.css`, and `app.js` to the Giga R1's web server
   - Configure the Giga R1 to serve these files

2. **Implement API Endpoints on Giga R1**
   
   The dashboard expects these endpoints:
   
   **GET /api/status**
   ```json
   {
     "status": "ok",
     "timestamp": "2025-10-21T15:30:00Z"
   }
   ```
   
   **GET /api/sensors**
   ```json
   {
     "R1": { "status": "low" },
     "R2": { "status": "high" },
     "R3": { "status": "low" },
     ...
     "L1": { "status": "low" }
   }
   ```
   
   Status values: `"low"` (normal), `"high"` (pothole detected), `"unknown"`

3. **Configure Arduino Nano BLE Devices**
   
   Each Nano should:
   - Advertise with name: `NanoTFluna-R1` through `NanoTFluna-R8`, `NanoTFluna-L8` through `NanoTFluna-L1`
   - Implement BLE service UUID: `19B10000-E8F2-537E-4F6C-D104768A1214`
   - Provide characteristics:
     - Distance (Read): `19B10001-E8F2-537E-4F6C-D104768A1214` (Float32, little-endian)
     - Zero (Read/Write): `19B10002-E8F2-537E-4F6C-D104768A1214` (Float32, little-endian)
     - Threshold (Read/Write): `19B10003-E8F2-537E-4F6C-D104768A1214` (Float32, little-endian)

4. **Connect to Dashboard**
   - Connect your device to the Giga R1's WiFi Access Point
   - Open your browser and navigate to the Giga R1's IP address (e.g., `http://192.168.4.1`)
   - The dashboard should automatically connect and start polling sensors

5. **Configure Sensors**
   - Click the 📡 icon to poll an individual sensor via BLE
   - Click the ⚙️ icon to configure zero reference and threshold
   - Use "Set Current as Zero" for easy calibration

## Troubleshooting

### Mock Server Issues

**Problem:** `node: command not found`
- **Solution:** Install Node.js from [nodejs.org](https://nodejs.org)

**Problem:** `Error: listen EADDRINUSE`
- **Solution:** Port 8080 is already in use. Change the PORT in `mock-server.js` or stop the other process

### Hardware Issues

**Problem:** WiFi status shows "Disconnected"
- **Solution:** Ensure you're connected to the Giga R1's WiFi network
- Check that the Giga R1 is powered on and broadcasting

**Problem:** BLE shows "Not Supported"
- **Solution:** Use Chrome, Edge, or Opera browser
- Ensure Bluetooth is enabled on your device
- Try accessing the dashboard via HTTPS (BLE requires secure context)

**Problem:** Cannot poll sensors via BLE
- **Solution:** 
  - Verify Arduino Nanos are powered on and advertising
  - Check BLE device names match expected format
  - Ensure you're within BLE range (typically < 10 meters)
  - Grant Bluetooth permissions when prompted

**Problem:** Sensors show "Unknown" status
- **Solution:**
  - Verify API endpoints are responding correctly
  - Check browser console for error messages
  - Ensure Giga R1 is reading digital input pins correctly

## Tips for Best Experience

1. **Use Chrome for full BLE support** - Chrome has the most mature Web Bluetooth implementation

2. **Calibrate sensors regularly** - Use the "Set Current as Zero" feature to establish baseline measurements

3. **Monitor the events log** - Keep an eye on the Recent Events section for pothole detections

4. **Test responsiveness** - Try the dashboard on different devices (phone, tablet, desktop)

5. **Keep browser updated** - Ensure you're using the latest version for best compatibility

## What's Next?

- Check out the [detailed README](README.md) for complete documentation
- Review the sensor pin mapping in the main project README
- Customize the polling interval in `app.js` if needed
- Modify BLE UUIDs if using different values on your hardware

## Need Help?

- Review the main project [README](/README.md)
- Check the dashboard [README](README.md)
- Look at the browser console for error messages
- Verify your hardware matches the expected configuration

Enjoy monitoring your pothole detection system! 🚗🕳️
