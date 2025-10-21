# Firmware Documentation

This directory contains the firmware for the Pothole Detection System components.

## Contents

### Giga R1 Central Controller

The main controller firmware that manages the entire system:

📂 **[giga_r1_central_controller/](giga_r1_central_controller/)**
- Main Arduino sketch for Arduino Giga R1 WiFi
- Monitors 16 sensor input pins
- Provides WiFi Access Point and web dashboard
- Handles BLE communication with sensor nodes

## Quick Links

### Getting Started
- 🚀 **[Quick Start Guide](QUICKSTART.md)** - Get running in 15 minutes
- 📥 **[Installation Guide](INSTALLATION.md)** - Complete setup instructions
- 📖 **[Main README](giga_r1_central_controller/README.md)** - Feature documentation

### Documentation
- 🏗️ **[Architecture](ARCHITECTURE.md)** - System design and implementation details
- 📝 **[Examples](EXAMPLES.md)** - API usage and integration examples
- ⚙️ **[Configuration](giga_r1_central_controller/config.h)** - Customizable settings

### Build Systems
- **Arduino IDE** - Traditional Arduino development (recommended for beginners)
- **PlatformIO** - Advanced build system (see [platformio.ini](giga_r1_central_controller/platformio.ini))

## Features

### ✅ Implemented

The Arduino Giga R1 Central Controller firmware includes:

1. **Pin Monitoring**
   - Monitors 16 digital input pins (23-53, odd pins)
   - Detects LOW→HIGH transitions
   - Logs events to serial monitor

2. **BLE Communication**
   - Scans for sensor nodes by name
   - Polls sensors for real-time distance readings
   - Configures sensor parameters (zero reference, threshold)
   - Standardized UUIDs for consistent communication

3. **WiFi Access Point**
   - Creates open WiFi network "Pothole-Detection-AP"
   - Default IP: 192.168.4.1
   - No password required (configurable)

4. **Web Dashboard**
   - Real-time sensor state display
   - Auto-refresh every 5 seconds
   - Interactive controls for polling and configuration
   - Mobile-responsive design

5. **HTTP API**
   - RESTful endpoints for system integration
   - JSON responses for easy parsing
   - CORS enabled for cross-origin access

## Hardware Requirements

- **Arduino Giga R1 WiFi** board
- **16 Sensor Nodes** (Arduino Nano 33 BLE + TFLuna LIDAR)
- USB-C cable for programming
- Appropriate power supply
- Jumper wires for connections

## Software Requirements

### Required Libraries

1. **WiFi** - Built-in with Arduino Giga R1 board package
2. **ArduinoBLE** - Version 1.3.6 or higher

### Board Support

- **Arduino Mbed OS Giga Boards** package
- Available through Arduino IDE Board Manager

## Pin Mapping

| Sensor | Position | Giga Pin | Nano Pin | BLE Device Name |
|--------|----------|----------|----------|----------------|
| R1 | Right 1 | 23 | D2 | NanoTFluna-R1 |
| R2 | Right 2 | 25 | D2 | NanoTFluna-R2 |
| R3 | Right 3 | 27 | D2 | NanoTFluna-R3 |
| R4 | Right 4 | 29 | D2 | NanoTFluna-R4 |
| R5 | Right 5 | 31 | D2 | NanoTFluna-R5 |
| R6 | Right 6 | 33 | D2 | NanoTFluna-R6 |
| R7 | Right 7 | 35 | D2 | NanoTFluna-R7 |
| R8 | Right 8 | 37 | D2 | NanoTFluna-R8 |
| L8 | Left 8 | 39 | D2 | NanoTFluna-L8 |
| L7 | Left 7 | 41 | D2 | NanoTFluna-L7 |
| L6 | Left 6 | 43 | D2 | NanoTFluna-L6 |
| L5 | Left 5 | 45 | D2 | NanoTFluna-L5 |
| L4 | Left 4 | 47 | D2 | NanoTFluna-L4 |
| L3 | Left 3 | 49 | D2 | NanoTFluna-L3 |
| L2 | Left 2 | 51 | D2 | NanoTFluna-L2 |
| L1 | Left 1 | 53 | D2 | NanoTFluna-L1 |

## API Endpoints

### GET /
Main web dashboard interface

### GET /api/sensors
Returns JSON with all sensor states
```json
{
  "sensors": [
    {"index": 0, "name": "R1", "pin": 23, "state": false, "ble_name": "NanoTFluna-R1"},
    ...
  ]
}
```

