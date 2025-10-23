#include "ble.h"
#include "globals.h"
#include <ArduinoBLE.h>

static const char SERVICE_UUID[] = "19b10000-e8f2-537e-4f6c-d104768a1214";
static const char CHAR_DIST_UUID[] = "19b10001-e8f2-537e-4f6c-d104768a1214";
static const char CHAR_CALIB_UUID[] = "19b10002-e8f2-537e-4f6c-d104768a1214";

static bool _initialized = false;
static BLEDevice _peripheral;
static BLECharacteristic _distChar;
static BLECharacteristic _calibChar;
static bool _connected = false;
static long _lastValue = -1;
static String _lastNotifStr = String("");
static unsigned long _lastNotifyMs = 0;

void ble_init() {
  if (_initialized) {
#if ENABLE_DEBUG
    Serial.println("ble_init(): already initialized, skipping");
#endif
    return;
  }
  if (!BLE.begin()) {
#if ENABLE_DEBUG
    Serial.println("ble_init(): Failed to start BLE!");
#endif
    _initialized = false;
    return;
  }
  _initialized = true;
#if ENABLE_DEBUG
  Serial.println("ble_init(): BLE started");
#endif
}

bool ble_isConnected() {
  return _connected;
}

long ble_getLastValue() {
  return _lastValue;
}

String ble_getLastNotificationString() {
  return _lastNotifStr;
}

void ble_clearLastNotificationString() {
  _lastNotifStr = String("");
}

// Connect / discover / subscribe (unchanged)
String ble_connectPersistent(const String &macRaw, unsigned long scanTimeoutMs, unsigned long notifyTimeoutMs) {
  if (!_initialized) {
    return String("{\"ok\":false,\"error\":\"BLE_not_started\"}");
  }

  String targetMac;
  for (size_t i=0;i<macRaw.length();++i) {
    char c = macRaw[i];
    if (c==':'||c=='-'||c==' ') continue;
    targetMac += (char)toupper(c);
  }
#if ENABLE_DEBUG
  Serial.print("ble_connectPersistent(): targetMac=");
  Serial.println(targetMac);
#endif

  if (_connected) {
    String existing = _peripheral.address();
    String norm;
    for (size_t i=0;i<existing.length();++i) {
      char c = existing[i];
      if (c==':'||c=='-'||c==' ') continue;
      norm += (char)toupper(c);
    }
    if (norm == targetMac) {
      return String("{\"ok\":true,\"info\":\"already_connected\"}");
    } else {
      ble_disconnect();
    }
  }

  BLE.scan();
  unsigned long startScan = millis();
  bool found = false;
  while (millis() - startScan < scanTimeoutMs) {
    BLEDevice dev = BLE.available();
    if (dev) {
      String adr;
      for (size_t i=0;i<dev.address().length();++i) {
        char c = dev.address()[i];
        if (c==':'||c=='-'||c==' ') continue;
        adr += (char)toupper(c);
      }
#if ENABLE_DEBUG
      Serial.print("Found device: "); Serial.print(dev.address()); Serial.print(" name="); Serial.println(dev.localName());
#endif
      if (adr == targetMac) {
        _peripheral = dev;
        found = true;
        BLE.stopScan();
        break;
      }
    }
    delay(5);
  }
  if (!found) {
#if ENABLE_DEBUG
    Serial.println("ble_connectPersistent(): device not found");
#endif
    return String("{\"ok\":false,\"error\":\"device_not_found\"}");
  }

  if (!_peripheral.connect()) {
#if ENABLE_DEBUG
    Serial.println("ble_connectPersistent(): connect failed");
#endif
    return String("{\"ok\":false,\"error\":\"connect_failed\"}");
  }
  _connected = true;
#if ENABLE_DEBUG
  Serial.print("ble_connectPersistent(): connected to ");
  Serial.println(_peripheral.address());
#endif

  bool discovered = false;
  if (_peripheral.discoverAttributes()) {
    discovered = true;
#if ENABLE_DEBUG
    Serial.println("ble_connectPersistent(): attributes discovered");
#endif
  } else {
#if ENABLE_DEBUG
    Serial.println("ble_connectPersistent(): discoverAttributes() false or not supported");
#endif
  }

  if (discovered) {
    _distChar = _peripheral.characteristic(CHAR_DIST_UUID);
    _calibChar = _peripheral.characteristic(CHAR_CALIB_UUID);
#if ENABLE_DEBUG
    if (_distChar) {
      Serial.println("ble_connectPersistent(): found distance characteristic");
      Serial.print("  canRead="); Serial.print(_distChar.canRead());
      Serial.print(" canSubscribe="); Serial.println(_distChar.canSubscribe());
    } else {
      Serial.println("ble_connectPersistent(): distance characteristic NOT found");
    }
    if (_calibChar) {
      Serial.println("ble_connectPersistent(): found calib characteristic");
      Serial.print("  canWrite="); Serial.print(_calibChar.canWrite());
      Serial.print(" canRead="); Serial.println(_calibChar.canRead());
    } else {
      Serial.println("ble_connectPersistent(): calib characteristic NOT found");
    }
#endif
    if (_distChar && _distChar.canSubscribe()) {
      if (_distChar.subscribe()) {
#if ENABLE_DEBUG
        Serial.println("ble_connectPersistent(): subscribed to distance notifications");
#endif
      } else {
#if ENABLE_DEBUG
        Serial.println("ble_connectPersistent(): subscribe failed");
#endif
      }
    }
  }

  _lastValue = -1;
  _lastNotifStr = String("");
  _lastNotifyMs = millis();

  return String("{\"ok\":true,\"info\":\"connected\"}");
}

