/**
 * Configuration Template for Arduino Nano 33 BLE Sensor Nodes
 * 
 * This file provides a template for configuring multiple sensor nodes.
 * Copy this file and modify the SENSOR_POSITION for each sensor node
 * before uploading the firmware.
 * 
 * Sensor Positions:
 * Right side (R1-R8): R1, R2, R3, R4, R5, R6, R7, R8
 * Left side (L1-L8): L1, L2, L3, L4, L5, L6, L7, L8
 * 
 * Giga R1 Pin Mapping:
 * R1 -> Pin 23
 * R2 -> Pin 25
 * R3 -> Pin 27
 * R4 -> Pin 29
 * R5 -> Pin 31
 * R6 -> Pin 33
 * R7 -> Pin 35
 * R8 -> Pin 37
 * L8 -> Pin 39
 * L7 -> Pin 41
 * L6 -> Pin 43
 * L5 -> Pin 45
 * L4 -> Pin 47
 * L3 -> Pin 49
 * L2 -> Pin 51
 * L1 -> Pin 53
 */

#ifndef CONFIG_TEMPLATE_H
#define CONFIG_TEMPLATE_H

// ===== SENSOR POSITION CONFIGURATION =====
// Uncomment ONE of the following lines for each sensor node:

// Right Side Sensors
// #define SENSOR_POSITION "R1"
// #define SENSOR_POSITION "R2"
// #define SENSOR_POSITION "R3"
// #define SENSOR_POSITION "R4"
// #define SENSOR_POSITION "R5"
// #define SENSOR_POSITION "R6"
// #define SENSOR_POSITION "R7"
// #define SENSOR_POSITION "R8"

// Left Side Sensors
// #define SENSOR_POSITION "L1"
// #define SENSOR_POSITION "L2"
// #define SENSOR_POSITION "L3"
// #define SENSOR_POSITION "L4"
// #define SENSOR_POSITION "L5"
// #define SENSOR_POSITION "L6"
// #define SENSOR_POSITION "L7"
// #define SENSOR_POSITION "L8"

// ===== DEFAULT DETECTION PARAMETERS =====
// These can be adjusted per sensor if needed
#ifndef DEFAULT_ZERO_REFERENCE
#define DEFAULT_ZERO_REFERENCE 0      // Default zero reference in cm
#endif

#ifndef DEFAULT_THRESHOLD_CM
#define DEFAULT_THRESHOLD_CM 5         // Default detection threshold in cm
#endif

// ===== PIN CONFIGURATION =====
#ifndef OUTPUT_PIN
#define OUTPUT_PIN 2                   // Digital output pin for pothole detection
#endif

#ifndef OUTPUT_DURATION_MS
#define OUTPUT_DURATION_MS 2000        // Output HIGH duration in milliseconds
#endif

// ===== I2C CONFIGURATION =====
#ifndef TFLUNA_I2C_ADDR
#define TFLUNA_I2C_ADDR 0x10          // TFLuna I2C address
#endif

// ===== BLE CONFIGURATION =====
#ifndef BLE_SERVICE_UUID
#define BLE_SERVICE_UUID "12340000-1234-5678-9abc-def012345678"
#endif

#ifndef DISTANCE_CHAR_UUID
#define DISTANCE_CHAR_UUID "12340001-1234-5678-9abc-def012345678"
#endif

#ifndef ZERO_REF_CHAR_UUID
#define ZERO_REF_CHAR_UUID "12340002-1234-5678-9abc-def012345678"
#endif

#ifndef THRESHOLD_CHAR_UUID
#define THRESHOLD_CHAR_UUID "12340003-1234-5678-9abc-def012345678"
#endif

#ifndef STATUS_CHAR_UUID
#define STATUS_CHAR_UUID "12340004-1234-5678-9abc-def012345678"
#endif

#endif // CONFIG_TEMPLATE_H
