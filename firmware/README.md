# Pothole Detection System - Firmware

This directory contains the firmware for the Arduino-based components of the pothole detection system.

## Directory Structure

```
firmware/
├── README.md                          # This file
├── .gitignore                         # Git ignore rules for build artifacts
└── nano_33_ble_sensor/                # Arduino Nano 33 BLE sensor node firmware
    ├── nano_33_ble_sensor.ino         # Main Arduino sketch
    ├── README.md                      # Complete functionality guide
    ├── QUICKSTART.md                  # Fast-track setup guide
    ├── DEPLOYMENT.md                  # Multi-sensor deployment guide
    ├── WIRING.md                      # Hardware connection guide
    ├── TESTING.md                     # Comprehensive testing procedures
    ├── config_template.h              # Configuration template
    ├── generate_configs.py            # Configuration generator script
    ├── ble_test_client.py             # Python BLE testing tool
    ├── requirements.txt               # Python dependencies
    └── examples/                      # Pre-configured sensor examples
        ├── README.md                  # Examples guide
        ├── R1/                        # Right sensor 1 (pre-configured)
        │   ├── nano_33_ble_sensor_R1.ino
        │   └── README.md
        ├── R2/ ... R8/                # Right sensors 2-8
        └── L1/ ... L8/                # Left sensors 1-8
```

## Components

### Arduino Nano 33 BLE Sensor Nodes

**Location**: `nano_33_ble_sensor/`

Each sensor node combines:
- Arduino Nano 33 BLE microcontroller
- TFLuna LIDAR sensor (I2C)
- BLE communication for remote monitoring/configuration
- Digital output for event signaling

**Key Features**:
- Real-time distance measurement (10Hz)
- Pothole detection (>5cm depth, configurable)
- 2-second digital output signal on detection
- BLE service for live data and configuration
- Flash memory for persistent settings
- Unique BLE naming per sensor (R1-R8, L1-L8)

**Documentation**:
- Start with `nano_33_ble_sensor/QUICKSTART.md` for first-time setup
- See `nano_33_ble_sensor/README.md` for complete reference
- Use `nano_33_ble_sensor/DEPLOYMENT.md` for production deployment
- Reference `nano_33_ble_sensor/WIRING.md` for hardware connections
- Follow `nano_33_ble_sensor/TESTING.md` for validation procedures

## Quick Start

### For Development/Testing

1. **Install Prerequisites**
   ```bash
   # Install Arduino IDE from arduino.cc
   # Add Arduino Mbed OS Nano Boards support
   # Install ArduinoBLE and FlashStorage libraries
   ```

2. **Open Pre-configured Example**
   ```bash
   cd nano_33_ble_sensor/examples/R1/
   # Open nano_33_ble_sensor_R1.ino in Arduino IDE
   ```

3. **Upload to Board**
   - Select Board: Arduino Nano 33 BLE
   - Select correct COM/USB port
   - Click Upload

4. **Verify Operation**
   - Open Serial Monitor (115200 baud)
   - Check for initialization messages
   - Use nRF Connect app to verify BLE advertising

See `nano_33_ble_sensor/QUICKSTART.md` for detailed instructions.

### For Production Deployment

1. **Prepare 16 Arduino Nano 33 BLE boards**
2. **Upload firmware to each** using pre-configured examples (R1-R8, L1-L8)
3. **Connect hardware** following `nano_33_ble_sensor/WIRING.md`
4. **Test each sensor** using `nano_33_ble_sensor/TESTING.md` checklist
5. **Calibrate** by setting zero reference and threshold via BLE
6. **Integrate** with Giga R1 central controller

See `nano_33_ble_sensor/DEPLOYMENT.md` for step-by-step process.

## System Architecture

### Sensor Node Architecture

```
┌─────────────────────────────────────────────┐
│         Arduino Nano 33 BLE                 │
│                                             │
│  ┌────────────┐  ┌──────────────┐          │
│  │  I2C Bus   │  │     BLE      │          │
│  │            │  │   Service    │          │
│  └──────┬─────┘  └──────┬───────┘          │
│         │                │                  │
│    ┌────▼────┐      ┌───▼────┐             │
│    │ TFLuna  │      │  BLE   │             │
│    │ Handler │      │ Chars  │             │
│    └────┬────┘      └────────┘             │
│         │                                   │
│    ┌────▼──────────────┐                   │
│    │  Detection Logic  │                   │
│    │  (Threshold Check)│                   │
│    └────┬──────────────┘                   │
│         │                                   │
│    ┌────▼────────┐  ┌──────────────┐       │
│    │   Flash     │  │  Digital Out │       │
│    │   Storage   │  │   (Pin 2)    │       │
│    └─────────────┘  └──────┬───────┘       │
│                            │               │
└────────────────────────────┼───────────────┘
                             │
                        To Giga R1
```

### System Integration

```
16 x Sensor Nodes → Digital Signals → Giga R1 Controller → WiFi AP → Web Dashboard
       ↓                                      ↑
    BLE Connection ←───────────────────────────┘
 (Config & Monitoring)
```

## Sensor Positions and Pin Mapping

### Right Side Sensors (R1-R8)
| Sensor | BLE Name | Giga Pin | Output Pin |
|--------|----------|----------|------------|
| R1 | NanoTFluna-R1 | 23 | 2 |
| R2 | NanoTFluna-R2 | 25 | 2 |
| R3 | NanoTFluna-R3 | 27 | 2 |
| R4 | NanoTFluna-R4 | 29 | 2 |
| R5 | NanoTFluna-R5 | 31 | 2 |
| R6 | NanoTFluna-R6 | 33 | 2 |
| R7 | NanoTFluna-R7 | 35 | 2 |
| R8 | NanoTFluna-R8 | 37 | 2 |

