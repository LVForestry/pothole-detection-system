# System Architecture

## Overview

The Pothole Detection System consists of a distributed network of sensors managed by a central controller. This document describes the architecture of the Arduino Giga R1 Central Controller firmware.

## System Components

### 1. Hardware Layer

```
┌─────────────────────────────────────────────────────────────┐
│                   Arduino Giga R1 WiFi                      │
│                                                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │  ARM     │  │   WiFi   │  │   BLE    │  │  GPIO    │  │
│  │ Cortex-M7│  │  Radio   │  │  Radio   │  │  Pins    │  │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘  │
└─────────────────────────────────────────────────────────────┘
        │              │              │              │
        ▼              ▼              ▼              ▼
   Processing      WiFi AP       BLE Client    16 Digital
   (Main Loop)   (192.168.4.1)  (Scanner/Conn)   Inputs
```

### 2. Software Layer

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                        │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────┐  │
│  │  Pin Monitor │  │ BLE Manager  │  │  Web Dashboard  │  │
│  │   Module     │  │   Module     │  │     Module      │  │
│  └──────────────┘  └──────────────┘  └─────────────────┘  │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                  Communication Layer                        │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────┐  │
│  │    Serial    │  │ WiFi Server  │  │  BLE Protocol   │  │
│  │   (Logging)  │  │   (HTTP)     │  │   (GATT)        │  │
│  └──────────────┘  └──────────────┘  └─────────────────┘  │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                     Hardware Layer                          │
│    WiFi Library    │  ArduinoBLE   │   Arduino Core        │
└─────────────────────────────────────────────────────────────┘
```

## Module Descriptions

### Pin Monitor Module

**Purpose**: Continuously monitors 16 digital input pins for HIGH signals from sensor nodes.

**Functions**:
- `initializePins()`: Configures all 16 pins as INPUT
- `monitorSensorPins()`: Reads pin states and detects state changes
- State tracking: Maintains current and previous states to detect LOW→HIGH transitions

**Pin Mapping**:
- Pins 23-37 (odd): Right sensors R1-R8
- Pins 39-53 (odd): Left sensors L8-L1

**Operation Flow**:
```
Loop Start
    │
    ├─► Read Pin 23 (R1)
    ├─► Read Pin 25 (R2)
    ├─► ... (all 16 pins)
    ├─► Read Pin 53 (L1)
    │
    ├─► Compare with Previous State
    │   │
    │   └─► If LOW→HIGH Transition
    │       └─► Log Event to Serial
    │
    └─► Update Previous State
```

### BLE Manager Module

**Purpose**: Handles Bluetooth Low Energy communication with sensor nodes.

**Functions**:
- `initializeBLE()`: Initializes BLE radio
- `pollSensor()`: Connects to a sensor node and reads distance
- `configureSensor()`: Writes configuration values to a sensor node

**BLE Service Structure**:
```
Service UUID: 19b10000-e8f2-537e-4f6c-d104768a1214
│
├─► Distance Characteristic (Read)
│   UUID: 19b10001-e8f2-537e-4f6c-d104768a1214
│   Type: Float (4 bytes)
│   Description: Current distance reading in cm
│
├─► Zero Reference Characteristic (Write)
│   UUID: 19b10002-e8f2-537e-4f6c-d104768a1214
│   Type: Float (4 bytes)
│   Description: Zero reference value for calibration
│
└─► Threshold Characteristic (Write)
    UUID: 19b10003-e8f2-537e-4f6c-d104768a1214
    Type: Float (4 bytes)
    Description: Detection threshold in cm
```

**Connection Flow**:
```
Poll/Configure Request
    │
    ├─► Start BLE Scan
    │   └─► Scan for 5 seconds
    │
    ├─► Device Found?
    │   ├─► No → Return Error
    │   └─► Yes
    │       │
    │       ├─► Stop Scan
    │       ├─► Connect to Device
    │       ├─► Discover Attributes
    │       │
    │       ├─► Read/Write Characteristic
    │       ├─► Disconnect
    │       └─► Return Result
```

### Web Dashboard Module

**Purpose**: Provides HTTP server and web interface for monitoring and control.

**Components**:
1. **WiFi Access Point**: Creates open network "Pothole-Detection-AP"
2. **HTTP Server**: Listens on port 80
3. **Request Handler**: Routes requests to appropriate handlers
4. **HTML Generator**: Dynamically generates dashboard interface

**HTTP API Endpoints**:

```
GET /
├─► Serves main dashboard HTML
└─► Shows all 16 sensors with current states

GET /api/sensors
├─► Returns JSON with all sensor states
└─► Format: {"sensors": [{index, name, pin, state, ble_name}, ...]}

