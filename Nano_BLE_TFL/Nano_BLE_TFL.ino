/*
  Nano BLE peripheral for RoadScanner - high-rate continuous sampling (using TFLI2C)

  - Target sampling rate: 100 Hz (period 10 ms). Adjust CONTINUOUS_POLL_MS to tune.
  - Non-blocking alert: OUTPUT_PIN is kept HIGH until alertUntilMs (no blocking delay()).
  - Uses tflI2C.getData(tfDist, tfAddr) for TF-Luna reads (as in your example).
  - Sends one measurement to Giga on BLE connect, supports READ/READASCII and calib writes.
  - Prints samples/sec every second for verification.
*/

#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Wire.h>
#include <TFLI2C.h>

#define DEVICE_NAME "L1"
#define OUTPUT_PIN 2

// Target poll period (ms). For ~100 Hz use 10 ms. Set higher if unstable.
#define CONTINUOUS_POLL_MS 10

// How long to keep OUTPUT_PIN HIGH when alert occurs (ms) - non-blocking
#define ALERT_DURATION_MS 2000
#define HYSTERESIS 5

// BLE UUIDs (same as Giga)
const char SERVICE_UUID[]     = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char CHAR_DIST_UUID[]   = "19b10001-e8f2-537e-4f6c-d104768a1214";
const char CHAR_CALIB_UUID[]  = "19b10002-e8f2-537e-4f6c-d104768a1214";
const char CHAR_CMD_UUID[]    = "19b10003-e8f2-537e-4f6c-d104768a1214";

BLEService sensorService(SERVICE_UUID);
BLECharacteristic distanceChar(CHAR_DIST_UUID, BLERead | BLENotify, 32);
BLECharacteristic calibChar(CHAR_CALIB_UUID, BLEWrite, 32);
BLECharacteristic cmdChar(CHAR_CMD_UUID, BLEWrite, 32);

// TF-Luna objects
TFLI2C tflI2C;
int16_t tfDist = 0;
int16_t tfAddr = 0x3E; // default from your example

volatile long currentThreshold = 200;

// runtime variables for non-blocking alert
unsigned long alertUntilMs = 0;

// stats for sample rate measurement
unsigned long samplesCount = 0;
unsigned long lastSamplesReportMs = 0;

// heartbeat for debug
unsigned long lastHeartbeatMs = 0;
const unsigned long HEARTBEAT_MS = 2000;

// forward declarations
void sendNotificationBuffer(const uint8_t *data, size_t len);
void sendDistanceNotification(long value, bool ascii = false);
void sendAck(long value);

// BLE handlers
void onCalibWritten(BLEDevice /*central*/, BLECharacteristic /*characteristic*/) {
  int len = calibChar.valueLength();
  if (len <= 0) return;
  const unsigned char *buf = (const unsigned char*)calibChar.value();

  long parsed = -1;
  bool parsedOk = false;

  if (len >= 4) {
    uint32_t v = ((uint32_t)buf[0]) | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
    parsed = (long)v;
    parsedOk = true;
  }

  if (!parsedOk) {
    char tmp[32] = {0};
    int copy = min(len, (int)sizeof(tmp)-1);
    memcpy(tmp, buf, copy);
    tmp[copy] = '\0';
    long v = atol(tmp);
    if (v != 0 || (tmp[0] == '0' && tmp[1] == '\0')) {
      parsed = v;
      parsedOk = true;
    }
  }

  if (parsedOk) {
    currentThreshold = parsed;
    Serial.print("Calib written: ");
    Serial.println(parsed);
    sendAck(parsed);
    // after ack we resume continuous sampling (central usually disconnects)
  } else {
    Serial.println("Calib parse failed");
  }
}

void onCmdWritten(BLEDevice /*central*/, BLECharacteristic /*characteristic*/) {
  int len = cmdChar.valueLength();
  if (len <= 0) return;
  char tmp[64] = {0};
  int copy = min(len, (int)sizeof(tmp)-1);
  memcpy(tmp, cmdChar.value(), copy);
  tmp[copy] = '\0';
  String cmd = String(tmp);
  cmd.trim();
  cmd.toUpperCase();

  Serial.print("CMD received: ");
  Serial.println(cmd);

  if (cmd == "READ" || cmd == "REREAD") {
    long d = -1;
    if (tflI2C.getData(tfDist, tfAddr)) d = (long)tfDist;
    if (d >= 0) {
      sendDistanceNotification(d, false);
      Serial.print("Sent READ (binary): ");
      Serial.println(d);
    } else {
      Serial.println("READ: no data from TFLib");
      tflI2C.printStatus();
    }
  } else if (cmd == "READASCII") {
    long d = -1;
    if (tflI2C.getData(tfDist, tfAddr)) d = (long)tfDist;
    if (d >= 0) {
      sendDistanceNotification(d, true);
      Serial.print("Sent READASCII: ");
      Serial.println(d);
    } else {
      Serial.println("READASCII: no data from TFLib");
      tflI2C.printStatus();
    }
  } else {
    Serial.println("Unknown CMD");
  }
}

