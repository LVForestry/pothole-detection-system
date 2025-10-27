# Hardware Wiring Guide

## Component Overview

### Arduino Nano 33 BLE
- Microcontroller: nRF52840 (32-bit ARM Cortex-M4)
- Operating Voltage: 3.3V (5V tolerant inputs)
- BLE: Built-in Bluetooth 5.0
- I2C: Hardware I2C support on SDA/SCL pins

### TFLuna LIDAR Sensor
- Detection Range: 0.2m to 8m
- Accuracy: ±2cm @ <3m
- Operating Voltage: 5V
- Interface: I2C (also supports UART)
- I2C Address: 0x10 (default)

## Complete Wiring Diagram

### Sensor Node Components
```
┌─────────────────────┐
│  Arduino Nano 33    │
│       BLE           │
│                     │
│  5V  ────────────── │──── 5V  ──┐
│  GND ────────────── │──── GND ──┤
│  SDA ────────────── │──── SDA ──┤    ┌──────────────┐
│  SCL ────────────── │──── SCL ──┼────│  TFLuna      │
│                     │           │    │  LIDAR       │
│  Pin 2 ──────────── │───────────┘    │              │
│                     │                └──────────────┘
│  USB ────────────── │──── PC (Programming/Power)
│                     │
└─────────────────────┘
         │
         │ Digital Output
         │ to Giga R1
         └────────────► Giga R1 Pin (23-53)
```

## Detailed Pin Connections

### TFLuna to Arduino Nano 33 BLE

| TFLuna Pin | Wire Color | Arduino Pin | Description |
|------------|------------|-------------|-------------|
| VCC | Red | 5V | Power supply (5V) |
| GND | Black | GND | Ground |
| SDA | Green/White | SDA (A4) | I2C Data |
| SCL | Blue/Yellow | SCL (A5) | I2C Clock |

### Arduino to Giga R1 Controller

| Arduino Pin | Giga R1 Pin | Sensor Position | Notes |
|-------------|-------------|-----------------|-------|
| Pin 2 | 23 | R1 | Digital output signal |
| Pin 2 | 25 | R2 | Digital output signal |
| Pin 2 | 27 | R3 | Digital output signal |
| Pin 2 | 29 | R4 | Digital output signal |
| Pin 2 | 31 | R5 | Digital output signal |
| Pin 2 | 33 | R6 | Digital output signal |
| Pin 2 | 35 | R7 | Digital output signal |
| Pin 2 | 37 | R8 | Digital output signal |
| Pin 2 | 39 | L8 | Digital output signal |
| Pin 2 | 41 | L7 | Digital output signal |
| Pin 2 | 43 | L6 | Digital output signal |
| Pin 2 | 45 | L5 | Digital output signal |
| Pin 2 | 47 | L4 | Digital output signal |
| Pin 2 | 49 | L3 | Digital output signal |
| Pin 2 | 51 | L2 | Digital output signal |
| Pin 2 | 53 | L1 | Digital output signal |
| GND | GND | All | Common ground (shared) |

**Important**: Each Arduino's Pin 2 connects to a different Giga R1 input pin based on its position.

## I2C Bus Specifications

### Pin Locations on Arduino Nano 33 BLE
- **SDA**: A4 (also available on dedicated SDA pin header)
- **SCL**: A5 (also available on dedicated SCL pin header)

### Configuration in Firmware
```cpp
Wire.begin();           // Initialize I2C as master
Wire.setClock(400000);  // Set I2C speed to 400kHz (Fast Mode)
```

### I2C Pull-up Resistors
- Arduino Nano 33 BLE has internal pull-ups enabled by default
- TFLuna has internal pull-ups on the I2C bus
- External pull-ups (4.7kΩ to 10kΩ) are optional but can improve reliability for long wires

## Power Supply

### Option 1: USB Power (Development/Testing)
```
USB Cable → Arduino Nano 33 BLE (5V/500mA)
  └─→ 5V Pin → TFLuna (via pin header)
```

