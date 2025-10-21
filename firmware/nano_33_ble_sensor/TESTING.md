# Testing and Validation Guide

## Overview
This document provides comprehensive testing procedures for the Arduino Nano 33 BLE sensor nodes to ensure proper functionality before deployment.

## Testing Levels

1. **Component Testing**: Individual component verification
2. **Unit Testing**: Single sensor node functionality
3. **Integration Testing**: Sensor node with Giga R1 controller
4. **System Testing**: All 16 sensors in complete system
5. **Field Testing**: Real-world validation

## Pre-Deployment Testing Checklist

### Hardware Verification

#### Visual Inspection
- [ ] Arduino Nano 33 BLE is not damaged
- [ ] TFLuna LIDAR sensor is not damaged
- [ ] All pins are straight and not bent
- [ ] No visible solder bridges or cold joints
- [ ] Cable insulation is intact
- [ ] Connectors are properly seated

#### Power Testing
- [ ] Measure 5V at Arduino 5V pin
- [ ] Measure 5V at TFLuna VCC pin
- [ ] No short circuit between VCC and GND
- [ ] Arduino LED indicator lights up when powered
- [ ] TFLuna LED indicator lights up (if equipped)

#### Connectivity Testing
- [ ] I2C connections verified (SDA, SCL)
- [ ] Output pin connection verified (Pin 2)
- [ ] Ground connections verified
- [ ] All connections have continuity
- [ ] No unintended connections (shorts)

### Firmware Verification

#### Upload Test
- [ ] Firmware compiles without errors
- [ ] Firmware uploads successfully
- [ ] No upload errors or warnings
- [ ] Board responds after upload
- [ ] Reset button works properly

#### Serial Communication
- [ ] Serial Monitor connects at 115200 baud
- [ ] Initialization messages appear
- [ ] Sensor position is correctly displayed
- [ ] BLE device name is correctly displayed
- [ ] No error messages in serial output
- [ ] Serial output is continuous and stable

Expected Serial Output:
```
=== Arduino Nano 33 BLE Sensor Node ===
Sensor Position: [POSITION]
Configuration loaded from flash:
  Zero Reference: 0 cm
  Threshold: 5 cm
BLE initialized. Device name: NanoTFluna-[POSITION]
Advertising started.
Setup complete. Starting main loop...
```

### TFLuna LIDAR Testing

#### I2C Communication
- [ ] TFLuna responds to I2C requests
- [ ] I2C scanner detects device at 0x10
- [ ] No I2C communication errors in Serial Monitor
- [ ] Distance readings are being received

#### Distance Measurement
- [ ] Distance values are reasonable (not -1 or 0)
- [ ] Distance changes when object is moved
- [ ] Readings are stable (not erratic)
- [ ] Range is within 20cm to 800cm (TFLuna spec)
- [ ] Accuracy is within ±2cm at short range (<1m)

Test Procedure:
1. Hold flat object at 50cm from sensor
2. Verify reading is approximately 50cm (±5cm)
3. Move object to 100cm
4. Verify reading is approximately 100cm (±5cm)
5. Move object closer (30cm)
6. Verify reading tracks movement

### BLE Functionality Testing

#### BLE Advertising
- [ ] BLE device appears in scan (use nRF Connect app)
- [ ] Device name matches expected format: NanoTFluna-[POSITION]
- [ ] Device is consistently discoverable
- [ ] RSSI signal strength is appropriate (-40 to -70 dBm at 1m)

#### BLE Connection
- [ ] Can connect to device from BLE client
- [ ] Connection is stable (no random disconnects)
- [ ] Service UUID is correct
- [ ] All characteristics are visible

#### Characteristics Testing

**Distance Characteristic (12340001-...)**
- [ ] Can read distance value
- [ ] Distance value matches Serial Monitor output
- [ ] Notifications work (value updates automatically)
- [ ] Update rate is approximately 10Hz

**Zero Reference Characteristic (12340002-...)**
- [ ] Can read current zero reference value
- [ ] Can write new zero reference value
- [ ] Written value is confirmed in Serial Monitor
- [ ] Value persists after power cycle

**Threshold Characteristic (12340003-...)**
- [ ] Can read current threshold value
- [ ] Can write new threshold value
- [ ] Written value is confirmed in Serial Monitor
- [ ] Value persists after power cycle

**Status Characteristic (12340004-...)**
- [ ] Can read status string
- [ ] Status updates when configuration changes
- [ ] Status string is readable text

### Pothole Detection Testing

#### Basic Detection
- [ ] Detection threshold is correctly set (default 5cm)
- [ ] Zero reference is correctly set
- [ ] Detection logic responds to depth changes

Test Procedure:
1. Set zero reference to current flat surface distance
2. Set threshold to 5cm
3. Lift sensor 10cm (or lower surface)
4. Verify "POTHOLE DETECTED" in Serial Monitor
5. Verify calculation shows correct depth
6. Lower sensor back to reference
7. Detection should stop

