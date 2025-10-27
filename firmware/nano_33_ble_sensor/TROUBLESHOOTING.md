# Troubleshooting Guide

This guide provides systematic approaches to diagnose and fix common issues with the Arduino Nano 33 BLE sensor nodes.

## Quick Diagnostic Flowchart

```
Problem?
   │
   ├─→ Can't upload firmware?
   │      └─→ See "Upload Issues" section
   │
   ├─→ No serial output?
   │      └─→ See "Serial Communication" section
   │
   ├─→ TFLuna not responding?
   │      └─→ See "LIDAR Issues" section
   │
   ├─→ BLE not working?
   │      └─→ See "BLE Issues" section
   │
   ├─→ Detection not working?
   │      └─→ See "Detection Issues" section
   │
   └─→ Output pin not triggering?
          └─→ See "Output Signal Issues" section
```

## Upload Issues

### Problem: Can't Upload Firmware

#### Symptom
- Arduino IDE shows upload errors
- "Port not found" error
- "Device not responding" error

#### Solutions

**1. Check USB Connection**
```
✓ Try different USB cable (some are charge-only)
✓ Try different USB port on computer
✓ Check if USB port provides data (not just power)
✓ Look for Arduino LED indicator (should light up)
```

**2. Check Board Selection**
```
Arduino IDE → Tools → Board
  ✓ Must be "Arduino Nano 33 BLE" (not "Arduino Nano")
  ✓ From "Arduino Mbed OS Nano Boards" package
  ✗ NOT "Arduino AVR Boards" Nano
```

**3. Check Port Selection**
```
Arduino IDE → Tools → Port
  Windows: Look for "COM3" or higher with "(Arduino Nano 33 BLE)"
  Mac: Look for "/dev/cu.usbmodem*"
  Linux: Look for "/dev/ttyACM0"
```

**4. Enter Bootloader Mode**
```
On Arduino Nano 33 BLE:
  1. Press RESET button twice quickly (double-click)
  2. Orange LED should pulse (bootloader mode)
  3. Port should appear in Arduino IDE
  4. Try upload immediately
```

**5. Check Drivers (Windows)**
```
Device Manager → Ports (COM & LPT)
  ✓ Should see "USB Serial Device" or "Arduino Nano 33 BLE"
  ✗ If yellow triangle: Update/reinstall driver
```

**6. Verify Board Support**
```
Tools → Board → Boards Manager
  Search: "Arduino Mbed OS Nano Boards"
  ✓ Should be installed (green checkmark)
  Version: 2.0.0 or later
```

### Problem: Upload Works But Program Won't Run

#### Symptom
- Upload completes successfully
- No serial output
- No LED activity

#### Solutions

**1. Check Power**
```
✓ Verify 5V power supply is connected
✓ Check current supply (minimum 200mA)
✓ Measure voltage at 5V pin (should be 4.75-5.25V)
```

**2. Reset After Upload**
```
1. Press RESET button once after upload
2. Wait 2 seconds
3. Check serial output
```

**3. Verify Firmware**
```
✓ Re-upload firmware
✓ Check for compilation errors
✓ Ensure all libraries are installed
```

## Serial Communication Issues

### Problem: No Serial Output

#### Symptom
- Serial Monitor is blank
- No initialization messages
- No errors displayed

#### Solutions

**1. Check Baud Rate**
```
Serial Monitor (bottom right):
  ✓ Must be set to 115200 baud
  ✗ Common mistake: 9600 (default)
```

**2. Check Serial Monitor Settings**
```
✓ "Both NL & CR" or "Newline" line ending
✓ Correct port selected
✓ Monitor opened AFTER upload (or press RESET)
```

**3. Close Other Programs**
```
✓ Close any other serial monitors
✓ Close Arduino Serial Plotter
✓ Close other Arduino IDE instances
✓ Check for Python scripts using the port
```

**4. Reset Board**
```
1. Press RESET button
2. Wait 3 seconds
3. Check for boot messages
```

**5. Test with Minimal Sketch**
```cpp
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Test");
}
void loop() {
  Serial.println("Loop");
  delay(1000);
}
```

### Problem: Garbled Serial Output

#### Symptom
- Random characters displayed
- Unreadable text

