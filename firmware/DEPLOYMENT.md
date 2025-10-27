# Deployment Checklist

This checklist guides you through deploying the Pothole Detection System in a production environment.

## Pre-Deployment Phase

### Hardware Preparation

- [ ] **Arduino Giga R1 WiFi**
  - [ ] Board tested and verified working
  - [ ] Firmware uploaded and tested
  - [ ] Serial output verified
  - [ ] WiFi AP functionality confirmed
  - [ ] Web dashboard accessible

- [ ] **16 Sensor Nodes** (per node)
  - [ ] Arduino Nano 33 BLE tested
  - [ ] TFLuna LIDAR sensor connected
  - [ ] I2C communication verified
  - [ ] BLE advertising confirmed
  - [ ] Digital output pin tested
  - [ ] Firmware uploaded (when available)

- [ ] **Wiring and Connections**
  - [ ] All jumper wires prepared
  - [ ] Pin-to-pin mapping documented
  - [ ] Common ground verified
  - [ ] Wire lengths appropriate for installation
  - [ ] Connectors labeled for easy identification

- [ ] **Power Supply**
  - [ ] Appropriate voltage and current ratings
  - [ ] Power distribution planned
  - [ ] Backup power considered (optional)
  - [ ] Cables and connectors prepared

- [ ] **Enclosures**
  - [ ] Weatherproof enclosures for outdoor use
  - [ ] Ventilation for heat dissipation
  - [ ] Cable entry points sealed
  - [ ] Mounting hardware included

### Software Configuration

- [ ] **Central Controller**
  - [ ] WiFi AP SSID configured (if customized)
  - [ ] WiFi password set (if securing network)
  - [ ] BLE UUIDs verified against sensor nodes
  - [ ] Pin mappings double-checked
  - [ ] Serial logging configured

- [ ] **Sensor Nodes**
  - [ ] Each node has unique BLE name
  - [ ] Default zero reference set
  - [ ] Default threshold set (5cm)
  - [ ] I2C address conflicts resolved
  - [ ] Flash storage verified

- [ ] **Documentation**
  - [ ] Sensor location map created
  - [ ] Pin mapping documented
  - [ ] BLE device names recorded
  - [ ] IP addresses documented
  - [ ] WiFi credentials stored securely

### Testing

- [ ] **Individual Component Testing**
  - [ ] Each sensor node tested individually
  - [ ] Central controller tested with single sensor
  - [ ] BLE connectivity verified
  - [ ] Digital pin signaling confirmed
  - [ ] Web dashboard functional

- [ ] **System Integration Testing**
  - [ ] All 16 sensors connected
  - [ ] Simultaneous detections handled
  - [ ] Web dashboard shows all sensors
  - [ ] BLE polling works for all sensors
  - [ ] Configuration updates successful

- [ ] **Performance Testing**
  - [ ] Response time measured
  - [ ] False positive rate acceptable
  - [ ] Detection accuracy verified
  - [ ] System stability over 24 hours
  - [ ] Multiple client connections tested

## Deployment Phase

### Physical Installation

- [ ] **Site Survey**
  - [ ] Installation locations marked
  - [ ] Sensor spacing planned (R1-R8, L8-L1)
  - [ ] Central controller location selected
  - [ ] Power source identified
  - [ ] Environmental factors considered

- [ ] **Sensor Node Installation**
  - [ ] Nodes mounted securely
  - [ ] LIDAR sensors pointing down
  - [ ] Optimal height from road surface
  - [ ] Protection from direct exposure
  - [ ] Cables routed safely

- [ ] **Central Controller Installation**
  - [ ] Controller mounted in accessible location
  - [ ] WiFi coverage verified
  - [ ] Power connection secure
  - [ ] Ventilation adequate
  - [ ] Protection from weather

- [ ] **Wiring**
  - [ ] All connections made per mapping
  - [ ] Continuity tested for each wire
  - [ ] Common ground established
  - [ ] Cable management implemented
  - [ ] Connections secured and labeled

- [ ] **Power Up**
  - [ ] Central controller powered on
  - [ ] Serial output checked
  - [ ] WiFi AP broadcasting
  - [ ] All sensor nodes powered on
  - [ ] BLE devices advertising