#### Output Pin Behavior
- [ ] Pin 2 goes HIGH when pothole detected
- [ ] Pin 2 stays HIGH for approximately 2 seconds
- [ ] Pin 2 returns to LOW after timeout
- [ ] Timing is consistent across multiple detections
- [ ] Multiple rapid detections handled correctly

Test with LED:
1. Connect LED (+ 220Ω resistor) to Pin 2
2. Trigger detection
3. LED lights up immediately
4. LED stays on for 2 seconds
5. LED turns off automatically

Test with Multimeter:
1. Set multimeter to DC voltage mode
2. Connect to Pin 2 and GND
3. Trigger detection
4. Verify ~3.3V during HIGH state
5. Verify ~0V during LOW state

### Configuration Persistence Testing

#### Flash Storage
- [ ] Configuration survives power cycle
- [ ] Zero reference persists after reset
- [ ] Threshold persists after reset
- [ ] Invalid configuration is detected and defaults applied

Test Procedure:
1. Set zero reference to 100 via BLE
2. Set threshold to 10 via BLE
3. Verify values in Serial Monitor
4. Disconnect power completely
5. Reconnect power
6. Check Serial Monitor for loaded values
7. Verify zero reference is 100
8. Verify threshold is 10

### Stress Testing

#### Continuous Operation
- [ ] Runs continuously for 1 hour without errors
- [ ] No memory leaks (consistent operation)
- [ ] No random resets or crashes
- [ ] BLE remains stable
- [ ] Distance readings remain accurate

#### Rapid Detection Events
- [ ] Can handle multiple rapid detections
- [ ] Output pin timing is maintained
- [ ] No missed detections
- [ ] System remains responsive

Test Procedure:
1. Trigger 10 detections in quick succession
2. Verify each detection is logged
3. Verify output pin behavior for each
4. Check for any errors or warnings

#### BLE Connection Stability
- [ ] Can disconnect and reconnect multiple times
- [ ] Configuration changes during connection work
- [ ] No crashes on unexpected disconnect
- [ ] Continues operating if BLE connection drops

### Environmental Testing (Optional for Production)

#### Temperature Range
- [ ] Operates correctly at 0°C
- [ ] Operates correctly at 50°C
- [ ] BLE range is stable across temperature
- [ ] LIDAR accuracy maintained

#### Vibration
- [ ] Connections remain secure with vibration
- [ ] No intermittent failures
- [ ] Mounting is stable

#### Moisture (if applicable)
- [ ] Enclosure is sealed properly
- [ ] No condensation inside enclosure
- [ ] All connections protected

## Integration Testing with Giga R1

### Digital Signal Connection
- [ ] Pin 2 output reaches Giga R1 input
- [ ] Signal level is sufficient (3.3V is readable by Giga)
- [ ] Common ground established
- [ ] No ground loops

Test Procedure:
1. Connect Arduino Pin 2 to Giga input (correct pin for position)
2. Connect Arduino GND to Giga GND
3. Trigger detection on sensor node
4. Verify Giga R1 reads HIGH on corresponding input
5. Verify Giga R1 reads LOW after 2 seconds

### BLE Communication
- [ ] Giga R1 can discover sensor node
- [ ] Giga R1 can connect to sensor node
- [ ] Giga R1 can read distance
- [ ] Giga R1 can write configuration
- [ ] Multiple connections work (sequential)

### Complete Node Testing
- [ ] Both digital and BLE communication work simultaneously
- [ ] Detection triggers digital output
- [ ] BLE notifications continue during detection
- [ ] Configuration via BLE affects detection behavior

## System Testing (All 16 Sensors)

### Installation Verification
- [ ] All 16 sensors powered on
- [ ] All 16 sensors appear in BLE scan
- [ ] Each sensor has unique name (R1-R8, L1-L8)
- [ ] All digital outputs connected to correct Giga pins

### Simultaneous Operation
- [ ] All sensors measure continuously
- [ ] No interference between sensors
- [ ] BLE communication works for all
- [ ] Digital signals are distinct (no crosstalk)

### Sequential Testing
Test each sensor individually:
- [ ] R1: Detection works, correct Giga pin (23), correct BLE name
- [ ] R2: Detection works, correct Giga pin (25), correct BLE name
- [ ] R3: Detection works, correct Giga pin (27), correct BLE name
- [ ] R4: Detection works, correct Giga pin (29), correct BLE name
- [ ] R5: Detection works, correct Giga pin (31), correct BLE name
- [ ] R6: Detection works, correct Giga pin (33), correct BLE name
- [ ] R7: Detection works, correct Giga pin (35), correct BLE name
- [ ] R8: Detection works, correct Giga pin (37), correct BLE name
- [ ] L8: Detection works, correct Giga pin (39), correct BLE name
- [ ] L7: Detection works, correct Giga pin (41), correct BLE name
- [ ] L6: Detection works, correct Giga pin (43), correct BLE name
- [ ] L5: Detection works, correct Giga pin (45), correct BLE name
- [ ] L4: Detection works, correct Giga pin (47), correct BLE name
- [ ] L3: Detection works, correct Giga pin (49), correct BLE name
- [ ] L2: Detection works, correct Giga pin (51), correct BLE name
- [ ] L1: Detection works, correct Giga pin (53), correct BLE name