### Option 2: External Power Supply (Production)
```
5V DC Power Supply → Arduino VIN Pin (or 5V Pin)
                  └─→ TFLuna VCC
```

### Power Requirements
- Arduino Nano 33 BLE: ~50mA (typical), up to 100mA with BLE active
- TFLuna LIDAR: ~100-150mA (typical)
- **Total per node**: ~200mA peak
- **Recommended supply**: 5V @ 500mA per node

### All 16 Nodes
- Total current: 16 × 200mA = 3.2A
- Recommended supply: 5V @ 5A (with margin)

## Output Signal Characteristics

### Pin 2 Output Specifications
- **Logic Level**: 3.3V HIGH, 0V LOW
- **Output Current**: Maximum 15mA per pin
- **Duration**: 2 seconds (configurable in firmware)
- **Timing**: HIGH immediately upon detection, returns LOW after 2s

### Connecting to 5V Logic (Giga R1)
The Giga R1 can read 3.3V logic levels directly:
- 3.3V is above the HIGH threshold for 5V CMOS logic
- No level shifter required
- Direct connection: Arduino Pin 2 → Giga Input Pin

If voltage level conversion is needed:
```
Arduino Pin 2 ─────┬──── 10kΩ ──── Giga Input Pin
                   │
                   └──── 4.7kΩ ──── GND
(Voltage divider, but typically not necessary)
```

## Assembly Instructions

### Step 1: Prepare Components
1. Arduino Nano 33 BLE board
2. TFLuna LIDAR sensor
3. Jumper wires (4 for I2C, 1 for output, 1 for ground to Giga)
4. Optional: Breadboard for testing

### Step 2: Make TFLuna Connections
1. Connect VCC (red wire) to Arduino 5V pin
2. Connect GND (black wire) to Arduino GND pin
3. Connect SDA (green wire) to Arduino SDA pin (A4)
4. Connect SCL (blue wire) to Arduino SCL pin (A5)

### Step 3: Make Giga R1 Connections
1. Connect Arduino Pin 2 to corresponding Giga R1 input pin
2. Connect Arduino GND to Giga R1 GND (common ground)

### Step 4: Secure Connections
- Use female-to-male jumper wires for breadboard connections
- For production: Solder connections or use screw terminals
- Use heat shrink tubing for insulation
- Label wires with sensor position

### Step 5: Mount Hardware
- Mount Arduino in protective enclosure
- Mount TFLuna with clear view to road surface
- Protect from moisture and debris
- Ensure stable mounting (minimize vibration)

## Wiring Best Practices

### Cable Management
1. **Keep I2C wires short**: <30cm for reliable communication
2. **Separate power and signal**: Reduce noise interference
3. **Strain relief**: Secure cables to prevent pulling on connections
4. **Label everything**: Mark each cable with sensor position

### Noise Reduction
1. **Twisted pairs**: Twist SDA/SCL wires together
2. **Shielded cable**: Use for long runs (>30cm)
3. **Ground plane**: Connect shield to GND at one end only
4. **Capacitors**: 0.1µF ceramic cap across VCC/GND near TFLuna

### Reliability Improvements
1. **Add external pull-ups**: 4.7kΩ resistors on SDA/SCL to 3.3V
2. **Power filtering**: Add 100µF electrolytic cap on power supply
3. **ESD protection**: Use TVS diodes on exposed connections
4. **Secure connectors**: Use locking connectors for production

## Testing the Wiring

### Step 1: Visual Inspection
- [ ] All connections secure
- [ ] No short circuits
- [ ] Correct polarity (VCC/GND)
- [ ] Wire colors match diagram

### Step 2: Continuity Test (Multimeter)
- [ ] Measure resistance between Arduino GND and TFLuna GND (should be ~0Ω)
- [ ] Verify no shorts between VCC and GND (should be open circuit)
- [ ] Check SDA and SCL connections

### Step 3: Voltage Test (Multimeter)
- [ ] Measure 5V between Arduino 5V pin and GND
- [ ] Measure 5V at TFLuna VCC pin
- [ ] Measure 3.3V on Arduino 3.3V output pin (not used, but verify board health)

