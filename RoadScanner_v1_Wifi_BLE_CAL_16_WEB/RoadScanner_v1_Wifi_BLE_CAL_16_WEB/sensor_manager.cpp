#include "sensor_manager.h"
#include "usb_mac.h"
#include "ble.h"
#include "globals.h"

#define MAX_SENSORS 32

struct SensorEntry {
  String name;
  long threshold;
  bool initialized;
};

static SensorEntry _sensors[MAX_SENSORS];
static String _sensorErrors[MAX_SENSORS]; // Ajoutez un tableau pour les erreurs
static int _sensorCount = 0;

static void addInitializedSensor(const String &name, long threshold) {
  if (_sensorCount >= MAX_SENSORS) return;
  _sensors[_sensorCount].name = name;
  _sensors[_sensorCount].threshold = threshold;
  _sensors[_sensorCount].initialized = true;
  _sensorErrors[_sensorCount] = ""; // Pas d'erreur
  _sensorCount++;
}

static void addErrorSensor(const String &name, const String &error) {
  if (_sensorCount >= MAX_SENSORS) return;
  _sensors[_sensorCount].name = name;
  _sensors[_sensorCount].threshold = -1;
  _sensors[_sensorCount].initialized = false;
  _sensorErrors[_sensorCount] = error; // Stocker l'erreur
  _sensorCount++;
}

// Clear all the sensors (definition added for missing function)
void clearSensors() {
  for (int i = 0; i < MAX_SENSORS; ++i) {
    _sensors[i].name = String("");
    _sensors[i].threshold = -1;
    _sensors[i].initialized = false;
    _sensorErrors[i] = String("");
  }
  _sensorCount = 0;
}

// Split a string into lines (definition added for missing function)
void splitLines(const String &in, String outLines[], int &outCount, int maxOut) {
  outCount = 0;
  int start = 0;
  while (start < in.length() && outCount < maxOut) {
    int nl = in.indexOf('\n', start);
    String line;
    if (nl < 0) {
      line = in.substring(start);
      start = in.length();
    } else {
      line = in.substring(start, nl);
      start = nl + 1;
    }
    line.trim();
    if (line.length() > 0) {
      outLines[outCount++] = line;
    }
  }
}

void sensor_manager_init() {
  clearSensors();

  String listContent = usb_readFile("liste.txt");
  if (listContent.length() == 0) {
    Serial.println("Pas de fichier liste.txt ou fichier vide");
    return;
  }

  String names[32];
  int ncount = 0;
  splitLines(listContent, names, ncount, 32);

  for (int i = 0; i < ncount; ++i) {
    String name = names[i];
    name.trim();
    if (name.length() == 0) continue;

    String macFilename = name + ".mac";
    String mac = usb_readFirstLine(macFilename.c_str());
    if (mac.length() == 0) {
      Serial.print(name);
      Serial.println(" - Fichier MAC manquant ou vide");
      addErrorSensor(name, "Fichier MAC manquant ou vide");
      continue;
    }

    Serial.print("Connexion au capteur : ");
    Serial.println(name);

    String r = ble_connectPersistent(mac);
    if (!r.startsWith("{\"ok\":true")) {
      Serial.print(name);
      Serial.println(" - Erreur de connexion BLE");
      addErrorSensor(name, "Erreur de connexion BLE");
      ble_disconnect();
      continue;
    }

    String valFile = name;
    String valStr = usb_readFirstLine(valFile.c_str());
    if (valStr.length() == 0) {
      Serial.print(name);
      Serial.println(" - Fichier de valeur manquant ou vide");
      addErrorSensor(name, "Fichier de valeur manquant ou vide");
      ble_disconnect();
      continue;
    }
    long val = valStr.toInt();

    String wr = ble_writeCalib(val);
    if (!wr.startsWith("{\"ok\":true")) {
      Serial.print(name);
      Serial.println(" - Erreur d'écriture BLE");
      addErrorSensor(name, "Erreur d'écriture BLE");
      ble_disconnect();
      continue;
    }

    bool ack = ble_waitForAck(name, val, 5000);
    if (!ack) {
      Serial.print(name);
      Serial.println(" - Pas d'acknowledgment BLE");
      addErrorSensor(name, "Pas d'acknowledgment BLE");
      ble_disconnect();
      continue;
    }

    ble_disconnect();
    addInitializedSensor(name, val);
    Serial.print("Capteur initialisé : ");
    Serial.print(name);
    Serial.print(" avec seuil ");
    Serial.println(val);
  }
}

String sensor_manager_getSummaryJson() {
  String out = "[";
  bool first = true;
  for (int i = 0; i < _sensorCount; ++i) {
    if (!first) out += ",";
    out += "{\"name\":\"" + _sensors[i].name + "\",";
    out += "\"threshold\":" + String(_sensors[i].threshold) + ",";
    out += "\"status\":\"" + String(_sensors[i].initialized ? "initialized" : "not_initialized") + "\",";
    out += "\"error\":\"" + _sensorErrors[i] + "\"}"; // Inclure l'erreur
    first = false;
  }
  out += "]";
  return out;
}