### Calibration Verification
- [ ] All sensors calibrated to same reference surface
- [ ] Zero references recorded for each sensor
- [ ] Thresholds set appropriately for all sensors
- [ ] Configuration saved and verified

## Field Testing

### Real-World Conditions
- [ ] Sensors respond to actual road surface
- [ ] Detections occur at real potholes
- [ ] False positives are minimal
- [ ] Detection sensitivity is appropriate

### Long-Term Reliability
- [ ] System operates continuously for 24 hours
- [ ] No sensor failures
- [ ] Configuration remains stable
- [ ] BLE connectivity maintained

### Data Validation
- [ ] Distance measurements are reasonable for road application
- [ ] Detection events correlate with actual potholes
- [ ] Timing of detections is accurate
- [ ] Giga R1 correctly logs all events

## Troubleshooting During Testing

### Common Issues and Solutions

**No Serial Output**
- Check Serial Monitor baud rate (115200)
- Press reset button
- Check USB connection
- Try different USB cable

**TFLuna Not Responding**
- Check I2C wiring (SDA, SCL)
- Verify power to TFLuna (5V)
- Run I2C scanner to verify address
- Check for loose connections

**BLE Not Advertising**
- Verify ArduinoBLE library installed
- Check for error messages in Serial Monitor
- Reset the board
- Verify "Arduino Nano 33 BLE" board selected

**Incorrect Detection**
- Verify zero reference is set correctly
- Check threshold value
- Confirm LIDAR has clear view
- Test with known distances

**Output Pin Not Working**
- Test with LED instead of Giga connection
- Verify detection is occurring (Serial Monitor)
- Check wiring to Pin 2
- Measure voltage with multimeter

**Configuration Not Saving**
- Verify FlashStorage library installed
- Check for errors in Serial Monitor
- Ensure proper write confirmation
- Test with power cycle

## Test Documentation

### Test Record Template

For each sensor, record:
```
Sensor Position: _______
Board Serial Number: _______
Firmware Version: _______
Test Date: _______

Hardware Tests: PASS / FAIL
Firmware Tests: PASS / FAIL
BLE Tests: PASS / FAIL
Detection Tests: PASS / FAIL
Integration Tests: PASS / FAIL

Zero Reference: _______ cm
Threshold: _______ cm

Notes:
_________________________________
_________________________________

Tester Signature: _____________
```

### Batch Testing Spreadsheet

Create spreadsheet with columns:
- Position (R1-R8, L1-L8)
- BLE Name Verified (Y/N)
- Distance Reading (Y/N)
- Detection Works (Y/N)
- Output Pin Works (Y/N)
- Giga Pin Verified (Y/N)
- Config Persists (Y/N)
- Overall Status (PASS/FAIL)
- Notes

## Performance Benchmarks

### Expected Performance

| Metric | Expected Value | Tolerance |
|--------|----------------|-----------|
| Measurement Rate | 10 Hz | ±1 Hz |
| Detection Latency | <100 ms | ±20 ms |
| Output Duration | 2000 ms | ±50 ms |
| BLE Connection Time | <2 s | ±0.5 s |
| Distance Accuracy | ±2 cm | @<3m range |
| Power Consumption | ~200 mA | ±50 mA |
| BLE Range | >10 m | Line of sight |

### Acceptance Criteria

A sensor node passes testing if:
- ✅ All hardware tests pass
- ✅ Firmware uploads and runs without errors
- ✅ TFLuna provides stable distance readings
- ✅ BLE is discoverable with correct name
- ✅ All BLE characteristics work correctly
- ✅ Pothole detection logic functions properly
- ✅ Output pin timing is correct
- ✅ Configuration persists in flash
- ✅ Runs continuously for minimum 1 hour
- ✅ Integration with Giga R1 is verified

## Final Pre-Deployment Checklist

Before deploying to field:
- [ ] All 16 sensors have passed individual testing
- [ ] All sensors have unique, correct BLE names
- [ ] All digital outputs connect to correct Giga pins
- [ ] System integration testing complete
- [ ] Calibration performed and recorded
- [ ] All sensors labeled with position
- [ ] Backup configuration data recorded
- [ ] Enclosures sealed and weatherproofed (if applicable)
- [ ] Test documentation complete
- [ ] Team trained on system operation

---

**Note**: Keep detailed test records for troubleshooting and warranty purposes. Document any deviations from expected performance.
