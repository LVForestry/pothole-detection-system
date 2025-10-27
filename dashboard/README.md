# Pothole Detection System - Web Dashboard

A responsive web-based dashboard for monitoring and configuring the pothole detection system in real-time.

## Features

### 1. Real-Time Sensor Monitoring
- Visual display of all 16 sensors (R1-R8, L8-L1) in a grid layout
- Color-coded indicators:
  - 🟢 **GREEN (LOW)**: Normal road surface
  - 🔴 **RED (HIGH)**: Pothole detected (pulsing animation)
  - ⚫ **GRAY (Unknown)**: Sensor status unknown or not connected

### 2. WiFi Connectivity
- Connects to Arduino Giga R1's WiFi Access Point
- Automatic polling of sensor states every second
- Real-time status updates without page refresh
- Connection status indicators in the header

### 3. BLE Communication
- Individual sensor polling via Bluetooth Low Energy
- Read real-time distance measurements from each sensor
- Configure zero reference and threshold values
- Persistent storage on Arduino Nano devices

### 4. Configuration Interface
- User-friendly modal dialogs for sensor configuration
- Set zero reference point (calibration)
- Adjust detection threshold (default: 5.0 cm)
- Quick "Set Current as Zero" button for easy calibration
- Real-time reading display during configuration

### 5. Events Logging
- Automatic logging of pothole detection events
- Status change notifications
- Configuration change history
- Timestamped event entries
- Scrollable log with last 50 events

### 6. Responsive Design
- Mobile-friendly interface
- Touch-optimized controls
- Adaptive layout for different screen sizes
- Works on tablets, phones, and desktop browsers

## Requirements

### Browser Compatibility
The dashboard requires a modern browser with Web Bluetooth API support:
- ✅ **Google Chrome** 56+ (recommended)
- ✅ **Microsoft Edge** 79+
- ✅ **Opera** 43+
- ❌ Firefox (Web Bluetooth not supported)
- ❌ Safari (Web Bluetooth not supported)

### Hardware Requirements
- Arduino Giga R1 WiFi configured as Access Point
- 16 Arduino Nano 33 BLE devices with TFLuna LIDAR sensors
- Device with WiFi and Bluetooth capabilities

## Installation

### Option 1: Serve from Arduino Giga R1 (Recommended)
1. Upload the dashboard files to the Giga R1's SD card or internal storage
2. Configure the Giga R1 to serve the files over HTTP
3. Connect to the Giga R1's WiFi AP
4. Access the dashboard at `http://192.168.4.1/` (or configured IP)

### Option 2: Local Development
1. Clone the repository
2. Navigate to the `dashboard` directory
3. Serve using any web server:
   ```bash
   # Using Python
   python -m http.server 8000
   
   # Using Node.js
   npx http-server -p 8000
   
   # Using PHP
   php -S localhost:8000
   ```
4. Open `http://localhost:8000` in your browser

## Usage Instructions

### Initial Setup

1. **Connect to WiFi**
   - Connect your device to the Arduino Giga R1's WiFi Access Point
   - Open the dashboard in a compatible browser
   - Click "Connect to Giga WiFi AP" if not auto-detected
   - Wait for the connection status to show "Connected"

2. **Verify BLE Support**
   - The dashboard automatically checks for Web Bluetooth API support
   - The BLE status indicator shows "Available" if supported
   - If not supported, switch to a compatible browser

### Monitoring Sensors

1. **View Sensor Status**
   - All 16 sensors are displayed in two columns (Left and Right)
   - Sensor indicators update automatically every second
   - Watch for red (HIGH) indicators showing pothole detection
   - Green (LOW) indicators show normal road surface

2. **Poll Individual Sensor**
   - Click the 📡 (radio) icon next to any sensor
   - Grant Bluetooth permissions when prompted
   - View detailed readings in the sensor details panel
   - See current distance, zero reference, and threshold values

### Configuring Sensors

1. **Open Configuration**
   - Click the ⚙️ (gear) icon next to any sensor
   - The configuration modal opens

2. **Set Zero Reference (Calibration)**
   - Option A: Enter a manual value in the "Zero Reference" field
   - Option B: Click "Set Current as Zero" to use the current reading
   - This sets the baseline distance for pothole detection

