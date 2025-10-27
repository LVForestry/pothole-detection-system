# System Architecture and Design

## Overview
This document describes the architecture and design decisions for the Arduino Nano 33 BLE sensor node firmware.

## System Architecture

### High-Level System Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    Pothole Detection System                      │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
        ▼                     ▼                     ▼
┌──────────────┐     ┌──────────────┐      ┌──────────────┐
│   Sensor     │     │   Sensor     │ ...  │   Sensor     │
│   Node R1    │     │   Node R2    │      │   Node L1    │
│ (16 total)   │     │              │      │              │
└──────┬───────┘     └──────┬───────┘      └──────┬───────┘
       │                    │                     │
       │ Digital Signal     │                     │
       │ (Pin 2 → HIGH)     │                     │
       └────────────────────┴─────────────────────┘
                            │
                            ▼
                  ┌─────────────────┐
                  │  Arduino Giga   │
                  │  R1 Controller  │
                  └────────┬────────┘
                           │
                           ▼
                  ┌─────────────────┐
                  │   WiFi AP +     │
                  │  Web Dashboard  │
                  └─────────────────┘
                           │
                           ▼
                    ┌──────────────┐
                    │  User Access │
                    └──────────────┘
```

## Sensor Node Architecture

### Component Diagram

```
┌───────────────────────────────────────────────────────────────┐
│                  Arduino Nano 33 BLE Sensor Node              │
│                                                               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │                    Main Loop (10Hz)                   │   │
│  └──────────────────────┬───────────────────────────────┘   │
│                         │                                     │
│         ┌───────────────┼───────────────┐                    │
│         │               │               │                    │
│         ▼               ▼               ▼                    │
│  ┌──────────┐   ┌──────────┐   ┌──────────────┐            │
│  │   I2C    │   │   BLE    │   │   Digital    │            │
│  │ Handler  │   │ Handler  │   │    Output    │            │
│  └────┬─────┘   └────┬─────┘   └──────┬───────┘            │
│       │              │                 │                     │
│       ▼              ▼                 ▼                     │
│  ┌──────────────────────────────────────────┐               │
│  │        Detection & Processing Logic      │               │
│  │                                          │               │
│  │  • Read distance from TFLuna            │               │
│  │  • Compare with zero reference          │               │
│  │  • Check against threshold              │               │
│  │  • Trigger output if pothole detected   │               │
│  │  • Update BLE characteristics           │               │
│  └──────────────────────────────────────────┘               │
│                         │                                     │
│                         ▼                                     │
│  ┌──────────────────────────────────────────┐               │
│  │          Flash Storage Layer             │               │
│  │  • Zero Reference (persistent)           │               │
│  │  • Threshold (persistent)                │               │
│  │  • Valid flag                            │               │
│  └──────────────────────────────────────────┘               │
│                                                               │
└───────────────────────────────────────────────────────────────┘
         │                  │                  │
         ▼                  ▼                  ▼
    TFLuna LIDAR      BLE Client        Giga R1 Pin
   (I2C @ 0x10)    (Phone/Controller)   (Digital Input)
```

### Software Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Firmware Layers                          │
├─────────────────────────────────────────────────────────────┤
│  Application Layer                                          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  Detection   │  │ BLE Service  │  │   Output     │     │
│  │    Logic     │  │   Manager    │  │   Control    │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
├─────────────────────────────────────────────────────────────┤
│  Hardware Abstraction Layer                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  I2C Driver  │  │  BLE Stack   │  │  GPIO Driver │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
├─────────────────────────────────────────────────────────────┤
│  Storage Layer                                              │
│  ┌───────────────────────────────────────────────┐         │
│  │         FlashStorage (EEPROM Emulation)       │         │
│  └───────────────────────────────────────────────┘         │
├─────────────────────────────────────────────────────────────┤
│  Arduino Core & Libraries                                   │
│  • Arduino Mbed OS Core (nRF52840)                         │
│  • ArduinoBLE Library                                      │
│  • Wire Library (I2C)                                      │
│  • FlashStorage Library                                    │
└─────────────────────────────────────────────────────────────┘
```

## Data Flow

### Distance Measurement Flow

```
Start
  │
  ├─→ I2C Request to TFLuna (addr 0x10)
  │        │
  │        ▼
  │   Read 2 bytes (distance data)
  │        │
  │        ▼
  │   Parse to int16_t (distance in cm)
  │        │
  │        ▼
  │   Store in currentDistance
  │        │
  │        ├─→ Update BLE characteristic (notify)
  │        │
  │        └─→ Pass to Detection Logic
  │                 │
  │                 ▼
  │            Calculate: depth = currentDistance - zeroReference
  │                 │
  │                 ▼
  │            Is depth > threshold?
  │                 │
  │         ┌───────┴───────┐
  │         │               │
  │        YES              NO
  │         │               │
  │         ▼               └─→ Continue monitoring
  │    Trigger Output
  │         │
  │         ├─→ Set Pin 2 HIGH
  │         ├─→ Record timestamp
  │         └─→ Log to serial
  │
  └─→ After 2 seconds: Set Pin 2 LOW
  │
  ▼
Loop continues (10Hz)
```

