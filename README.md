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
   - Dashboard features:
       * Responsive design for mobile and desktop devices
       * Real-time sensor status monitoring with color-coded indicators
       * BLE communication for individual sensor polling and configuration
       * Event logging for pothole detection and system activities
       * Intuitive configuration interface with modal dialogs
   - See [dashboard/README.md](dashboard/README.md) for detailed documentation

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

Open Points:
- BLE protocol details (characteristics, UUIDs) for Nano–Giga config/data exchange.
- Dashboard extensibility for logging, export, or analytics.

End of summary.
