/**
 * Arduino Nano 33 BLE Sensor Node Firmware
 * For Pothole Detection System
 * 
 * Functionality:
 * - Reads distance from TFLuna LIDAR sensor via I2C
 * - Detects potholes deeper than configurable threshold (default 5cm)
 * - Outputs HIGH on pin 2 for 2 seconds when pothole detected
 * - Provides BLE service for real-time monitoring and configuration
 * - Stores zero reference and threshold in flash memory
 * - Advertises unique BLE name based on sensor position
 */

#include <ArduinoBLE.h>
#include <Wire.h>
#include <FlashStorage.h>

// ===== CONFIGURATION =====
// Set the sensor position here (e.g., "R1", "R2", ..., "R8", "L1", ..., "L8")
// This should be configured before uploading to each sensor
#define SENSOR_POSITION "L2"

// TFLuna I2C Configuration
#define TFLUNA_I2C_ADDR 0x10
#define TFLUNA_CMD_TRIGGER 0x04
#define TFLUNA_REG_DIST_LOW 0x00
#define TFLUNA_REG_DIST_HIGH 0x01

// Pin Configuration
#define OUTPUT_PIN 2
#define OUTPUT_DURATION_MS 2000

// Detection Configuration (defaults)
#define DEFAULT_ZERO_REFERENCE 0
#define DEFAULT_THRESHOLD_CM 5

// BLE Configuration
#define BLE_SERVICE_UUID "12340000-1234-5678-9abc-def012345678"
#define DISTANCE_CHAR_UUID "12340001-1234-5678-9abc-def012345678"
#define ZERO_REF_CHAR_UUID "12340002-1234-5678-9abc-def012345678"
#define THRESHOLD_CHAR_UUID "12340003-1234-5678-9abc-def012345678"
#define STATUS_CHAR_UUID "12340004-1234-5678-9abc-def012345678"

// ===== FLASH STORAGE =====
struct SensorConfig {
  int16_t zeroReference;  // Zero reference distance in cm
  int16_t thresholdCm;    // Pothole detection threshold in cm
  bool valid;             // Flag to check if stored data is valid
};

FlashStorage(configStorage, SensorConfig);

// ===== GLOBAL VARIABLES =====
SensorConfig config;
int16_t currentDistance = 0;
bool potholeDetected = false;
unsigned long potholeDetectedTime = 0;
unsigned long lastMeasurementTime = 0;
const unsigned long MEASUREMENT_INTERVAL_MS = 100; // 10Hz measurement rate

// BLE Service and Characteristics
BLEService sensorService(BLE_SERVICE_UUID);
BLEIntCharacteristic distanceChar(DISTANCE_CHAR_UUID, BLERead | BLENotify);
BLEIntCharacteristic zeroRefChar(ZERO_REF_CHAR_UUID, BLERead | BLEWrite);
BLEIntCharacteristic thresholdChar(THRESHOLD_CHAR_UUID, BLERead | BLEWrite);
BLEStringCharacteristic statusChar(STATUS_CHAR_UUID, BLERead, 64);

// ===== FUNCTION DECLARATIONS =====
void loadConfiguration();
void saveConfiguration();
int16_t readTFLunaDistance();
void checkPotholeDetection();
void updateOutputPin();
void setupBLE();
void handleBLEEvents();

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial or timeout
  
  Serial.println("=== Arduino Nano 33 BLE Sensor Node ===");
  Serial.print("Sensor Position: ");
  Serial.println(SENSOR_POSITION);
  
  // Configure output pin
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
  
  // Initialize I2C for TFLuna
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C speed
  
  // Load configuration from flash
  loadConfiguration();
  
  // Initialize BLE
  setupBLE();
  
  Serial.println("Setup complete. Starting main loop...");
}

// ===== MAIN LOOP =====
void loop() {
  // Poll for BLE events
  BLE.poll();
  
  // Handle BLE characteristic writes
  handleBLEEvents();
  
  // Read distance from sensor at regular intervals
  if (millis() - lastMeasurementTime >= MEASUREMENT_INTERVAL_MS) {
    lastMeasurementTime = millis();
    currentDistance = readTFLunaDistance();
    
    // Update BLE characteristic
    distanceChar.writeValue(currentDistance);
    
    // Check for pothole detection
    checkPotholeDetection();
  }
  
  // Update output pin state
  updateOutputPin();
}