### BLE Configuration Flow

```
BLE Client connects
       │
       ▼
  Access Service (12340000-...)
       │
       ├─→ Read Distance Characteristic
       │        └─→ Returns currentDistance
       │
       ├─→ Read Zero Reference
       │        └─→ Returns config.zeroReference
       │
       ├─→ Read Threshold
       │        └─→ Returns config.thresholdCm
       │
       ├─→ Write Zero Reference
       │        │
       │        ├─→ Validate value
       │        ├─→ Update config.zeroReference
       │        ├─→ Save to flash
       │        └─→ Log to serial
       │
       └─→ Write Threshold
                │
                ├─→ Validate value
                ├─→ Update config.thresholdCm
                ├─→ Save to flash
                └─→ Log to serial
```

## State Machine

### Sensor Node States

```
┌──────────────┐
│  POWER ON    │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ INITIALIZE   │ ← Setup I2C, BLE, Load config
└──────┬───────┘
       │
       ▼
┌──────────────────────────────────────┐
│         MONITORING                   │ ◄─────┐
│  • Read distance (10Hz)              │       │
│  • Update BLE                        │       │
│  • Check for detection               │       │
└──────┬───────────────────────────────┘       │
       │                                        │
       │ Pothole detected                      │
       │ (depth > threshold)                   │
       ▼                                        │
┌──────────────────────────────────────┐       │
│         ALERTING                     │       │
│  • Pin 2 = HIGH                      │       │
│  • Timer = 2 seconds                 │       │
│  • Continue monitoring               │       │
└──────┬───────────────────────────────┘       │
       │                                        │
       │ Timeout (2s elapsed)                  │
       │                                        │
       ├─→ Pin 2 = LOW ───────────────────────┘
       │
       │ At any time:
       │ BLE event
       │
       ▼
┌──────────────────────────────────────┐
│    BLE CONFIGURATION                 │
│  • Read characteristics              │
│  • Write characteristics             │
│  • Update flash storage              │
└──────┬───────────────────────────────┘
       │
       └─────────────────────────────────────┐
                                             │
                                             ▼
                                      Resume monitoring
```

## Memory Map

### Flash Storage Layout

```
Flash Address Space (nRF52840 Flash)
┌────────────────────────────────┐
│   Program Code (~8KB)          │
├────────────────────────────────┤
│   ...                          │
├────────────────────────────────┤
│   FlashStorage Emulation       │
│   ┌──────────────────────┐     │
│   │ zeroReference (2B)   │     │
│   │ thresholdCm (2B)     │     │
│   │ valid (1B)           │     │
│   └──────────────────────┘     │
└────────────────────────────────┘

Total Config Size: 5 bytes
```

### RAM Usage

```
RAM (256KB available)

Stack:
• Local variables
• Function calls
• Interrupt handlers

Heap:
• BLE buffers
• String objects
• Dynamic allocations

Global Variables:
• config (5 bytes)
• currentDistance (2 bytes)
• potholeDetected (1 byte)
• timestamps (8 bytes)
• BLE objects (~1KB)

Estimated Total: <5KB
```

## Timing Diagram

### Main Loop Timing (10Hz operation)

```
Time (ms):  0      100     200     300     400     500
            │       │       │       │       │       │
Measure:    ●───────●───────●───────●───────●───────●
            │       │       │       │       │       │
BLE Update: ●───────●───────●───────●───────●───────●
            │       │       │       │       │       │
Detection:  │   ●───┼───────┼───────┼───────┼───────
Check       │       │       │       │       │       │
            │       │       │       │       │       │
Pin 2:      │   ████████████████████│       │       │
            LOW     HIGH (2000ms)   LOW     LOW     LOW
```

### Detection Event Timing

```
Event:
  Distance drop detected (depth > threshold)
  │
  ├─ t=0ms:    Pothole detected flag set
  ├─ t=0ms:    Pin 2 → HIGH
  ├─ t=0ms:    Serial log message
  ├─ t=0ms:    Record timestamp
  │
  ├─ t=100ms:  Continue measurements
  ├─ t=200ms:  Continue measurements
  ├─ t=....:   Continue measurements
  │
  └─ t=2000ms: Pin 2 → LOW
               Reset detection flag
               Serial log message
```