void ble_disconnect() {
  if (!_connected) return;
#if ENABLE_DEBUG
  Serial.print("ble_disconnect(): disconnecting from ");
  Serial.println(_peripheral.address());
#endif
  if (_distChar && _distChar.canSubscribe()) {
    _distChar.unsubscribe();
  }
  _peripheral.disconnect();
  _connected = false;
  _lastValue = -1;
  _lastNotifStr = String("");
#if ENABLE_DEBUG
  Serial.println("ble_disconnect(): disconnected");
#endif
}

// Write calibration: try binary (32-bit LE) then ASCII fallback if needed
String ble_writeCalib(long value) {
  if (!_connected) {
    return String("{\"ok\":false,\"error\":\"not_connected\"}");
  }
  if (!_calibChar) {
    return String("{\"ok\":false,\"error\":\"calib_characteristic_not_found\"}");
  }
  if (!_calibChar.canWrite()) {
    return String("{\"ok\":false,\"error\":\"calib_not_writable\"}");
  }

  // Try binary little-endian 32-bit first
  uint32_t v32 = (uint32_t)((unsigned long)value);
  int res = _calibChar.writeValue((const uint8_t*)&v32, sizeof(v32));
  if (res > 0) {
#if ENABLE_DEBUG
    Serial.print("ble_writeCalib(): wrote binary value=");
    Serial.println(value);
#endif
    return String("{\"ok\":true}");
  }

  // If binary write failed, try ASCII fallback
  char asciiBuf[16];
  int n = snprintf(asciiBuf, sizeof(asciiBuf), "%lu", (unsigned long)value);
  if (n > 0) {
    int res2 = _calibChar.writeValue((const uint8_t*)asciiBuf, (size_t)n);
    if (res2 > 0) {
#if ENABLE_DEBUG
      Serial.print("ble_writeCalib(): wrote ascii value=");
      Serial.println(asciiBuf);
#endif
      return String("{\"ok\":true}");
    }
  }

#if ENABLE_DEBUG
  Serial.print("ble_writeCalib(): write failed (binary res=");
  Serial.print(res);
  Serial.print(" ascii res=");
  Serial.print("?)");
  Serial.println("");
#endif
  return String("{\"ok\":false,\"error\":\"write_failed\"}");
}