3. **Adjust Threshold**
   - Enter the desired threshold in centimeters (default: 5.0 cm)
   - Potholes are detected when distance increases by more than this value
   - Example: Zero = 50 cm, Threshold = 5 cm → Detect when > 55 cm

4. **Save Configuration**
   - Click "Save Configuration" to write settings via BLE
   - Settings are stored in flash memory on the Arduino Nano
   - Configuration persists across power cycles

### Viewing Events

- The "Recent Events" section logs all system activities
- Events include:
  - Pothole detection alerts
  - Sensor status changes
  - Configuration updates
  - BLE polling results
- Click "Clear Log" to remove all events

## API Endpoints

The dashboard expects the following API endpoints on the Giga R1:

### GET `/api/status`
Check system status and connectivity.

**Response:**
```json
{
  "status": "ok",
  "timestamp": "2025-10-21T15:30:00Z"
}
```

### GET `/api/sensors`
Get current status of all sensors.

**Response:**
```json
{
  "R1": { "status": "low" },
  "R2": { "status": "high" },
  ...
  "L1": { "status": "low" }
}
```

Status values: `"low"`, `"high"`, `"unknown"`

## BLE Protocol

### Service UUID
- Service: `19B10000-E8F2-537E-4F6C-D104768A1214`

### Characteristics
- **Distance** (Read): `19B10001-E8F2-537E-4F6C-D104768A1214`
  - Format: Float32 (little-endian)
  - Unit: centimeters
  
- **Zero Reference** (Read/Write): `19B10002-E8F2-537E-4F6C-D104768A1214`
  - Format: Float32 (little-endian)
  - Unit: centimeters
  - Stored in flash memory
  
- **Threshold** (Read/Write): `19B10003-E8F2-537E-4F6C-D104768A1214`
  - Format: Float32 (little-endian)
  - Unit: centimeters
  - Stored in flash memory

### Device Names
Each Arduino Nano advertises with a unique name:
- `NanoTFluna-R1` through `NanoTFluna-R8` (right side)
- `NanoTFluna-L8` through `NanoTFluna-L1` (left side)

## Troubleshooting

### WiFi Connection Issues
- Ensure you're connected to the correct WiFi network
- Check that the Giga R1 is powered on and broadcasting
- Verify the IP address matches your Giga R1 configuration
- Try refreshing the page after connecting to WiFi

### BLE Not Working
- Use Chrome, Edge, or Opera browser
- Enable Bluetooth on your device
- Grant Bluetooth permissions when prompted
- Ensure Arduino Nanos are powered on and advertising
- Try moving closer to the sensors (BLE range is limited)

### Sensors Showing "Unknown"
- Check WiFi connection to Giga R1
- Verify the Giga R1 is receiving signals from the Nanos
- Check that the API endpoints are responding correctly
- Inspect browser console for error messages

### Configuration Not Saving
- Ensure stable BLE connection during write operation
- Check that the Arduino Nano has flash memory available
- Verify BLE characteristic UUIDs match Arduino implementation
- Try configuring again if the first attempt fails

## Development

### File Structure
```
dashboard/
├── index.html      # Main HTML structure
├── styles.css      # Responsive CSS styling
├── app.js          # JavaScript application logic
└── README.md       # This documentation
```

### Customization

**Change Polling Interval:**
Edit `app.js`:
```javascript
const CONFIG = {
    pollingInterval: 1000, // milliseconds
    ...
};
```

**Modify API Base URL:**
Edit `app.js`:
```javascript
const CONFIG = {
    apiBaseUrl: '/api', // change to your endpoint
    ...
};
```

**Update BLE UUIDs:**
Edit `app.js`:
```javascript
const CONFIG = {
    bleServiceUUID: 'your-service-uuid',
    bleCharacteristics: {
        distance: 'your-distance-uuid',
        ...
    },
    ...
};
```

### Testing Without Hardware

For development without physical hardware, you can:
1. Mock the API responses with a local server
2. Use browser developer tools to simulate responses
3. Create a test harness that returns dummy sensor data

## License

This project follows the same license as the parent repository.

## Support

For issues, questions, or contributions, please refer to the main project repository.