#### Solution
```
✓ Verify 115200 baud rate
✓ Check USB cable quality
✓ Try different USB port
✓ Close other serial applications
```

## LIDAR Issues

### Problem: TFLuna Not Responding

#### Symptom
- Serial shows "Error: Failed to communicate with TFLuna"
- Distance always -1
- No distance readings

#### Diagnostic Steps

**1. Verify I2C Connections**
```
TFLuna → Arduino
  VCC (Red)    → 5V
  GND (Black)  → GND
  SDA (Green)  → SDA (A4)
  SCL (Blue)   → SCL (A5)

✓ Check for loose connections
✓ Verify correct pin assignments
✓ Ensure good contact (no cold solder joints)
```

**2. Check Power Supply**
```
Use multimeter:
  ✓ Measure 5V between TFLuna VCC and GND
  ✓ Voltage should be 4.75-5.25V
  ✓ Check for voltage drop under load
  ✓ Verify sufficient current (150mA for TFLuna)
```

**3. Run I2C Scanner**
```cpp
// Upload this test sketch
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial);
  Serial.println("I2C Scanner");
}

void loop() {
  byte count = 0;
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(i, HEX);
      count++;
    }
  }
  if (count == 0) Serial.println("No I2C devices found");
  delay(5000);
}
```

**Expected Result:**
```
Found device at 0x10    ← TFLuna should appear here
```

**4. Check TFLuna Configuration**
```
TFLuna must be in I2C mode (not UART mode)
  ✓ Default is I2C mode for most TFLuna sensors
  ✓ I2C address should be 0x10
  ✗ If in UART mode, won't respond to I2C
```

**5. Test with Known Good Sensor**
```
If available:
  1. Swap TFLuna with known working unit
  2. If new sensor works, original sensor faulty
  3. If new sensor also fails, check Arduino/wiring
```

### Problem: Erratic Distance Readings

#### Symptom
- Distance jumps around erratically
- Readings don't match actual distance
- Inconsistent measurements

#### Solutions

**1. Check Physical Obstacles**
```
✓ Ensure clear line of sight to target
✓ Remove any obstructions in front of sensor
✓ Clean TFLuna lens (gentle wipe with soft cloth)
✓ Target should be non-reflective and flat
```

**2. Check I2C Cable Length**
```
✓ Keep I2C wires short (<30cm)
✓ Use twisted pair for SDA/SCL
✓ Add external pull-up resistors (4.7kΩ) if long cables
```

**3. Check Power Stability**
```
✓ Add 100µF capacitor across VCC/GND near TFLuna
✓ Use separate power supply if current insufficient
✓ Check for voltage drops during operation
```

**4. Environmental Factors**
```
✓ Avoid direct sunlight on sensor
✓ Keep sensor at stable temperature
✓ Avoid highly reflective or transparent targets
✓ Ensure target is within 0.2m to 8m range
```

## BLE Issues

### Problem: BLE Not Advertising

#### Symptom
- Serial shows "BLE initialized" but device not found
- Cannot scan/discover device with phone
- nRF Connect doesn't show device

#### Solutions

**1. Verify BLE Initialization**
```
Check serial output for:
  ✓ "BLE initialized. Device name: NanoTFluna-XX"
  ✓ "Advertising started."
  ✗ Any error messages

If error messages:
  → ArduinoBLE library may not be installed
  → Re-install ArduinoBLE library
```

**2. Check Board Type**
```
✓ Must use Arduino Nano 33 BLE (has built-in BLE)
✗ Regular Arduino Nano does NOT have BLE
✗ Other Arduino boards may not support ArduinoBLE
```

**3. Restart BLE Scanner**
```
On phone/tablet:
  1. Close nRF Connect (or other BLE app)
  2. Toggle Bluetooth OFF
  3. Toggle Bluetooth ON
  4. Reopen BLE app
  5. Start fresh scan
```

**4. Reset Arduino**
```
1. Press RESET button on Arduino
2. Wait 5 seconds for full initialization
3. Scan again with BLE app
```

**5. Check BLE Name**
```
Serial output shows:
  "Device name: NanoTFluna-R1"  ← This should match scan
  
In BLE scanner, look for:
  ✓ "NanoTFluna-R1" (exact match)
  ✓ Check for typos in firmware
```