GET /api/poll/{index}
├─► Polls specific sensor via BLE
├─► Parameters: index (0-15)
└─► Returns: {success, name, ble_device, distance, status, pin_state}

POST /api/configure/{index}
├─► Configures sensor via BLE
├─► Parameters: index (0-15)
├─► Body: {zero: float, threshold: float}
└─► Returns: {success, message, sensor, zero, threshold}
```

**Request Processing Flow**:
```
HTTP Request
    │
    ├─► Parse Request Line
    │   └─► Extract Method, Path, Headers
    │
    ├─► Route Request
    │   ├─► "/" → serveDashboard()
    │   ├─► "/api/sensors" → serveSensorStates()
    │   ├─► "/api/poll/{id}" → pollSensor()
    │   ├─► "/api/configure/{id}" → configureSensor()
    │   └─► Other → send404()
    │
    ├─► Process Request
    │   └─► Generate Response
    │
    └─► Send HTTP Response
        └─► Close Connection
```

## Data Flow

### Event Detection Flow

```
Sensor Node (Nano)
    │ Detects pothole
    ├─► Sets pin D2 HIGH (2 seconds)
    │
    ▼
Giga R1 Digital Input (Pin 23-53)
    │ HIGH signal detected
    ├─► Logs to Serial Monitor
    │   ├─► Sensor name
    │   ├─► Pin number
    │   └─► Timestamp
    │
    └─► Updates internal state
        └─► Available in web dashboard
```

### Real-time Polling Flow

```
User (Web Browser)
    │ Clicks "Poll Sensor"
    ├─► HTTP GET /api/poll/{index}
    │
    ▼
Giga R1 Web Server
    │ Receives request
    ├─► Extracts sensor index
    │
    ▼
Giga R1 BLE Manager
    │ Initiates BLE scan
    ├─► Finds sensor node
    ├─► Connects to device
    ├─► Reads distance characteristic
    ├─► Disconnects
    │
    ▼
Giga R1 Web Server
    │ Formats JSON response
    └─► Sends to browser
        │
        ▼
    User (Web Browser)
        └─► Displays distance reading
```

### Configuration Update Flow

```
User (Web Browser)
    │ Clicks "Configure"
    │ Enters zero and threshold values
    ├─► HTTP POST /api/configure/{index}
    │   Body: {zero: 150.0, threshold: 5.0}
    │
    ▼
Giga R1 Web Server
    │ Receives request
    ├─► Parses JSON body
    ├─► Extracts sensor index and values
    │
    ▼
Giga R1 BLE Manager
    │ Initiates BLE scan
    ├─► Finds sensor node
    ├─► Connects to device
    ├─► Writes zero reference characteristic
    ├─► Writes threshold characteristic
    ├─► Disconnects
    │
    ▼
Sensor Node (Nano)
    │ Receives new values
    ├─► Updates internal variables
    └─► Stores to flash memory
        │
        ▼
Giga R1 Web Server
    │ Formats JSON response
    └─► Sends confirmation to browser
        │
        ▼
    User (Web Browser)
        └─► Shows success message
```

## Memory Management

### Static Allocations

```
SENSOR_PINS[16]        : 64 bytes (int array)
SENSOR_NAMES[16]       : 64 bytes (pointers to const strings)
BLE_DEVICE_NAMES[16]   : 64 bytes (pointers to const strings)
sensorStates[16]       : 16 bytes (bool array)
lastSensorStates[16]   : 16 bytes (bool array)
WiFiServer             : Variable (WiFi library)
```

### Dynamic Allocations

- BLE connections: Temporary during scan/connect operations
- HTTP requests: Temporary string buffers for request parsing
- Web responses: Generated on-the-fly, sent incrementally

### Stack Usage

- Main loop: Minimal, primarily function calls
- BLE operations: Moderate stack usage during scanning
- HTTP handling: String operations may use significant stack

## Timing and Performance

### Main Loop Cycle

```
Pin Monitoring:     < 1 ms (16 digital reads)
Web Server Check:   < 1 ms (if no client)
Web Server Handle:  Variable (if client connected)
Loop Delay:         10 ms
──────────────────────────────────────────
Total Cycle:        ~11 ms (idle)
                    Variable (serving requests)
```

### BLE Operations

```
BLE Scan:          up to 5000 ms (timeout)
BLE Connect:       500-2000 ms (typical)
Read Char:         100-500 ms
Write Char:        100-500 ms
──────────────────────────────────────────
Total Poll:        1-8 seconds (typical)
Total Configure:   1-8 seconds (typical)
```

### Web Response Times

```
Dashboard HTML:    100-300 ms
JSON API:          10-50 ms
Poll Sensor:       1-8 seconds (BLE operation)
Configure:         1-8 seconds (BLE operation)
```

## State Machine

### System States

```
┌─────────────┐
│   STARTUP   │
│   (init)    │
└──────┬──────┘
       │
       ├─► Initialize Pins
       ├─► Initialize BLE
       ├─► Initialize WiFi AP
       │
       ▼