### Left Side Sensors (L8-L1)
| Sensor | BLE Name | Giga Pin | Output Pin |
|--------|----------|----------|------------|
| L8 | NanoTFluna-L8 | 39 | 2 |
| L7 | NanoTFluna-L7 | 41 | 2 |
| L6 | NanoTFluna-L6 | 43 | 2 |
| L5 | NanoTFluna-L5 | 45 | 2 |
| L4 | NanoTFluna-L4 | 47 | 2 |
| L3 | NanoTFluna-L3 | 49 | 2 |
| L2 | NanoTFluna-L2 | 51 | 2 |
| L1 | NanoTFluna-L1 | 53 | 2 |

**Note**: Each sensor's Pin 2 connects to its assigned Giga R1 input pin.

## BLE Service Specification

**Service UUID**: `12340000-1234-5678-9abc-def012345678`

### Characteristics

| Characteristic | UUID | Type | Properties | Description |
|---------------|------|------|------------|-------------|
| Distance | 12340001-... | Int16 | Read, Notify | Current distance in cm |
| Zero Reference | 12340002-... | Int16 | Read, Write | Calibration reference |
| Threshold | 12340003-... | Int16 | Read, Write | Detection threshold |
| Status | 12340004-... | String | Read | Status messages |

## Hardware Requirements

### Per Sensor Node
- 1x Arduino Nano 33 BLE
- 1x TFLuna LIDAR sensor
- Jumper wires (I2C + output)
- USB cable (for programming)
- Power supply: 5V @ 500mA
- Optional: Enclosure for protection

### Complete System (16 nodes)
- 16x Arduino Nano 33 BLE boards
- 16x TFLuna LIDAR sensors
- Wire for connections
- Power supply: 5V @ 5A (minimum)
- 1x Arduino Giga R1 (central controller)

## Software Requirements

### Arduino Development
- Arduino IDE 1.8.13+ or Arduino IDE 2.x
- Arduino Mbed OS Nano Boards package
- ArduinoBLE library (v1.3.0+)
- FlashStorage library (v1.0.0+)
- Wire library (included)

### Testing Tools
- Python 3.7+ (for BLE test client)
- bleak library (for Python BLE)
- nRF Connect mobile app (optional, for BLE testing)

## Configuration

### Sensor Position Configuration

Each sensor must be configured with its unique position:

**Method 1**: Use pre-configured examples (easiest)
- Open `examples/R1/nano_33_ble_sensor_R1.ino` for sensor R1
- Upload directly (no editing needed)

**Method 2**: Edit main sketch
- Open `nano_33_ble_sensor.ino`
- Change `#define SENSOR_POSITION "R1"` to desired position
- Upload to board

### Runtime Configuration (via BLE)

- **Zero Reference**: Set calibration baseline (stored in flash)
- **Threshold**: Adjust detection sensitivity (stored in flash)
- Both persist across power cycles

## Testing

### Unit Testing
Test individual sensor functionality:
- LIDAR communication
- Distance measurement
- Detection logic
- Output signal timing
- BLE connectivity
- Configuration persistence

### Integration Testing
Test with Giga R1 controller:
- Digital signal reception
- BLE communication
- Configuration via central
- Multiple sensor coordination

### System Testing
Test complete 16-sensor system:
- All sensors operational
- No interference
- Correct event logging
- Calibration across all nodes

See `nano_33_ble_sensor/TESTING.md` for detailed procedures.

## Troubleshooting

### Common Issues

**No Serial Output**
- Check baud rate (115200)
- Press reset button
- Verify USB connection

**LIDAR Not Responding**
- Check I2C wiring (SDA, SCL)
- Verify 5V power supply
- Run I2C scanner (address 0x10)

**BLE Not Working**
- Install ArduinoBLE library
- Select correct board (Nano 33 BLE)
- Check for error messages

**Detection Not Triggering**
- Verify threshold setting
- Check zero reference
- Confirm distance change >threshold

See individual component READMEs for detailed troubleshooting.

## Performance Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Measurement Rate | 10 Hz | Configurable |
| Detection Latency | <100 ms | Typical |
| Output Duration | 2 seconds | Configurable |
| BLE Range | >10 m | Line of sight |
| Distance Accuracy | ±2 cm | @ <3m range |
| Power per Node | ~200 mA | @ 5V |

## Future Enhancements

Potential improvements for future versions:
- OTA firmware updates via BLE
- Data logging to SD card
- Enhanced filtering algorithms
- Multi-threshold detection
- Temperature compensation
- Low-power sleep modes
- Event timestamping
- Diagnostic LED indicators

## Contributing

When adding new firmware:
1. Create subdirectory for new component
2. Include comprehensive README.md
3. Provide example configurations
4. Add testing procedures
5. Update this overview README

## License

See main repository LICENSE file.

## Support

For issues or questions:
1. Check component-specific README files
2. Review troubleshooting sections
3. Verify hardware connections
4. Test with example configurations
5. Check Serial Monitor for errors

## Related Documentation

- **Main Project**: `/README.md` - Overall system architecture
- **Sensor Firmware**: `nano_33_ble_sensor/README.md` - Detailed sensor docs
- **Quick Start**: `nano_33_ble_sensor/QUICKSTART.md` - Fast setup
- **Deployment**: `nano_33_ble_sensor/DEPLOYMENT.md` - Production guide
- **Wiring**: `nano_33_ble_sensor/WIRING.md` - Hardware connections
- **Testing**: `nano_33_ble_sensor/TESTING.md` - Validation procedures

---

**Version**: 1.0  
**Last Updated**: 2025-10-21  
**Status**: Production Ready