## Design Decisions

### Why I2C for TFLuna?
- **Pro**: Standard protocol, simple wiring (2 wires)
- **Pro**: Multiple sensors possible on same bus (not used here)
- **Pro**: Well-supported libraries
- **Con**: Limited cable length (<30cm recommended)
- **Decision**: I2C chosen for simplicity and reliability

### Why 10Hz Measurement Rate?
- **Pro**: Fast enough for real-time detection
- **Pro**: Low enough to avoid I2C congestion
- **Pro**: Matches typical vehicle speeds (pothole detection)
- **Con**: Higher rates would increase power consumption
- **Decision**: 10Hz provides good balance

### Why 2-Second Output Duration?
- **Pro**: Long enough for Giga R1 to reliably detect
- **Pro**: Prevents missed events due to polling delays
- **Pro**: Simple timing (no complex protocols)
- **Con**: Multiple rapid events may overlap
- **Decision**: 2 seconds provides reliable detection

### Why BLE Instead of WiFi?
- **Pro**: Lower power consumption
- **Pro**: Built into Nano 33 BLE (no external hardware)
- **Pro**: Suitable for short-range configuration
- **Con**: Limited range (~10m)
- **Decision**: BLE sufficient for local configuration

### Why Flash Storage?
- **Pro**: Configuration persists across power cycles
- **Pro**: No external EEPROM needed
- **Pro**: Built-in FlashStorage library
- **Con**: Limited write cycles (~10,000)
- **Decision**: Adequate for infrequent config updates

## Performance Characteristics

### Latency Budget

| Operation | Time | Notes |
|-----------|------|-------|
| I2C Read | 5-10ms | TFLuna response time |
| Distance Parse | <1ms | Simple 16-bit conversion |
| Detection Check | <1ms | Basic comparison |
| BLE Update | 1-5ms | Characteristic write |
| GPIO Output | <1µs | Direct pin write |
| **Total** | **<20ms** | Well within 100ms budget |

### Throughput

| Metric | Value | Unit |
|--------|-------|------|
| Measurements/sec | 10 | Hz |
| BLE Updates/sec | 10 | Hz |
| I2C Transactions/sec | 10 | Hz |
| Detections/sec | Variable | Events |

## Error Handling

### I2C Communication Errors
```
I2C Read Error
  │
  ├─→ Log to serial
  ├─→ Set currentDistance = -1 (invalid)
  ├─→ Skip detection for this cycle
  └─→ Continue monitoring (retry next cycle)
```

### BLE Connection Errors
```
BLE Connection Lost
  │
  ├─→ Continue monitoring (independent of BLE)
  ├─→ Digital output continues to work
  ├─→ Auto-reconnect on next client request
  └─→ No data loss (flash storage persists)
```

### Flash Storage Errors
```
Invalid Configuration Detected
  │
  ├─→ Log to serial
  ├─→ Load default values
  ├─→ Save defaults to flash
  └─→ Continue operation
```

## Security Considerations

### BLE Security
- **Current**: Open pairing (no encryption)
- **Rationale**: Local network, no sensitive data
- **Future**: Could add pairing/bonding if needed

### Flash Security
- **Current**: No encryption of stored data
- **Rationale**: Configuration is not sensitive
- **Protection**: Flash is physically protected in device

### I2C Security
- **Current**: No encryption
- **Rationale**: Short cables, physically secured
- **Protection**: Device enclosure prevents tampering

## Scalability

### Current Implementation
- 16 independent sensor nodes
- Each with unique BLE name
- Separate I2C buses (no interference)
- Individual digital outputs

### Future Scaling Options
- Support up to 32+ sensors (limited by Giga pins)
- I2C bus sharing (if needed)
- CAN bus for high-reliability communication
- LoRa for long-range deployment

## Reliability Features

1. **Watchdog Timer** (future): Auto-reset on hang
2. **Error Logging**: Serial output for debugging
3. **Flash Validation**: Check for corrupt data
4. **Default Fallback**: Use defaults if config invalid
5. **Continuous Operation**: BLE loss doesn't stop monitoring
6. **I2C Retry**: Continue despite temporary failures

## Testing Strategy

### Unit Tests
- I2C communication
- Distance parsing
- Detection logic
- BLE characteristics
- Flash storage

### Integration Tests
- Complete measurement cycle
- BLE + detection simultaneously
- Configuration persistence
- Multi-hour stability

### System Tests
- All 16 sensors active
- Giga R1 integration
- Real-world potholes
- Environmental conditions

---

**Version**: 1.0  
**Last Updated**: 2025-10-21  
**Status**: Production Design
