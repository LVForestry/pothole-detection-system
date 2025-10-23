#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

// Initialise le manager de capteurs : lit /usb/liste.txt puis pour chaque capteur exécute la séquence:
// read mac, connect BLE, read value file, send value to nano, wait ack, disconnect.
// Les résultats (capteurs initialisés) sont stockés en mémoire pour la page web.
void sensor_manager_init();

// Retourne un JSON (string) listant les capteurs initialisés et leurs seuils, ex:
// [{"name":"L1","threshold":40}, {"name":"L2","threshold":...}]
String sensor_manager_getSummaryJson();

// Pour la calibration manuelle via HTTP : connecte au capteur (utilise fichier X.mac)
// retourne JSON {"ok":true} ou {"ok":false,"error":"..."}
String sensor_manager_calibConnect(const String &name);

// Lit une valeur depuis le capteur connecté (ou attend notification si nécessaire).
// Retourne JSON {"ok":true,"value":"..."} ou {"ok":false,"error":"..."}
String sensor_manager_calibRead(const String &name, unsigned long timeoutMs = 4000);

// Envoie la valeur choisie au capteur connecté (set threshold) et attend ack, si OK écrit le fichier valeur sur la clé USB.
// Retourne JSON {"ok":true} ou {"ok":false,"error":"..."}
String sensor_manager_calibSetAndPersist(const String &name, long value, unsigned long timeoutMs = 4000);

// Déconnecte la session de calibration éventuellement ouverte
void sensor_manager_calibDisconnect(const String &name);

// Helper functions (now declared in the header for proper linkage)
void clearSensors();
void splitLines(const String &in, String outLines[], int &outCount, int maxOut);

#endif // SENSOR_MANAGER_H