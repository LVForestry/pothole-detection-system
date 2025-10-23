#include "sensor_manager.h"
#include "usb_mac.h"
#include "ble.h"
#include "globals.h"

// Calibration manuelle via HTTP
String sensor_manager_calibConnect(const String &name) {
  String macFilename = name + ".mac";
  String mac = usb_readFirstLine(macFilename.c_str());
  if (mac.length() == 0) {
    return String("{\"ok\":false,\"error\":\"Fichier MAC manquant ou vide\"}");
  }

  Serial.print("Connexion au capteur pour calibration : ");
  Serial.println(name);

  String r = ble_connectPersistent(mac);
  if (!r.startsWith("{\"ok\":true")) {
    ble_disconnect();
    return String("{\"ok\":false,\"error\":\"Erreur de connexion BLE\"}");
  }

  Serial.print("Calibration réussie pour le capteur : ");
  Serial.println(name);

  ble_disconnect();
  return String("{\"ok\":true}");
}