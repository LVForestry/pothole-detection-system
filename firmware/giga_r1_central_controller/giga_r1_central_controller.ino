/*
 * Arduino Giga R1 Central Controller Firmware
 * Pothole Detection System
 * 
 * This firmware manages:
 * - 16 digital input pins monitoring for sensor node HIGH signals
 * - Serial logging of detection events
 * - BLE communication for sensor polling and configuration
 * - WiFi Access Point for web dashboard
 * - Web server for real-time monitoring and configuration
 */

#include <WiFi.h>
#include <ArduinoBLE.h>

// Pin definitions for 16 sensor inputs
const int SENSOR_PINS[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53};
const int NUM_SENSORS = 16;

// Sensor names mapping
const char* SENSOR_NAMES[] = {
  "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8",
  "L8", "L7", "L6", "L5", "L4", "L3", "L2", "L1"
};

// BLE device names for sensor nodes
const char* BLE_DEVICE_NAMES[] = {
  "NanoTFluna-R1", "NanoTFluna-R2", "NanoTFluna-R3", "NanoTFluna-R4",
  "NanoTFluna-R5", "NanoTFluna-R6", "NanoTFluna-R7", "NanoTFluna-R8",
  "NanoTFluna-L8", "NanoTFluna-L7", "NanoTFluna-L6", "NanoTFluna-L5",
  "NanoTFluna-L4", "NanoTFluna-L3", "NanoTFluna-L2", "NanoTFluna-L1"
};

// Store sensor states
bool sensorStates[NUM_SENSORS] = {false};
bool lastSensorStates[NUM_SENSORS] = {false};

// WiFi AP configuration
const char* AP_SSID = "Pothole-Detection-AP";
const char* AP_PASSWORD = "";  // Open network
WiFiServer server(80);

// BLE service and characteristic UUIDs (standardized for this system)
const char* BLE_SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* BLE_DISTANCE_CHAR_UUID = "19b10001-e8f2-537e-4f6c-d104768a1214";
const char* BLE_ZERO_REF_CHAR_UUID = "19b10002-e8f2-537e-4f6c-d104768a1214";
const char* BLE_THRESHOLD_CHAR_UUID = "19b10003-e8f2-537e-4f6c-d104768a1214";

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial or timeout
  
  Serial.println("Arduino Giga R1 Central Controller");
  Serial.println("Pothole Detection System");
  Serial.println("================================");
  
  // Initialize sensor input pins
  initializePins();
  
  // Initialize BLE
  initializeBLE();
  
  // Initialize WiFi Access Point
  initializeWiFi();
  
  Serial.println("System initialized successfully");
  Serial.println("Ready to monitor sensors");
}

void loop() {
  // Monitor digital input pins for HIGH signals
  monitorSensorPins();
  
  // Handle web server requests
  handleWebServer();
  
  // Small delay to prevent CPU overload
  delay(10);
}

void initializePins() {
  Serial.println("Initializing sensor input pins...");
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
    Serial.print("Pin ");
    Serial.print(SENSOR_PINS[i]);
    Serial.print(" (");
    Serial.print(SENSOR_NAMES[i]);
    Serial.println(") configured as INPUT");
  }
  Serial.println("All pins initialized");
}

void initializeBLE() {
  Serial.println("Initializing BLE...");
  if (!BLE.begin()) {
    Serial.println("ERROR: Failed to initialize BLE!");
    return;
  }
  Serial.println("BLE initialized successfully");
  Serial.println("Ready to scan for sensor nodes");
}

void initializeWiFi() {
  Serial.println("Initializing WiFi Access Point...");
  
  // Set WiFi to AP mode
  WiFi.mode(WIFI_AP);
  
  // Create open AP (no password)
  bool apCreated = WiFi.beginAP(AP_SSID);
  
  if (apCreated) {
    Serial.println("WiFi Access Point created successfully");
    Serial.print("SSID: ");
    Serial.println(AP_SSID);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Start web server
    server.begin();
    Serial.println("Web server started on port 80");
  } else {
    Serial.println("ERROR: Failed to create WiFi Access Point!");
  }
}

void monitorSensorPins() {
  bool stateChanged = false;
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorStates[i] = digitalRead(SENSOR_PINS[i]);
    
    // Check for state change (LOW to HIGH transition)
    if (sensorStates[i] == HIGH && lastSensorStates[i] == LOW) {
      Serial.println("================================");
      Serial.println("POTHOLE DETECTED!");
      Serial.print("Sensor: ");
      Serial.print(SENSOR_NAMES[i]);
      Serial.print(" (Pin ");
      Serial.print(SENSOR_PINS[i]);
      Serial.println(")");
      Serial.print("Timestamp: ");
      Serial.print(millis());
      Serial.println(" ms");
      Serial.println("================================");
      stateChanged = true;
    }
    
    lastSensorStates[i] = sensorStates[i];
  }
}