### System Calibration

- [ ] **Zero Reference Calibration**
  For each sensor (R1-R8, L8-L1):
  - [ ] Access web dashboard
  - [ ] Poll sensor for current distance
  - [ ] Record distance value
  - [ ] Configure zero reference
  - [ ] Verify setting saved

- [ ] **Threshold Configuration**
  For each sensor:
  - [ ] Set initial threshold (e.g., 5cm)
  - [ ] Test with known depth objects
  - [ ] Adjust as needed
  - [ ] Document final values

- [ ] **Detection Testing**
  - [ ] Simulate pothole with test object
  - [ ] Verify digital pin goes HIGH
  - [ ] Check serial log output
  - [ ] Confirm dashboard shows alert
  - [ ] Test multiple sensors simultaneously

### Network Configuration

- [ ] **WiFi Access Point**
  - [ ] SSID visible from monitoring location
  - [ ] Connection tested from multiple devices
  - [ ] IP address accessible (192.168.4.1)
  - [ ] Signal strength adequate
  - [ ] Range sufficient for operation

- [ ] **Dashboard Access**
  - [ ] Dashboard loads on multiple browsers
  - [ ] All 16 sensors displayed correctly
  - [ ] Real-time updates working
  - [ ] Buttons functional (Poll, Configure)
  - [ ] Auto-refresh working

### Monitoring Setup

- [ ] **Serial Monitoring**
  - [ ] Serial connection established (if needed)
  - [ ] Logging enabled
  - [ ] Timestamps recording
  - [ ] Log file rotation configured (optional)

- [ ] **Dashboard Monitoring**
  - [ ] Dedicated device for monitoring (optional)
  - [ ] Dashboard bookmarked
  - [ ] Refresh interval verified
  - [ ] Alert notifications configured (if implemented)

## Post-Deployment Phase

### Verification

- [ ] **24-Hour Burn-In**
  - [ ] System running continuously
  - [ ] No unexpected errors
  - [ ] All sensors responsive
  - [ ] Power supply stable
  - [ ] Temperature within limits

- [ ] **Detection Accuracy**
  - [ ] Known potholes detected
  - [ ] False positive rate acceptable
  - [ ] Response time adequate
  - [ ] Multiple detections handled
  - [ ] Edge cases tested

- [ ] **Reliability Testing**
  - [ ] System uptime tracking
  - [ ] Sensor failure detection
  - [ ] BLE connection stability
  - [ ] WiFi AP reliability
  - [ ] Power interruption recovery

### Documentation

- [ ] **As-Built Documentation**
  - [ ] Actual sensor locations recorded
  - [ ] Final wiring diagram created
  - [ ] Calibration values documented
  - [ ] Configuration settings saved
  - [ ] Photos of installation taken

- [ ] **Operational Manual**
  - [ ] Startup procedure documented
  - [ ] Shutdown procedure documented
  - [ ] Troubleshooting guide created
  - [ ] Maintenance schedule defined
  - [ ] Contact information listed

- [ ] **Configuration Backup**
  - [ ] Sensor calibration values backed up
  - [ ] Code configuration saved
  - [ ] Network settings documented
  - [ ] Pin mappings recorded
  - [ ] BLE UUIDs documented

### Training

- [ ] **Operator Training**
  - [ ] Dashboard usage demonstrated
  - [ ] Polling procedure explained
  - [ ] Configuration updates taught
  - [ ] Log interpretation covered
  - [ ] Troubleshooting basics reviewed

- [ ] **Maintenance Training**
  - [ ] Hardware inspection procedure
  - [ ] Cleaning and maintenance schedule
  - [ ] Sensor replacement procedure
  - [ ] Firmware update process
  - [ ] Calibration procedure

## Maintenance Schedule

### Daily Checks

- [ ] Dashboard accessible
- [ ] All sensors showing in dashboard
- [ ] No persistent error messages
- [ ] WiFi AP broadcasting

### Weekly Checks

- [ ] Poll each sensor for functionality
- [ ] Check for accumulated dirt/debris
- [ ] Verify all connections tight
- [ ] Review event logs
- [ ] Check power supply voltage

### Monthly Checks