// Wait for ack "<name> OK <value>" (ASCII) OR numeric value == expectedValue
bool ble_waitForAck(const String &expectedName, long expectedValue, unsigned long timeoutMs) {
  if (!_initialized || !_connected) return false;
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    // let BLE stack process
    BLE.poll();

    // If distChar has updated value, read and translate into _lastNotifStr/_lastValue
    if (_distChar && _distChar.valueUpdated()) {
      int len = _distChar.valueLength();
      const unsigned char *buf = (const unsigned char*)_distChar.value();
      if (buf && len > 0) {
        // Detect printable ASCII
        bool allPrintable = true;
        for (int i = 0; i < len; ++i) {
          if (buf[i] < 9 || buf[i] > 127) { allPrintable = false; break; }
        }
        if (allPrintable) {
          char tmp[128] = {0};
          int copy = min(len, 127);
          memcpy(tmp, buf, copy);
          tmp[copy] = 0;
          String s(tmp);
          s.trim();
          _lastNotifStr = s;
#if ENABLE_DEBUG
          Serial.print("ble_waitForAck(): got notification string = ");
          Serial.println(_lastNotifStr);
#endif
        } else {
          long parsed = 0;
          if (len == 2) {
            parsed = (long)( (int16_t)((buf[1] << 8) | buf[0]) );
          } else if (len >= 4) {
            parsed = (long)( ((uint32_t)buf[0]) | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24) );
          } else {
            char tmp[32] = {0};
            int copy = min(len, 31);
            memcpy(tmp, buf, copy);
            parsed = atol(tmp);
          }
          _lastValue = parsed;
          _lastNotifyMs = millis();
#if ENABLE_DEBUG
          Serial.print("ble_waitForAck(): got numeric notification = ");
          Serial.println(_lastValue);
#endif
        }
      }
    }

    // 1) If ASCII notification present, try parse "<name> OK <value>"
    if (_lastNotifStr.length() > 0) {
      String s = _lastNotifStr;
      s.trim();
      int sp1 = s.indexOf(' ');
      int sp2 = s.indexOf(' ', sp1 + 1);
      if (sp1 > 0 && sp2 > sp1) {
        String name = s.substring(0, sp1);
        String okPart = s.substring(sp1 + 1, sp2);
        String valPart = s.substring(sp2 + 1);
        valPart.trim();
        if (okPart == "OK") {
          long v = valPart.toInt();
          if (name == expectedName && v == expectedValue) {
            _lastNotifStr = String("");
            return true;
          }
        }
      }
      // clear non-matching ASCII to avoid infinite loop on same data
      _lastNotifStr = String("");
    }

    // 2) If numeric notification equals expectedValue -> accept as ack
    if (_lastValue >= 0) {
      if (_lastValue == expectedValue) {
        // consume
        _lastValue = -1;
        return true;
      } else {
        // numeric value but not matching: ignore and continue waiting
        // do not clear immediately (we already consumed it by comparing), but reset to -1
        _lastValue = -1;
      }
    }

    delay(10);
  }
  return false;
}

void ble_update() {
  if (!_initialized) return;
  if (!_connected) return;

  BLE.poll();

  if (_distChar && _distChar.valueUpdated()) {
    int len = _distChar.valueLength();
    const unsigned char *buf = (const unsigned char*)_distChar.value();
    if (buf && len > 0) {
      bool allPrintable = true;
      for (int i = 0; i < len; ++i) {
        if (buf[i] < 9 || buf[i] > 127) { allPrintable = false; break; }
      }
      if (allPrintable) {
        char tmp[128] = {0};
        int copy = min(len, 127);
        memcpy(tmp, buf, copy);
        tmp[copy] = 0;
        String s(tmp);
        s.trim();
        _lastNotifStr = s;
#if ENABLE_DEBUG
        Serial.print("ble_update(): got notification string = ");
        Serial.println(_lastNotifStr);
#endif
      } else {
        long parsed = 0;
        if (len == 2) {
          parsed = (long)( (int16_t)((buf[1] << 8) | buf[0]) );
        } else if (len >= 4) {
          parsed = (long)( ((uint32_t)buf[0]) | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24) );
        } else {
          char tmp[32] = {0};
          int copy = min(len, 31);
          memcpy(tmp, buf, copy);
          parsed = atol(tmp);
        }
        _lastValue = parsed;
        _lastNotifyMs = millis();
#if ENABLE_DEBUG
        Serial.print("ble_update(): got notification value = ");
        Serial.println(_lastValue);
#endif
      }
    }
  }

  if (!_peripheral.connected()) {
#if ENABLE_DEBUG
    Serial.println("ble_update(): peripheral disconnected unexpectedly");
#endif
    _connected = false;
    _lastValue = -1;
    _lastNotifStr = String("");
  }
}