┌─────────────┐
│    READY    │◄─────────────┐
│  (running)  │              │
└──────┬──────┘              │
       │                     │
       ├─► Monitor Pins ─────┤
       ├─► Handle Web ───────┤
       │   Requests          │
       │                     │
       └─────────────────────┘
```

### Pin State Machine (per pin)

```
    ┌──────┐
    │ LOW  │
    └───┬──┘
        │ Signal HIGH
        ▼
    ┌──────┐
    │ HIGH │
    │(log) │
    └───┬──┘
        │ Signal LOW
        ▼
    ┌──────┐
    │ LOW  │
    └──────┘
```

## Error Handling

### BLE Errors

- **Scan Timeout**: Returns error if device not found in 5 seconds
- **Connection Failed**: Returns disconnected status
- **Attribute Discovery Failed**: Returns error, disconnects
- **Characteristic Not Found**: Skips operation, continues
- **Read/Write Failed**: Returns error, disconnects

### WiFi Errors

- **AP Creation Failed**: Logs error, continues with other functions
- **Client Connection Lost**: Closes connection, continues
- **Invalid Request**: Returns 404 error

### Pin Monitoring

- No error handling needed (digital reads always succeed)
- State tracking prevents duplicate logs

## Security Considerations

### Current Implementation

- **Open WiFi**: No password required for AP
- **No Authentication**: Web API has no authentication
- **No Encryption**: HTTP traffic is unencrypted
- **No Input Validation**: Minimal validation of configuration values

### Recommended Enhancements (Future)

1. Add WPA2 encryption to WiFi AP
2. Implement basic authentication for web interface
3. Add HTTPS support for encrypted communication
4. Validate and sanitize all input values
5. Implement rate limiting for API requests
6. Add session management for web users

## Scalability

### Current Limits

- **Sensors**: Fixed at 16 (hardware pin limitation)
- **Concurrent Web Clients**: 1 at a time (single-threaded)
- **BLE Connections**: 1 at a time (sequential polling)
- **Memory**: Dependent on Giga R1 RAM (~1 MB available)

### Extension Points

- Add logging to SD card
- Implement sensor data buffering
- Add support for external storage
- Enable concurrent BLE connections (if supported)
- Implement WebSocket for real-time updates

## Configuration

### Compile-Time Configuration

All settings in `config.h`:
- Pin assignments
- WiFi credentials
- BLE UUIDs
- Timing parameters
- Debug flags

### Runtime Configuration

- Sensor zero references (stored on sensor nodes)
- Sensor thresholds (stored on sensor nodes)
- No runtime configuration for central controller

## Dependencies

### Arduino Libraries

1. **WiFi.h**
   - Provided by: Arduino Mbed OS Giga board package
   - Purpose: WiFi AP and HTTP server functionality
   - Version: Included with board package

2. **ArduinoBLE.h**
   - Provided by: Arduino BLE library
   - Purpose: Bluetooth Low Energy communication
   - Version: 1.3.6 or higher

### Board Support

- **Arduino Mbed OS Giga Boards**: Required for Giga R1 WiFi support
- **ARM Mbed OS**: Underlying RTOS and HAL

## Future Enhancements

### Planned Features

1. **Data Logging**
   - Log events to SD card
   - Timestamp with RTC
   - Export logs via web interface

2. **Analytics**
   - Track pothole frequency
   - Heat maps of detection zones
   - Historical trending

3. **Notifications**
   - Email alerts for detections
   - SMS notifications
   - Push notifications to mobile app

4. **Cloud Integration**
   - MQTT publish to cloud platform
   - RESTful API for remote access
   - Cloud-based dashboard

5. **Advanced BLE**
   - Automatic node discovery
   - Health monitoring of sensor nodes
   - OTA firmware updates

6. **Improved Web Interface**
   - WebSocket for real-time updates
   - Interactive map view
   - Mobile-responsive design
   - Multi-language support

## Conclusion

The Arduino Giga R1 Central Controller firmware provides a robust, modular architecture for the Pothole Detection System. Its design emphasizes:

- **Reliability**: Continuous monitoring with minimal overhead
- **Accessibility**: Easy-to-use web interface
- **Flexibility**: Configurable via web dashboard
- **Maintainability**: Clear module separation and documentation
- **Extensibility**: Easy to add new features

The architecture balances functionality with the constraints of embedded systems, providing a solid foundation for real-world deployment.