### Problem: Can't Connect to BLE Device

#### Symptom
- Device appears in scan
- Connection fails immediately
- "Connection timeout" error

#### Solutions

**1. Check Distance**
```
✓ Move phone closer to Arduino (<2m)
✓ Ensure line of sight (no walls/obstacles)
✓ Typical BLE range: 10m, but can be less
```

**2. Forget and Reconnect**
```
In BLE app:
  1. If device was previously connected, "forget" it
  2. Restart Arduino (RESET button)
  3. Fresh scan and connect
```

**3. Check for Multiple Connections**
```
✓ Disconnect any other BLE devices
✓ Arduino can only handle 1 BLE connection at a time
✓ Close other apps that might be connecting
```

**4. Restart Bluetooth**
```
On phone:
  Settings → Bluetooth → OFF → ON
```

### Problem: Can't Read/Write BLE Characteristics

#### Symptom
- Connected successfully
- Can see service/characteristics
- Read/Write operations fail

#### Solutions

**1. Verify Service UUID**
```
Service UUID should be:
  12340000-1234-5678-9abc-def012345678

Check in nRF Connect:
  ✓ Expand "Unknown Service" or service with this UUID
  ✓ Should see 4 characteristics
```

**2. Check Characteristic Properties**
```
Distance:       READ + NOTIFY
Zero Ref:       READ + WRITE
Threshold:      READ + WRITE
Status:         READ

✓ Can only WRITE to characteristics with WRITE property
✓ Can only READ from characteristics with READ property
```

**3. Check Data Format**
```
For Int16 characteristics (Distance, Zero Ref, Threshold):
  ✓ Value should be 2 bytes (16-bit integer)
  ✓ Little-endian byte order
  ✓ Signed integer (-32768 to 32767)

Example: Writing 50 (decimal)
  Hex: 0x0032
  Bytes: [0x32, 0x00] (little-endian)
```

**4. Verify Arduino is Running**
```
✓ Check serial monitor for activity
✓ Arduino should be continuously measuring
✓ No errors in serial output
```

## Detection Issues

### Problem: No Pothole Detection

#### Symptom
- Distance readings look correct
- No "POTHOLE DETECTED" messages
- Output pin never goes HIGH

#### Diagnostic Steps

**1. Check Configuration Values**
```
Serial output shows:
  Zero Reference: _____ cm
  Threshold: _____ cm

Verify:
  ✓ Zero reference is set (not 0 unless intended)
  ✓ Threshold is reasonable (5-10 cm typical)
  ✓ Values make sense for your setup
```

**2. Test Detection Logic**
```
Manual test:
  1. Note current distance (e.g., 50 cm)
  2. Set zero reference to current distance via BLE
  3. Set threshold to 5 cm
  4. Move sensor up 10 cm (or target down)
  5. Should trigger "POTHOLE DETECTED"

If this works:
  → Detection logic is OK
  → May need to calibrate zero reference
```

**3. Verify Detection Math**
```
Detection formula:
  depth = currentDistance - zeroReference
  if (depth > threshold) → DETECTION

Example:
  zeroReference = 50 cm
  threshold = 5 cm
  currentDistance = 60 cm
  depth = 60 - 50 = 10 cm
  10 > 5 → DETECTION ✓

Check your values match this logic
```

**4. Check for Invalid Readings**
```
If currentDistance = -1:
  → TFLuna communication error
  → Detection skipped for this cycle
  → Fix TFLuna issues first
```

### Problem: False Positives (Detects When It Shouldn't)

#### Symptom
- Detection triggers inappropriately
- Too sensitive

#### Solutions

**1. Increase Threshold**
```
Via BLE:
  1. Read current threshold
  2. Increase by 2-3 cm
  3. Write new threshold
  4. Test again

Typical values:
  5 cm  → High sensitivity
  8 cm  → Medium sensitivity
  10 cm → Low sensitivity
```

**2. Calibrate Zero Reference**
```
1. Place sensor over level surface (no pothole)
2. Read current distance
3. Set zero reference to this value
4. This establishes baseline
```

**3. Check for Vibration**
```
✓ Secure sensor mounting
✓ Reduce mechanical vibration
✓ Avoid loose connections
```

## Output Signal Issues

