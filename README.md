# pothole-detection-system

pothole-detection-system

Project Overview:
A distributed system for real-time detection and reporting of potholes (deeper than 5cm) on roads, using multiple LIDAR sensors and microcontrollers, with remote monitoring and configuration via a web dashboard.

System Architecture:

1. Sensor Nodes:
   - 16 nodes, each combining a TFLuna LIDAR sensor and an Arduino Nano 33 BLE.
   - Sensors are positioned in two lines: R1–R8 (right), L8–L1 (left).
   - Each Nano reads its LIDAR via I2C, processes the distance data, and outputs a digital HIGH on pin 2 to indicate a pothole (distance drop beyond threshold), holding HIGH for 2 seconds after the event.
   - Each Nano responds to BLE requests:
       * Provides real-time distance
       * Updates/stores zero reference and threshold values in flash
   - Each Nano advertises a unique BLE name (e.g., "NanoTFluna-R1").

2. Central Controller:
   - Arduino Giga R1 as the core controller.
   - Monitors 16 digital input pins (23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53), mapped to the 16 Nanos.
   - Logs sensor events to serial when any input goes HIGH.
   - Polls/configures each Nano over BLE for live readings, zeroing, and threshold changes.

3. Communication:
   - Nano ↔ TFLuna: I2C for distance measurement.
   - Nano ↔ Giga: Digital output (event alert) and BLE (data/config/debug).

4. Web Dashboard:
   - Giga R1 creates an open WiFi Access Point (no password required).
   - Serves a web interface for:
       * Viewing the HIGH/LOW state of all sensors in real time
       * Polling individual sensors for live data via BLE
       * Setting/calibrating zero and threshold values (persisted to each Nano)

Naming and Pin Mapping:
- Sensor names: R1–R8 (right), L8–L1 (left).
- Giga digital input pin mapping: R1=23, R2=25, ..., R8=37, L8=39, ..., L1=53.
- Each Nano BLE device is uniquely named (e.g., "NanoTFluna-R1", etc).

User Interaction:
- User connects to the Giga’s WiFi AP and opens the web dashboard.
- Can view sensor states, get real-time readings, and update calibration or thresholds.

Requirements:
- Detection of potholes deeper than 5cm (configurable threshold).
- Robust, real-time digital and BLE communication.
- Local, passwordless WiFi access and web monitoring.
- All settings persist across power cycles (flash storage on Nanos).

## Implementation Status

### ✅ Completed Components

#### Sensor Node Firmware (Arduino Nano 33 BLE)
**Location**: `firmware/nano_33_ble_sensor/`

**Status**: Production-ready (v1.0.0)

**Features Implemented**:
- ✅ TFLuna LIDAR I2C communication (400kHz, 10Hz)
- ✅ Pothole detection (>5cm configurable threshold)
- ✅ Digital output signal (pin 2, 2-second HIGH)
- ✅ BLE service with 4 characteristics
- ✅ Flash memory configuration persistence
- ✅ Unique BLE naming (NanoTFluna-R1 through L1)
- ✅ 16 pre-configured examples ready to deploy

**BLE Protocol Details** (Implemented):
- Service UUID: `12340000-1234-5678-9abc-def012345678`
- Characteristics:
  - Distance (12340001-...): Int16, Read/Notify - Real-time distance in cm
  - Zero Reference (12340002-...): Int16, Read/Write - Calibration baseline
  - Threshold (12340003-...): Int16, Read/Write - Detection threshold in cm
  - Status (12340004-...): String, Read - Status messages

**Documentation**:
- Complete user guides (QUICKSTART, README, DEPLOYMENT)
- Hardware wiring guide (WIRING)
- Testing procedures (TESTING)
- System architecture (ARCHITECTURE)
- Troubleshooting guide (TROUBLESHOOTING)
- Python BLE test client included

See `firmware/README.md` for detailed information.

### 🚧 Pending Components

#### Central Controller (Arduino Giga R1)
- Digital input monitoring for 16 sensors
- BLE client for sensor configuration
- Event logging and management
- WiFi Access Point setup
- Web server implementation

#### Web Dashboard
- Real-time sensor state display
- Individual sensor data polling
- Configuration interface (zero ref, threshold)
- Event logging and history
- Export functionality

## Getting Started

### For Sensor Node Deployment

1. **Navigate to firmware directory**:
   ```bash
   cd firmware/nano_33_ble_sensor
   ```

2. **Quick start** (for testing):
   - Follow `QUICKSTART.md` for first sensor setup

3. **Full deployment** (for production):
   - Follow `DEPLOYMENT.md` for all 16 sensors
   - Use pre-configured examples in `examples/` directory

4. **Hardware setup**:
   - See `WIRING.md` for connection details
   - Each sensor connects TFLuna via I2C
   - Pin 2 outputs to Giga R1 input pins

5. **Testing**:
   - Follow `TESTING.md` for validation procedures
   - Use `ble_test_client.py` for BLE testing

### For System Integration

1. **Sensor nodes**: Upload firmware from `firmware/nano_33_ble_sensor/examples/`
2. **Connect hardware**: Follow pin mapping in `firmware/nano_33_ble_sensor/WIRING.md`
3. **Test individually**: Verify each sensor per `firmware/nano_33_ble_sensor/TESTING.md`
4. **Central controller**: (To be implemented)
5. **Web dashboard**: (To be implemented)

## Technical Specifications

### Sensor Nodes
- **Microcontroller**: Arduino Nano 33 BLE (nRF52840)
- **LIDAR**: TFLuna (I2C, 400kHz)
- **Measurement Rate**: 10Hz
- **Detection Threshold**: 5cm (configurable, persistent)
- **Output Signal**: 3.3V HIGH for 2 seconds
- **BLE Range**: >10m line-of-sight
- **Power**: ~200mA per node @ 5V

### System Capacity
- **Sensors**: 16 nodes (R1-R8, L1-L8)
- **Total Power**: ~3.2A @ 5V (5A supply recommended)
- **Detection Latency**: <100ms
- **Distance Accuracy**: ±2cm @ <3m range

## Repository Structure

```
pothole-detection-system/
├── README.md                          # This file
├── LICENSE                            # Apache 2.0 License
└── firmware/                          # Firmware directory
    ├── README.md                      # Firmware overview
    └── nano_33_ble_sensor/            # Sensor node firmware
        ├── nano_33_ble_sensor.ino    # Main Arduino sketch
        ├── README.md                  # Complete guide
        ├── QUICKSTART.md              # Fast setup
        ├── DEPLOYMENT.md              # Production deployment
        ├── WIRING.md                  # Hardware guide
        ├── TESTING.md                 # Validation procedures
        ├── ARCHITECTURE.md            # System design
        ├── TROUBLESHOOTING.md         # Diagnostics
        ├── CHANGELOG.md               # Version history
        ├── generate_configs.py        # Config generator
        ├── ble_test_client.py         # BLE test tool
        └── examples/                  # Pre-configured sketches
            ├── R1/, R2/, ..., R8/     # Right sensors
            └── L1/, L2/, ..., L8/     # Left sensors
```

End of summary.