void handleWebServer() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New web client connected");
    String currentLine = "";
    String request = "";
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // End of HTTP request, process it
            processWebRequest(client, request);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void processWebRequest(WiFiClient &client, String request) {
  Serial.print("Request: ");
  Serial.println(request.substring(0, request.indexOf('\n')));
  
  // Parse request type
  if (request.indexOf("GET / ") >= 0) {
    serveDashboard(client);
  } else if (request.indexOf("GET /api/sensors") >= 0) {
    serveSensorStates(client);
  } else if (request.indexOf("GET /api/poll/") >= 0) {
    int sensorIndex = parseSensorIndex(request);
    if (sensorIndex >= 0) {
      pollSensor(client, sensorIndex);
    } else {
      send404(client);
    }
  } else if (request.indexOf("POST /api/configure/") >= 0) {
    int sensorIndex = parseSensorIndex(request);
    if (sensorIndex >= 0) {
      configureSensor(client, sensorIndex, request);
    } else {
      send404(client);
    }
  } else {
    send404(client);
  }
}

void serveDashboard(WiFiClient &client) {
  sendHTTPHeader(client, "text/html");
  
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Pothole Detection Dashboard</title>");
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f0f0f0; }");
  client.println("h1 { color: #333; text-align: center; }");
  client.println(".container { max-width: 1200px; margin: 0 auto; }");
  client.println(".sensor-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 15px; margin-top: 20px; }");
  client.println(".sensor-card { background: white; padding: 15px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }");
  client.println(".sensor-card h3 { margin: 0 0 10px 0; color: #555; }");
  client.println(".status { padding: 8px; border-radius: 4px; text-align: center; font-weight: bold; margin: 10px 0; }");
  client.println(".status.high { background: #ff4444; color: white; }");
  client.println(".status.low { background: #44ff44; color: #333; }");
  client.println(".button { display: inline-block; padding: 8px 16px; margin: 5px 2px; background: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; text-decoration: none; }");
  client.println(".button:hover { background: #0056b3; }");
  client.println(".button.config { background: #28a745; }");
  client.println(".button.config:hover { background: #1e7e34; }");
  client.println(".info { font-size: 14px; color: #666; margin-top: 10px; }");
  client.println(".header { text-align: center; margin-bottom: 20px; padding: 20px; background: white; border-radius: 8px; }");
  client.println(".refresh-btn { margin: 10px auto; display: block; padding: 12px 24px; font-size: 16px; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<div class='container'>");
  client.println("<div class='header'>");
  client.println("<h1>Pothole Detection System</h1>");
  client.println("<p>Real-time monitoring dashboard</p>");
  client.println("<button class='button refresh-btn' onclick='location.reload()'>Refresh Status</button>");
  client.println("</div>");
  client.println("<div class='sensor-grid' id='sensorGrid'>");
  
  // Generate sensor cards
  for (int i = 0; i < NUM_SENSORS; i++) {
    client.print("<div class='sensor-card'>");
    client.print("<h3>Sensor ");
    client.print(SENSOR_NAMES[i]);
    client.print("</h3>");
    client.print("<div class='status ");
    client.print(sensorStates[i] ? "high" : "low");
    client.print("'>");
    client.print(sensorStates[i] ? "ALERT - HIGH" : "Normal - LOW");
    client.println("</div>");
    client.print("<div class='info'>Pin: ");
    client.print(SENSOR_PINS[i]);
    client.print(" | BLE: ");
    client.print(BLE_DEVICE_NAMES[i]);
    client.println("</div>");
    client.print("<button class='button' onclick='pollSensor(");
    client.print(i);
    client.print(")'>Poll Sensor</button>");
    client.print("<button class='button config' onclick='configureSensor(");
    client.print(i);
    client.print(")'>Configure</button>");
    client.println("</div>");
  }
  
  client.println("</div>");
  client.println("</div>");
  client.println("<script>");
  client.println("function pollSensor(index) {");
  client.println("  fetch('/api/poll/' + index)");
  client.println("    .then(r => r.json())");
  client.println("    .then(data => {");
  client.println("      alert('Sensor ' + data.name + '\\nDistance: ' + data.distance + ' cm\\nStatus: ' + data.status);");
  client.println("    })");
  client.println("    .catch(e => alert('Failed to poll sensor: ' + e));");
  client.println("}");
  client.println("function configureSensor(index) {");
  client.println("  const zero = prompt('Enter zero reference value (cm):');");
  client.println("  const threshold = prompt('Enter threshold value (cm):');");
  client.println("  if (zero && threshold) {");
  client.println("    fetch('/api/configure/' + index, {");
  client.println("      method: 'POST',");
  client.println("      headers: {'Content-Type': 'application/json'},");
  client.println("      body: JSON.stringify({zero: parseFloat(zero), threshold: parseFloat(threshold)})");
  client.println("    })");
  client.println("    .then(r => r.json())");
  client.println("    .then(data => {");
  client.println("      alert('Configuration ' + (data.success ? 'successful' : 'failed') + '\\n' + data.message);");
  client.println("    })");
  client.println("    .catch(e => alert('Failed to configure sensor: ' + e));");
  client.println("  }");
  client.println("}");
  client.println("setInterval(() => location.reload(), 5000);");
  client.println("</script>");
  client.println("</body>");
  client.println("</html>");
}