- [ ] Comprehensive sensor testing
- [ ] Recalibrate if drift detected
- [ ] Physical inspection of installations
- [ ] Clean sensor optics
- [ ] Verify weatherproofing intact

### Quarterly Checks

- [ ] Full system diagnostic
- [ ] Firmware updates if available
- [ ] Deep clean all components
- [ ] Replace any worn cables
- [ ] Update documentation

## Troubleshooting Guide

### Sensor Not Responding

**Symptoms**: Sensor doesn't show in poll, or always shows LOW

**Checks**:
- [ ] Sensor node powered on
- [ ] BLE advertising (check with phone)
- [ ] Wiring connected correctly
- [ ] Pin number correct in code
- [ ] Sensor within BLE range

**Solutions**:
- [ ] Restart sensor node
- [ ] Check/replace wiring
- [ ] Verify pin configuration
- [ ] Move closer if out of range

### False Positives

**Symptoms**: Sensor triggers without actual pothole

**Checks**:
- [ ] Threshold too sensitive
- [ ] Environmental interference
- [ ] Sensor alignment
- [ ] Vibration or movement

**Solutions**:
- [ ] Increase threshold value
- [ ] Recalibrate zero reference
- [ ] Secure sensor mounting
- [ ] Shield from wind/debris

### WiFi Issues

**Symptoms**: Can't connect to AP or dashboard

**Checks**:
- [ ] Central controller powered
- [ ] Serial shows AP started
- [ ] WiFi SSID visible
- [ ] Correct IP address (192.168.4.1)

**Solutions**:
- [ ] Restart central controller
- [ ] Check serial for errors
- [ ] Verify WiFi antenna clear
- [ ] Try different device to connect

### BLE Connection Failures

**Symptoms**: Poll/configure fails for sensor

**Checks**:
- [ ] Sensor BLE advertising
- [ ] Device name matches
- [ ] UUIDs correct
- [ ] Within range (< 10m typically)

**Solutions**:
- [ ] Restart sensor node
- [ ] Verify BLE device name
- [ ] Move devices closer
- [ ] Check for interference

## Emergency Procedures

### System Shutdown

1. [ ] Note current system state
2. [ ] Save any configuration changes
3. [ ] Power off sensor nodes
4. [ ] Power off central controller
5. [ ] Document reason for shutdown

### System Restart

1. [ ] Verify power connections
2. [ ] Power on central controller
3. [ ] Wait for WiFi AP (30 seconds)
4. [ ] Power on sensor nodes
5. [ ] Verify all sensors responding
6. [ ] Check dashboard functionality

### Sensor Replacement

1. [ ] Document which sensor (position)
2. [ ] Power off old sensor
3. [ ] Disconnect wiring
4. [ ] Install new sensor
5. [ ] Connect wiring per mapping
6. [ ] Power on new sensor
7. [ ] Recalibrate zero and threshold
8. [ ] Test detection
9. [ ] Update documentation

## Performance Metrics

Track these metrics for system health:

- [ ] **Uptime**: Total system operational time
- [ ] **Detection Count**: Number of potholes detected
- [ ] **False Positive Rate**: Incorrect detections
- [ ] **Sensor Availability**: % of time each sensor responsive
- [ ] **BLE Success Rate**: % of successful BLE connections
- [ ] **WiFi Uptime**: % of time dashboard accessible

## Support Contacts

Document key contacts for support:

- [ ] Hardware supplier
- [ ] Software developer
- [ ] Local technical support
- [ ] Emergency contact
- [ ] Project manager

## Sign-Off

### Deployment Completed By

- **Name**: _____________________
- **Date**: _____________________
- **Signature**: _____________________

### Verified By

- **Name**: _____________________
- **Date**: _____________________
- **Signature**: _____________________

### Notes

Use this space for deployment-specific notes, observations, or lessons learned:

```
_________________________________________________________________
_________________________________________________________________
_________________________________________________________________
_________________________________________________________________
_________________________________________________________________
```

---

**Congratulations on completing your deployment!**

For ongoing support, refer to:
- [Quick Start Guide](QUICKSTART.md)
- [Installation Guide](INSTALLATION.md)
- [Architecture Documentation](ARCHITECTURE.md)
- [Examples and API Usage](EXAMPLES.md)