### Problem: Output Pin Not Triggering

#### Symptom
- Detection occurs (serial shows "POTHOLE DETECTED")
- Pin 2 never goes HIGH
- Giga R1 doesn't receive signal

#### Diagnostic Steps

**1. Test with LED**
```
Connect test LED:
  Pin 2 → LED (+) → 220Ω resistor → GND

Trigger detection:
  ✓ LED should light up
  ✓ Stay lit for 2 seconds
  ✓ Turn off automatically
```

**2. Test with Multimeter**
```
Set multimeter to DC voltage:
  Red probe → Pin 2
  Black probe → GND

Trigger detection:
  ✓ Should read ~3.3V when HIGH
  ✓ Should read ~0V when LOW
  ✓ Duration ~2 seconds
```

**3. Check Code**
```
Verify in firmware:
  ✓ #define OUTPUT_PIN 2
  ✓ pinMode(OUTPUT_PIN, OUTPUT) in setup()
  ✓ digitalWrite(OUTPUT_PIN, HIGH) on detection
  ✓ digitalWrite(OUTPUT_PIN, LOW) after timeout
```

**4. Check Wiring to Giga**
```
Arduino Pin 2 → Giga input pin (see mapping)
Arduino GND → Giga GND (common ground!)

Verify:
  ✓ Correct Giga pin for sensor position
  ✓ Common ground established
  ✓ No loose connections
```

### Problem: Output Stays HIGH

#### Symptom
- Pin 2 goes HIGH
- Never returns to LOW
- Stays HIGH indefinitely

#### Solution

**1. Check Timing Logic**
```
Serial monitor should show:
  "POTHOLE DETECTED" → Pin goes HIGH
  (2 seconds later)
  "Output pin reset to LOW" → Pin goes LOW

If "reset" message never appears:
  → Timing logic issue
  → Re-upload firmware
```

**2. Verify No Continuous Detection**
```
If pothole is still "detected":
  → Pin stays HIGH because depth > threshold
  → Move sensor away from detection point
  → Or adjust threshold
```

## Configuration Persistence Issues

### Problem: Configuration Not Saving

#### Symptom
- Set values via BLE
- Power cycle Arduino
- Values reset to defaults

#### Solutions

**1. Verify Save Confirmation**
```
Serial monitor should show:
  "Zero reference updated to: XX cm"
  "Configuration saved to flash."

If no "saved" message:
  → Write may not have occurred
  → Try writing again via BLE
```

**2. Check FlashStorage Library**
```
Arduino IDE:
  Tools → Manage Libraries
  Search: "FlashStorage"
  ✓ Should be installed by Arduino
  Version: 1.0.0 or later
```

**3. Test Persistence**
```
1. Set zero reference to 100 via BLE
2. Verify serial shows "saved to flash"
3. Disconnect power completely (unplug USB)
4. Wait 10 seconds
5. Reconnect power
6. Check serial output on boot
7. Should show "Zero Reference: 100 cm"
```

**4. Factory Reset**
```
If configuration seems corrupted:
  1. Upload firmware again (fresh upload)
  2. First boot will detect invalid config
  3. Will load defaults
  4. Saves defaults to flash
  5. Reconfigure via BLE
```

## System Integration Issues

### Problem: Giga R1 Not Receiving Signal

#### Symptom
- Sensor detects correctly (serial confirms)
- Pin 2 is HIGH (tested with LED/multimeter)
- Giga doesn't register event

#### Solutions

**1. Verify Pin Mapping**
```
Check correct Giga input pin:
  R1 → Pin 23    R5 → Pin 31
  R2 → Pin 25    R6 → Pin 33
  R3 → Pin 27    R7 → Pin 35
  R4 → Pin 29    R8 → Pin 37
  L8 → Pin 39    L4 → Pin 47
  L7 → Pin 41    L3 → Pin 49
  L6 → Pin 43    L2 → Pin 51
  L5 → Pin 45    L1 → Pin 53

✓ Verify sensor position matches pin
```

**2. Check Common Ground**
```
CRITICAL:
  ✓ Arduino GND must connect to Giga GND
  ✗ Without common ground, signal won't work
  
Test:
  Multimeter continuity between Arduino GND and Giga GND
  Should beep/show 0Ω
```