void serveSensorStates(WiFiClient &client) {
  sendHTTPHeader(client, "application/json");
  
  client.println("{");
  client.println("  \"sensors\": [");
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    client.print("    {");
    client.print("\"index\": ");
    client.print(i);
    client.print(", \"name\": \"");
    client.print(SENSOR_NAMES[i]);
    client.print("\", \"pin\": ");
    client.print(SENSOR_PINS[i]);
    client.print(", \"state\": ");
    client.print(sensorStates[i] ? "true" : "false");
    client.print(", \"ble_name\": \"");
    client.print(BLE_DEVICE_NAMES[i]);
    client.print("\"}");
    if (i < NUM_SENSORS - 1) client.println(",");
    else client.println();
  }
  
  client.println("  ]");
  client.println("}");
}

void pollSensor(WiFiClient &client, int sensorIndex) {
  Serial.print("Polling sensor: ");
  Serial.println(SENSOR_NAMES[sensorIndex]);
  
  // Attempt to connect to BLE device and read distance
  bool bleSuccess = false;
  float distance = 0.0;
  String status = "disconnected";
  
  // Start scanning for the specific device
  BLE.scan();
  
  unsigned long startTime = millis();
  BLEDevice peripheral;
  
  // Scan for up to 5 seconds
  while (millis() - startTime < 5000) {
    peripheral = BLE.available();
    
    if (peripheral) {
      String deviceName = peripheral.localName();
      if (deviceName == BLE_DEVICE_NAMES[sensorIndex]) {
        Serial.print("Found device: ");
        Serial.println(deviceName);
        
        // Stop scanning
        BLE.stopScan();
        
        // Try to connect
        if (peripheral.connect()) {
          Serial.println("Connected to device");
          
          // Discover attributes
          if (peripheral.discoverAttributes()) {
            Serial.println("Attributes discovered");
            
            // Get the distance characteristic
            BLECharacteristic distanceChar = peripheral.characteristic(BLE_DISTANCE_CHAR_UUID);
            
            if (distanceChar && distanceChar.canRead()) {
              if (distanceChar.readValue()) {
                // Assuming distance is sent as float (4 bytes)
                uint8_t data[4];
                distanceChar.readValue(data, 4);
                memcpy(&distance, data, 4);
                bleSuccess = true;
                status = "connected";
                Serial.print("Distance read: ");
                Serial.println(distance);
              }
            }
          }
          
          peripheral.disconnect();
        }
        break;
      }
    }
  }
  
  BLE.stopScan();
  
  // Send response
  sendHTTPHeader(client, "application/json");
  client.println("{");
  client.print("  \"success\": ");
  client.print(bleSuccess ? "true" : "false");
  client.println(",");
  client.print("  \"name\": \"");
  client.print(SENSOR_NAMES[sensorIndex]);
  client.println("\",");
  client.print("  \"ble_device\": \"");
  client.print(BLE_DEVICE_NAMES[sensorIndex]);
  client.println("\",");
  client.print("  \"distance\": ");
  client.print(distance, 2);
  client.println(",");
  client.print("  \"status\": \"");
  client.print(status);
  client.println("\",");
  client.print("  \"pin_state\": ");
  client.println(sensorStates[sensorIndex] ? "\"HIGH\"" : "\"LOW\"");
  client.println("}");
}