// notification helpers
void sendNotificationBuffer(const uint8_t *data, size_t len) {
  if (len == 0) distanceChar.setValue((const uint8_t*)NULL, 0);
  else distanceChar.setValue(data, len);
  // ensure BLE stack processes and sends notifications
  BLE.poll();
}

void sendDistanceNotification(long value, bool ascii) {
  if (ascii) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%ld", value);
    sendNotificationBuffer((const uint8_t*)buf, n);
  } else {
    uint8_t b[4];
    uint32_t v32 = (uint32_t)((unsigned long)value);
    b[0] = (v32 >> 0) & 0xFF;
    b[1] = (v32 >> 8) & 0xFF;
    b[2] = (v32 >> 16) & 0xFF;
    b[3] = (v32 >> 24) & 0xFF;
    sendNotificationBuffer(b, 4);
  }
}

void sendAck(long value) {
  char ack[64];
  int n = snprintf(ack, sizeof(ack), "%s OK %ld", DEVICE_NAME, value);
  sendNotificationBuffer((const uint8_t*)ack, (size_t)n);
}

void setup() {
  Serial.begin(115200);
  

  Serial.println("=== Nano TF-Luna high-rate sampling ===");

  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

  Wire.begin(); // used by TFLI2C
  Serial.println("Wire begun");

  // No explicit init required for TFLI2C in your example
  Serial.println("TFLI2C: ready to use getData()");

  if (!BLE.begin()) {
    Serial.println("BLE init failed!");
    while (1) delay(1000);
  }

  BLE.setLocalName(DEVICE_NAME);
  BLE.setDeviceName(DEVICE_NAME);
  BLE.setAdvertisedService(sensorService);

  distanceChar.setValue((const uint8_t*)0, 0);
  calibChar.setEventHandler(BLEWritten, onCalibWritten);
  cmdChar.setEventHandler(BLEWritten, onCmdWritten);

  sensorService.addCharacteristic(distanceChar);
  sensorService.addCharacteristic(calibChar);
  sensorService.addCharacteristic(cmdChar);

  BLE.addService(sensorService);
  BLE.advertise();

  Serial.println("BLE peripheral advertising");
  Serial.print("Initial threshold: ");
  Serial.println(currentThreshold);

  lastSamplesReportMs = millis();
  lastHeartbeatMs = millis();
}

void loop() {
  unsigned long now = millis();

  // heartbeat
  if (now - lastHeartbeatMs >= HEARTBEAT_MS) {
    lastHeartbeatMs = now;
    Serial.println("LOOP heartbeat");
  }

  // Sample-rate reporting once per second
  if (now - lastSamplesReportMs >= 1000) {
    Serial.print("Samples/sec = ");
    Serial.println(samplesCount);
    samplesCount = 0;
    lastSamplesReportMs = now;
  }

  // Handle BLE central session if any
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // pause continuous loop: send one measurement on connect
    long first = -1;
    if (tflI2C.getData(tfDist, tfAddr)) first = (long)tfDist;
    if (first >= 0) {
      sendDistanceNotification(first, false);
      Serial.print("Sent initial measurement on connect: ");
      Serial.println(first);
    } else {
      Serial.println("No initial measurement available on connect");
      tflI2C.printStatus();
    }

    // stay in BLE session and process writes via handlers
    while (central.connected()) {
      BLE.poll();
      // small yield
      delay(1);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    // resume continuous sampling automatically
  } else {
    // Continuous sampling at target rate (non-blocking)
    static unsigned long lastPoll = 0;
    if (now - lastPoll >= CONTINUOUS_POLL_MS) {
      lastPoll = now;

      // read TF-Luna via TFLI2C
      long d = -1;
      if (tflI2C.getData(tfDist, tfAddr)) {
        d = (long)tfDist;
      } else {
        // on occasional fail, print status but continue - don't spam every loop
        static unsigned long lastStatusPrint = 0;
        if (now - lastStatusPrint >= 2000) {
          Serial.println("TFLI2C getData() failed - status:");
          tflI2C.printStatus();
          lastStatusPrint = now;
        }
        d = -1;
      }

      if (d >= 0) {
        samplesCount++;
        // non-blocking alert: if over threshold set output and keep until alertUntilMs
        if (d > (currentThreshold + HYSTERESIS)) {
          alertUntilMs = now + ALERT_DURATION_MS;
          digitalWrite(OUTPUT_PIN, HIGH);
          Serial.print("ALERT: d=");
          Serial.print(d);
          Serial.print(" > thresh+Hyst=");
          Serial.println(currentThreshold + HYSTERESIS);
        } else {
          // only clear output if alert period expired
          if (now >= alertUntilMs) {
            digitalWrite(OUTPUT_PIN, LOW);
          }
        }
      } else {
        // invalid read
        // (status printed occasionally above)
      }
    }

    // If alert period expired, ensure pin cleared (in case no new reads triggered earlier)
    if (alertUntilMs > 0 && now >= alertUntilMs) {
      digitalWrite(OUTPUT_PIN, LOW);
      alertUntilMs = 0;
    }

    // Allow BLE stack to run frequently
    BLE.poll();

    // tiny yield - keep loop tight for high rate
    delay(1);
  }
}