**3. Check Logic Levels**
```
Arduino outputs 3.3V HIGH
Giga accepts 3.3V as HIGH (5V tolerant)

✓ No level shifter needed
✓ Direct connection OK
```

**4. Test Giga Input**
```
On Giga:
  1. Configure pin as INPUT
  2. Enable pull-down resistor
  3. Read digital input
  4. Should read HIGH when sensor triggers
```

## Multiple Sensor Issues

### Problem: BLE Name Conflicts

#### Symptom
- Multiple sensors with same name
- Can't distinguish sensors

#### Solution

**1. Verify Unique Configuration**
```
Each sensor must have unique SENSOR_POSITION:
  ✓ R1, R2, R3, ... R8
  ✓ L1, L2, L3, ... L8

Check firmware:
  #define SENSOR_POSITION "R1"  ← Must be unique

Use pre-configured examples to avoid errors
```

**2. Label Physical Boards**
```
✓ Mark each board with position (R1, R2, etc.)
✓ Prevents confusion during deployment
✓ Match label to BLE name
```

### Problem: Interference Between Sensors

#### Symptom
- Sensors affect each other
- Inconsistent readings when multiple powered on

#### Solutions

**1. Verify Separate I2C Buses**
```
✓ Each sensor has its own TFLuna
✓ Each sensor has its own I2C bus
✓ No sensors share I2C connections
✗ DO NOT connect multiple TFLuna to one I2C bus
```

**2. Check Power Supply**
```
✓ Adequate current for all sensors (16 × 200mA = 3.2A)
✓ No voltage drops under load
✓ Individual power filtering (100µF caps)
```

**3. BLE Interference**
```
✓ BLE should not interfere (different frequencies)
✓ Each sensor has unique BLE address
✓ Sequential connection OK (not simultaneous)
```

## Emergency Recovery

### Problem: Arduino Won't Respond At All

#### Last Resort Solutions

**1. Bootloader Mode Recovery**
```
1. Disconnect everything except USB
2. Press RESET twice quickly (double-click)
3. Orange LED should pulse
4. Upload known-good firmware immediately
```

**2. Factory Reset via Re-upload**
```
1. Upload blank sketch (just empty setup/loop)
2. Wait 5 seconds
3. Upload main firmware again
4. Configuration will reset to defaults
```

**3. Hardware Reset**
```
If bootloader unresponsive:
  1. Disconnect all peripherals
  2. Disconnect power
  3. Wait 30 seconds
  4. Connect USB only
  5. Try bootloader mode again
```

## Getting Additional Help

### Information to Collect

When seeking help, provide:
```
1. Serial Monitor output (full text)
2. Firmware version / exact .ino file used
3. Board type (Arduino Nano 33 BLE)
4. Library versions (ArduinoBLE, FlashStorage)
5. Steps to reproduce issue
6. What you've already tried
7. Photos of wiring (if hardware issue)
8. BLE scanner screenshots (if BLE issue)
```

### Diagnostic Serial Output

Run sensor and copy serial output including:
```
- Boot/initialization messages
- Configuration values
- Distance readings
- Error messages
- Detection events
```

### Test Sketches

Available test sketches for isolated testing:
```
- I2C Scanner (verify TFLuna presence)
- Minimal BLE (test BLE only)
- Pin Test (test output pin)
- Flash Test (test configuration storage)
```

---

## Quick Reference

### Serial Messages Meaning

| Message | Meaning |
|---------|---------|
| "Setup complete" | Normal startup |
| "Error: Failed to communicate" | I2C/TFLuna issue |
| "POTHOLE DETECTED" | Detection triggered |
| "Zero reference updated" | Config changed |
| "Configuration saved" | Saved to flash |

### LED Indicators

| LED State | Meaning |
|-----------|---------|
| Solid green | Powered on |
| Pulsing orange | Bootloader mode |
| Off | No power |

### Common Values

| Parameter | Typical Value |
|-----------|---------------|
| Baud Rate | 115200 |
| I2C Address | 0x10 |
| Threshold | 5-10 cm |
| Zero Reference | 30-100 cm (varies) |
| Output Duration | 2000 ms |

---

**Remember**: Most issues are wiring or configuration problems. Check basics first!
