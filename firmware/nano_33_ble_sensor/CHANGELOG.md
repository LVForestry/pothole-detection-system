# Changelog

All notable changes to the Arduino Nano 33 BLE Sensor Node firmware will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-10-21

### Added
- Initial release of Arduino Nano 33 BLE sensor node firmware
- TFLuna LIDAR sensor I2C communication (400kHz)
- Pothole detection with configurable threshold (default 5cm)
- Digital output on pin 2 (HIGH for 2 seconds on detection)
- BLE service with 4 characteristics:
  - Distance (Int16, Read/Notify) - Real-time distance in cm
  - Zero Reference (Int16, Read/Write) - Calibration baseline
  - Threshold (Int16, Read/Write) - Detection threshold
  - Status (String, Read) - Status messages
- Flash memory storage for configuration persistence
- Unique BLE naming per sensor position (NanoTFluna-R1, etc.)
- Serial debug output (115200 baud)
- 10Hz measurement rate
- Automatic configuration initialization with defaults
- Comprehensive error handling for I2C communication

### Documentation Added
- README.md - Complete functionality and usage guide
- QUICKSTART.md - Fast-track setup guide for first-time users
- DEPLOYMENT.md - Step-by-step deployment for all 16 sensors
- WIRING.md - Detailed hardware connection guide
- TESTING.md - Comprehensive testing and validation procedures
- CHANGELOG.md - This file

### Tools Added
- generate_configs.py - Automated configuration generator for all 16 sensors
- ble_test_client.py - Python BLE testing and calibration tool
- config_template.h - Configuration template
- requirements.txt - Python dependencies

### Examples Added
- 16 pre-configured sensor examples (R1-R8, L1-L8)
- Each example ready to upload without editing
- Individual READMEs with pin mappings and verification steps

### Features
- **Real-time Distance Monitoring**: Continuous distance measurement at 10Hz
- **Pothole Detection**: Automatic detection based on depth threshold
- **BLE Communication**: Remote monitoring and configuration
- **Configuration Persistence**: Settings saved to flash memory
- **Event Signaling**: 2-second digital HIGH output on detection
- **Unique Identification**: Position-based BLE names (R1-R8, L1-L8)
- **Robust Communication**: I2C error handling and recovery
- **User-friendly Setup**: Pre-configured examples for all sensors

### Technical Details
- Firmware size: ~8.3KB
- RAM usage: Minimal (well within Nano 33 BLE limits)
- Flash storage: 5 bytes per configuration
- I2C speed: 400kHz (Fast Mode)
- BLE update rate: 10Hz (matches measurement rate)
- Detection latency: <100ms
- Output signal duration: 2000ms ±50ms

### Hardware Support
- Arduino Nano 33 BLE (nRF52840)
- TFLuna LIDAR sensor (I2C mode)
- Compatible with Arduino Giga R1 controller

### Software Requirements
- Arduino IDE 1.8.13+ or Arduino IDE 2.x
- Arduino Mbed OS Nano Boards package
- ArduinoBLE library v1.3.0+
- FlashStorage library v1.0.0+

## [Unreleased]

### Planned Features
- OTA (Over-The-Air) firmware updates via BLE
- Data logging with timestamps
- Enhanced filtering for noise reduction
- Low-power sleep modes
- Diagnostic LED indicators
- Multi-threshold detection zones
- Temperature compensation
- Event buffering for missed BLE connections

### Known Limitations
- BLE range limited to ~10m line-of-sight
- I2C cable length should be <30cm for reliability
- No battery operation mode (requires constant 5V)
- Configuration limited to 2 parameters (zero ref, threshold)
- No built-in diagnostics for TFLuna sensor health

### Future Improvements
- Add CRC checking for flash storage
- Implement watchdog timer for fault recovery
- Add multiple detection zones with different thresholds
- Support for multiple LIDAR sensors per node
- Enhanced BLE security (pairing/bonding)
- UART mode support for TFLuna (as alternative to I2C)
- Configurable measurement rate
- Configurable output signal duration

## Version History

- **v1.0.0** (2025-10-21): Initial production release

---

## How to Report Issues

If you encounter bugs or have feature requests:
1. Check existing documentation for known limitations
2. Verify hardware connections and configuration
3. Review Serial Monitor output for error messages
4. Test with example configurations
5. Document steps to reproduce the issue
6. Report via project repository issues

## Upgrade Notes

### From Development to v1.0.0
- No upgrade path (initial release)
- Configuration format is stable and backward compatible
- Flash storage structure is finalized

### Future Upgrades
- Configuration data will be preserved across firmware updates
- BLE service UUIDs will remain stable
- Hardware connections will not change

---

**Note**: This is the initial release. Future versions will include detailed upgrade instructions and migration guides.
