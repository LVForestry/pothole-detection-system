/*
 * Configuration Header
 * Pothole Detection System - Arduino Giga R1 Central Controller
 * 
 * This file contains all configurable parameters for the system.
 * Modify these values to customize the system behavior.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// PIN CONFIGURATION
// ============================================================================

// Digital input pins for sensor monitoring
// These pins connect to the digital output pins of the 16 sensor nodes
#define SENSOR_PIN_R1  23
#define SENSOR_PIN_R2  25
#define SENSOR_PIN_R3  27
#define SENSOR_PIN_R4  29
#define SENSOR_PIN_R5  31
#define SENSOR_PIN_R6  33
#define SENSOR_PIN_R7  35
#define SENSOR_PIN_R8  37
#define SENSOR_PIN_L8  39
#define SENSOR_PIN_L7  41
#define SENSOR_PIN_L6  43
#define SENSOR_PIN_L5  45
#define SENSOR_PIN_L4  47
#define SENSOR_PIN_L3  49
#define SENSOR_PIN_L2  51
#define SENSOR_PIN_L1  53

// ============================================================================
// WIFI CONFIGURATION
// ============================================================================

// WiFi Access Point settings
#define WIFI_AP_SSID      "Pothole-Detection-AP"
#define WIFI_AP_PASSWORD  ""  // Empty = open network
#define WIFI_AP_CHANNEL   6   // WiFi channel (1-13)
#define WEB_SERVER_PORT   80  // HTTP server port

// ============================================================================
// BLE CONFIGURATION
// ============================================================================

// BLE Service UUID (must match sensor node firmware)
#define BLE_SERVICE_UUID       "19b10000-e8f2-537e-4f6c-d104768a1214"

// BLE Characteristic UUIDs (must match sensor node firmware)
#define BLE_DISTANCE_CHAR_UUID    "19b10001-e8f2-537e-4f6c-d104768a1214"
#define BLE_ZERO_REF_CHAR_UUID    "19b10002-e8f2-537e-4f6c-d104768a1214"
#define BLE_THRESHOLD_CHAR_UUID   "19b10003-e8f2-537e-4f6c-d104768a1214"

// BLE Device Name Prefix
#define BLE_DEVICE_PREFIX "NanoTFluna-"

// BLE Connection Settings
#define BLE_SCAN_TIMEOUT_MS    5000  // Maximum time to scan for a device (ms)
#define BLE_CONNECT_TIMEOUT_MS 3000  // Maximum time to establish connection (ms)

// ============================================================================
// SERIAL CONFIGURATION
// ============================================================================

#define SERIAL_BAUD_RATE      115200
#define SERIAL_INIT_TIMEOUT   3000    // Wait for serial connection (ms)

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================

// Main loop delay to prevent CPU overload (ms)
#define MAIN_LOOP_DELAY_MS    10

// Dashboard auto-refresh interval (ms)
#define DASHBOARD_REFRESH_MS  5000

// Enable/disable features
#define ENABLE_SERIAL_LOGGING  true   // Log events to serial monitor
#define ENABLE_BLE             true   // Enable BLE functionality
#define ENABLE_WIFI_AP         true   // Enable WiFi Access Point
#define ENABLE_WEB_DASHBOARD   true   // Enable web dashboard

// ============================================================================
// SENSOR NAMING
// ============================================================================

// Sensor position names
#define SENSOR_NAME_R1  "R1"
#define SENSOR_NAME_R2  "R2"
#define SENSOR_NAME_R3  "R3"
#define SENSOR_NAME_R4  "R4"
#define SENSOR_NAME_R5  "R5"
#define SENSOR_NAME_R6  "R6"
#define SENSOR_NAME_R7  "R7"
#define SENSOR_NAME_R8  "R8"
#define SENSOR_NAME_L8  "L8"
#define SENSOR_NAME_L7  "L7"
#define SENSOR_NAME_L6  "L6"
#define SENSOR_NAME_L5  "L5"
#define SENSOR_NAME_L4  "L4"
#define SENSOR_NAME_L3  "L3"
#define SENSOR_NAME_L2  "L2"
#define SENSOR_NAME_L1  "L1"

// BLE device names for each sensor
#define BLE_DEVICE_R1  "NanoTFluna-R1"
#define BLE_DEVICE_R2  "NanoTFluna-R2"
#define BLE_DEVICE_R3  "NanoTFluna-R3"
#define BLE_DEVICE_R4  "NanoTFluna-R4"
#define BLE_DEVICE_R5  "NanoTFluna-R5"
#define BLE_DEVICE_R6  "NanoTFluna-R6"
#define BLE_DEVICE_R7  "NanoTFluna-R7"
#define BLE_DEVICE_R8  "NanoTFluna-R8"
#define BLE_DEVICE_L8  "NanoTFluna-L8"
#define BLE_DEVICE_L7  "NanoTFluna-L7"
#define BLE_DEVICE_L6  "NanoTFluna-L6"
#define BLE_DEVICE_L5  "NanoTFluna-L5"
#define BLE_DEVICE_L4  "NanoTFluna-L4"
#define BLE_DEVICE_L3  "NanoTFluna-L3"
#define BLE_DEVICE_L2  "NanoTFluna-L2"
#define BLE_DEVICE_L1  "NanoTFluna-L1"

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

// Enable verbose debug output
#define DEBUG_VERBOSE         false

// Debug specific modules
#define DEBUG_PIN_MONITORING  false
#define DEBUG_BLE_OPERATIONS  false
#define DEBUG_WIFI_OPERATIONS false
#define DEBUG_WEB_REQUESTS    false

#endif // CONFIG_H