// ===== CONFIGURATION MANAGEMENT =====
void loadConfiguration() {
  config = configStorage.read();
  
  // Check if stored configuration is valid
  if (!config.valid) {
    Serial.println("No valid configuration found. Using defaults.");
    config.zeroReference = DEFAULT_ZERO_REFERENCE;
    config.thresholdCm = DEFAULT_THRESHOLD_CM;
    config.valid = true;
    saveConfiguration();
  } else {
    Serial.println("Configuration loaded from flash:");
  }
  
  Serial.print("  Zero Reference: ");
  Serial.print(config.zeroReference);
  Serial.println(" cm");
  Serial.print("  Threshold: ");
  Serial.print(config.thresholdCm);
  Serial.println(" cm");
}

void saveConfiguration() {
  config.valid = true;
  configStorage.write(config);
  Serial.println("Configuration saved to flash.");
}

// ===== TFLUNA LIDAR COMMUNICATION =====
int16_t readTFLunaDistance() {
  int16_t distance = -1;
  
  // Request data from TFLuna
  Wire.beginTransmission(TFLUNA_I2C_ADDR);
  Wire.write(TFLUNA_REG_DIST_LOW);
  
  if (Wire.endTransmission(false) != 0) {
    Serial.println("Error: Failed to communicate with TFLuna");
    return -1;
  }
  
  // Read 2 bytes (distance is 16-bit)
  Wire.requestFrom(TFLUNA_I2C_ADDR, 2);
  
  if (Wire.available() >= 2) {
    uint8_t distLow = Wire.read();
    uint8_t distHigh = Wire.read();
    distance = (distHigh << 8) | distLow;
  } else {
    Serial.println("Error: Insufficient data from TFLuna");
    return -1;
  }
  
  return distance;
}

// ===== POTHOLE DETECTION =====
void checkPotholeDetection() {
  if (currentDistance < 0) {
    // Invalid reading, skip detection
    return;
  }
  
  // Calculate depth relative to zero reference
  int16_t depthBelowReference = currentDistance - config.zeroReference;
  
  // Check if depth exceeds threshold (pothole detected)
  // A pothole is detected when the distance increases beyond the threshold
  // (sensor is farther from the road surface)
  if (depthBelowReference > config.thresholdCm) {
    if (!potholeDetected) {
      potholeDetected = true;
      potholeDetectedTime = millis();
      
      Serial.print("POTHOLE DETECTED! Distance: ");
      Serial.print(currentDistance);
      Serial.print(" cm, Depth: ");
      Serial.print(depthBelowReference);
      Serial.println(" cm");
    }
  }
}

// ===== OUTPUT PIN CONTROL =====
void updateOutputPin() {
  if (potholeDetected) {
    digitalWrite(OUTPUT_PIN, HIGH);
    
    // Check if output duration has elapsed
    if (millis() - potholeDetectedTime >= OUTPUT_DURATION_MS) {
      digitalWrite(OUTPUT_PIN, LOW);
      potholeDetected = false;
      Serial.println("Output pin reset to LOW");
    }
  }
}

// ===== BLE SETUP =====
void setupBLE() {
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1); // Halt if BLE fails
  }
  
  // Create BLE device name
  String deviceName = "NanoTFluna-";
  deviceName += SENSOR_POSITION;
  
  BLE.setLocalName(deviceName.c_str());
  BLE.setDeviceName(deviceName.c_str());
  
  // Set advertised service
  BLE.setAdvertisedService(sensorService);
  
  // Add characteristics to service
  sensorService.addCharacteristic(distanceChar);
  sensorService.addCharacteristic(zeroRefChar);
  sensorService.addCharacteristic(thresholdChar);
  sensorService.addCharacteristic(statusChar);
  
  // Add service
  BLE.addService(sensorService);
  
  // Initialize characteristic values
  distanceChar.writeValue(0);
  zeroRefChar.writeValue(config.zeroReference);
  thresholdChar.writeValue(config.thresholdCm);
  statusChar.writeValue("Ready");
  
  // Start advertising
  BLE.advertise();
  
  Serial.print("BLE initialized. Device name: ");
  Serial.println(deviceName);
  Serial.println("Advertising started.");
}

// ===== BLE EVENT HANDLING =====
void handleBLEEvents() {
  // Check if zero reference was written
  if (zeroRefChar.written()) {
    int16_t newZeroRef = zeroRefChar.value();
    config.zeroReference = newZeroRef;
    saveConfiguration();
    
    Serial.print("Zero reference updated to: ");
    Serial.print(newZeroRef);
    Serial.println(" cm");
    
    statusChar.writeValue("Zero ref updated");
  }
  
  // Check if threshold was written
  if (thresholdChar.written()) {
    int16_t newThreshold = thresholdChar.value();
    config.thresholdCm = newThreshold;
    saveConfiguration();
    
    Serial.print("Threshold updated to: ");
    Serial.print(newThreshold);
    Serial.println(" cm");
    
    statusChar.writeValue("Threshold updated");
  }
}