void configureSensor(WiFiClient &client, int sensorIndex, String request) {
  Serial.print("Configuring sensor: ");
  Serial.println(SENSOR_NAMES[sensorIndex]);
  
  // Parse JSON body (simple parsing for zero and threshold)
  float zeroRef = 0.0;
  float threshold = 5.0;
  bool parsed = false;
  
  int bodyStart = request.indexOf("\r\n\r\n");
  if (bodyStart > 0) {
    String body = request.substring(bodyStart + 4);
    
    // Simple JSON parsing
    int zeroIdx = body.indexOf("\"zero\"");
    int thresholdIdx = body.indexOf("\"threshold\"");
    
    if (zeroIdx > 0 && thresholdIdx > 0) {
      int zeroValStart = body.indexOf(':', zeroIdx) + 1;
      int zeroValEnd = body.indexOf(',', zeroValStart);
      if (zeroValEnd < 0) zeroValEnd = body.indexOf('}', zeroValStart);
      
      int thresholdValStart = body.indexOf(':', thresholdIdx) + 1;
      int thresholdValEnd = body.indexOf('}', thresholdValStart);
      if (thresholdValEnd < 0) thresholdValEnd = body.indexOf(',', thresholdValStart);
      
      String zeroStr = body.substring(zeroValStart, zeroValEnd);
      String thresholdStr = body.substring(thresholdValStart, thresholdValEnd);
      
      zeroStr.trim();
      thresholdStr.trim();
      
      zeroRef = zeroStr.toFloat();
      threshold = thresholdStr.toFloat();
      parsed = true;
      
      Serial.print("Parsed - Zero: ");
      Serial.print(zeroRef);
      Serial.print(", Threshold: ");
      Serial.println(threshold);
    }
  }
  
  bool bleSuccess = false;
  String message = "Failed to parse configuration";
  
  if (parsed) {
    // Connect to BLE device and write configuration
    BLE.scan();
    
    unsigned long startTime = millis();
    BLEDevice peripheral;
    
    while (millis() - startTime < 5000) {
      peripheral = BLE.available();
      
      if (peripheral) {
        String deviceName = peripheral.localName();
        if (deviceName == BLE_DEVICE_NAMES[sensorIndex]) {
          Serial.print("Found device: ");
          Serial.println(deviceName);
          
          BLE.stopScan();
          
          if (peripheral.connect()) {
            Serial.println("Connected to device");
            
            if (peripheral.discoverAttributes()) {
              Serial.println("Attributes discovered");
              
              // Write zero reference
              BLECharacteristic zeroChar = peripheral.characteristic(BLE_ZERO_REF_CHAR_UUID);
              if (zeroChar && zeroChar.canWrite()) {
                uint8_t zeroData[4];
                memcpy(zeroData, &zeroRef, 4);
                zeroChar.writeValue(zeroData, 4);
                Serial.println("Zero reference written");
              }
              
              // Write threshold
              BLECharacteristic thresholdChar = peripheral.characteristic(BLE_THRESHOLD_CHAR_UUID);
              if (thresholdChar && thresholdChar.canWrite()) {
                uint8_t thresholdData[4];
                memcpy(thresholdData, &threshold, 4);
                thresholdChar.writeValue(thresholdData, 4);
                Serial.println("Threshold written");
              }
              
              bleSuccess = true;
              message = "Configuration updated successfully";
            }
            
            peripheral.disconnect();
          }
          break;
        }
      }
    }
    
    BLE.stopScan();
    
    if (!bleSuccess) {
      message = "Failed to connect to BLE device";
    }
  }
  
  // Send response
  sendHTTPHeader(client, "application/json");
  client.println("{");
  client.print("  \"success\": ");
  client.print(bleSuccess ? "true" : "false");
  client.println(",");
  client.print("  \"message\": \"");
  client.print(message);
  client.println("\",");
  client.print("  \"sensor\": \"");
  client.print(SENSOR_NAMES[sensorIndex]);
  client.println("\",");
  client.print("  \"zero\": ");
  client.print(zeroRef, 2);
  client.println(",");
  client.print("  \"threshold\": ");
  client.println(threshold, 2);
  client.println("}");
}

int parseSensorIndex(String request) {
  int indexStart = request.indexOf("/api/poll/");
  if (indexStart < 0) {
    indexStart = request.indexOf("/api/configure/");
  }
  
  if (indexStart >= 0) {
    indexStart = request.indexOf('/', indexStart + 5) + 1;
    int indexEnd = request.indexOf(' ', indexStart);
    if (indexEnd < 0) indexEnd = request.indexOf('\n', indexStart);
    
    String indexStr = request.substring(indexStart, indexEnd);
    int index = indexStr.toInt();
    
    if (index >= 0 && index < NUM_SENSORS) {
      return index;
    }
  }
  
  return -1;
}

void sendHTTPHeader(WiFiClient &client, const char* contentType) {
  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(contentType);
  client.println("Connection: close");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

void send404(WiFiClient &client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html><body><h1>404 Not Found</h1></body></html>");
}