### GET /api/poll/{index}
Polls specific sensor via BLE (index 0-15)
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
Configures sensor via BLE (index 0-15)

Request:
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

## Usage Examples

### Access Web Dashboard

1. Connect to WiFi network "Pothole-Detection-AP"
2. Open browser to http://192.168.4.1/
3. View sensor states and interact with controls

### Monitor via Serial

1. Open Serial Monitor at 115200 baud
2. Watch for detection events:
```
================================
POTHOLE DETECTED!
Sensor: R2 (Pin 25)
Timestamp: 45230 ms
================================
```

### Query via API (Python)

```python
import requests

# Get all sensor states
response = requests.get('http://192.168.4.1/api/sensors')
sensors = response.json()

# Poll specific sensor
response = requests.get('http://192.168.4.1/api/poll/0')
data = response.json()
print(f"Distance: {data['distance']} cm")

# Configure sensor
response = requests.post(
    'http://192.168.4.1/api/configure/0',
    json={'zero': 150.0, 'threshold': 5.0}
)
result = response.json()
print(result['message'])
```

See [EXAMPLES.md](EXAMPLES.md) for more usage patterns.

## Customization

Edit `giga_r1_central_controller/config.h` to customize:

- WiFi AP name and password
- BLE UUIDs and timeouts
- Pin assignments
- Serial baud rate
- Debug output options

## Development

### Building with Arduino IDE

1. Install Arduino IDE 2.0+
2. Install Arduino Mbed OS Giga Boards package
3. Install ArduinoBLE library
4. Open `giga_r1_central_controller.ino`
5. Select Board: Arduino Giga R1
6. Upload to board

### Building with PlatformIO

1. Install VS Code + PlatformIO
2. Open firmware directory
3. Run `pio run` to build
4. Run `pio run --target upload` to flash

## Testing

### Unit Testing
Currently no automated unit tests. Testing requires hardware.

### Integration Testing
1. Flash firmware to Giga R1
2. Connect to WiFi AP
3. Access dashboard
4. Test each API endpoint
5. Verify pin detection with jumper wires

### System Testing
1. Deploy with sensor nodes
2. Verify digital pin alerts
3. Test BLE polling and configuration
4. Monitor for extended period

## Troubleshooting

### Common Issues

**WiFi AP not visible**
- Check Serial Monitor for errors
- Verify board has WiFi capability
- Press reset button

**BLE not connecting**
- Ensure sensor nodes are powered on
- Check device names match
- Verify UUIDs are correct
- Move devices closer together

**Dashboard not loading**
- Verify connected to correct WiFi
- Try http://192.168.4.1/ (not https)
- Clear browser cache

See [INSTALLATION.md](INSTALLATION.md) for more troubleshooting steps.

## Future Development

### Planned Enhancements

- Data logging to SD card
- Real-time timestamps with RTC
- Email/SMS notifications
- Cloud integration (MQTT)
- Mobile app support
- OTA firmware updates
- Advanced analytics

### Contributing

This is a project in active development. Contributions welcome for:
- Bug fixes
- Performance improvements
- New features
- Documentation improvements
- Test coverage

## License

See LICENSE file in repository root.

## Support

For questions, issues, or contributions:
- Review documentation in this directory
- Check existing issues in repository
- Create new issue with detailed information

## Directory Structure

```
firmware/
├── README.md                          # This file
├── QUICKSTART.md                      # 15-minute setup guide
├── INSTALLATION.md                    # Detailed installation
├── ARCHITECTURE.md                    # System design
├── EXAMPLES.md                        # API usage examples
└── giga_r1_central_controller/        # Main firmware
    ├── giga_r1_central_controller.ino # Arduino sketch
    ├── config.h                       # Configuration
    ├── platformio.ini                 # PlatformIO config
    └── README.md                      # Component README
```

## Version History

### v1.0.0 (Current)
- Initial implementation
- 16-pin digital monitoring
- BLE communication
- WiFi Access Point
- Web dashboard
- HTTP API
- Serial logging

## Credits

Developed for the Pothole Detection System project.

---

**Ready to get started?** → [Quick Start Guide](QUICKSTART.md)

**Need help?** → [Installation Guide](INSTALLATION.md)

**Want details?** → [Architecture Documentation](ARCHITECTURE.md)
