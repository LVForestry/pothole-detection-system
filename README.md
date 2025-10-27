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

### ✅ Completed - Central Controller (Giga R1)

The Arduino Giga R1 central controller firmware is fully implemented and ready for deployment:

- **Location**: [`firmware/giga_r1_central_controller/`](firmware/giga_r1_central_controller/)
- **Features**:
  - ✅ 16 digital input pin monitoring (pins 23-53)
  - ✅ Serial logging of pothole detection events
  - ✅ BLE communication (scan, poll, configure sensor nodes)
  - ✅ WiFi Access Point ("Pothole-Detection-AP")
  - ✅ Web dashboard with real-time monitoring
  - ✅ HTTP API for system integration
  - ✅ Sensor polling and configuration via BLE

### 📋 BLE Protocol Specification

The system uses standardized BLE characteristics for Nano-Giga communication:

**Service UUID**: `19b10000-e8f2-537e-4f6c-d104768a1214`

**Characteristics**:
- Distance (Read): `19b10001-e8f2-537e-4f6c-d104768a1214` - Float (4 bytes) in cm
- Zero Reference (Write): `19b10002-e8f2-537e-4f6c-d104768a1214` - Float (4 bytes) in cm
- Threshold (Write): `19b10003-e8f2-537e-4f6c-d104768a1214` - Float (4 bytes) in cm

### 📚 Documentation

Comprehensive documentation available in the firmware directory:

- **[Quick Start Guide](firmware/QUICKSTART.md)** - Get running in 15 minutes
- **[Installation Guide](firmware/INSTALLATION.md)** - Complete setup instructions
- **[Architecture](firmware/ARCHITECTURE.md)** - System design and implementation
- **[Examples](firmware/EXAMPLES.md)** - API usage and integration patterns
- **[Main README](firmware/README.md)** - Overview and feature list

### 🚀 Getting Started

1. Navigate to [`firmware/giga_r1_central_controller/`](firmware/giga_r1_central_controller/)
2. Follow the [Quick Start Guide](firmware/QUICKSTART.md)
3. Upload firmware to Arduino Giga R1 WiFi
4. Connect to "Pothole-Detection-AP" WiFi network
5. Access dashboard at http://192.168.4.1/

### 🔧 Next Steps

- **Sensor Node Firmware**: Develop Arduino Nano 33 BLE firmware for sensor nodes
- **Field Deployment**: Install and calibrate sensors on target roads
- **System Testing**: Validate detection accuracy and reliability
- **Dashboard Enhancements**: Add data logging, analytics, and export features

End of summary.