### Step 4: I2C Scanner Test
Upload I2C scanner sketch to verify TFLuna is detected:
```cpp
// I2C Scanner Sketch (for testing only)
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
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
  if (count == 0) Serial.println("No devices found");
  delay(5000);
}
```
Expected output: `Found device at 0x10` (TFLuna)

### Step 5: Firmware Test
1. Upload main firmware
2. Open Serial Monitor (115200 baud)
3. Verify initialization messages
4. Check for distance readings
5. Test pothole detection (move sensor or object)
6. Verify Pin 2 output with multimeter or LED

## Troubleshooting

### No Distance Readings
- **Check I2C wiring**: Verify SDA/SCL connections
- **Check power**: Verify 5V at TFLuna
- **Run I2C scanner**: Verify device appears at 0x10
- **Check TFLuna mode**: Ensure it's in I2C mode (not UART)

### Intermittent Connection
- **Loose wires**: Check all connections
- **Wire length**: Reduce I2C cable length
- **Add pull-ups**: Install 4.7kΩ resistors on SDA/SCL
- **Power supply**: Ensure stable 5V supply

### Giga Not Detecting Signal
- **Check output connection**: Verify Pin 2 to correct Giga pin
- **Common ground**: Ensure GND is shared between Arduino and Giga
- **Test with LED**: Connect LED to Pin 2 to verify output
- **Check trigger**: Verify pothole detection is occurring (Serial Monitor)

### Multiple Sensors Interference
- **Unique I2C bus**: Each sensor has its own I2C bus (no conflict)
- **Common ground**: All devices must share common ground
- **Separate power**: Consider individual power supplies for each node
- **Check addresses**: Verify each TFLuna is at 0x10 (all should be same)

## Safety Warnings

⚠️ **Important Safety Information**:
- Do not connect/disconnect components while powered
- Verify voltage levels before connecting
- Use appropriate wire gauge for current levels
- Protect against short circuits
- Ensure proper insulation of exposed conductors
- Keep connections away from moisture
- Use fuses on power supply lines

## Advanced Wiring Options

### For Development: Breadboard Setup
```
Breadboard Layout:
─────────────────────────────────
  Arduino Nano 33 BLE (on breadboard)
─────────────────────────────────
  +5V Rail → TFLuna VCC
  GND Rail → TFLuna GND
  SDA → TFLuna SDA
  SCL → TFLuna SCL
  Pin 2 → Test LED (+ 220Ω resistor)
─────────────────────────────────
```

### For Production: PCB or Soldered Connections
- Design custom PCB with screw terminals
- Include power protection circuits
- Add status LEDs for diagnostics
- Include test points for troubleshooting

### For Testing: Proto Shield
- Use Arduino proto shield for permanent connections
- Solder connections for reliability
- Add screw terminals for external connections
- Include protection diodes

## Wire Specifications

### Recommended Wire Types

| Connection | Wire Type | Gauge | Length |
|------------|-----------|-------|--------|
| Power (VCC/GND) | Stranded | 22 AWG | <1m |
| I2C (SDA/SCL) | Stranded, twisted pair | 24 AWG | <0.3m |
| Output Signal | Stranded | 24 AWG | As needed |

### Color Coding (Recommended)
- **Red**: VCC (5V power)
- **Black**: GND (ground)
- **Green/White**: SDA (I2C data)
- **Blue/Yellow**: SCL (I2C clock)
- **Orange**: Digital output (Pin 2)

## Maintenance

### Regular Checks
- Inspect connections monthly
- Check for loose wires
- Clean connectors as needed
- Test continuity if issues arise

### Environmental Protection
- Use IP65+ rated enclosures for outdoor use
- Apply conformal coating to PCBs
- Use waterproof connectors
- Protect from UV exposure

---

**Note**: This wiring guide assumes standard TFLuna and Arduino Nano 33 BLE specifications. Always verify your specific component datasheets for any variations.
