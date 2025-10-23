#ifndef BLE_H
#define BLE_H

#include <Arduino.h>

// Initialise le module BLE (doit être appelé une fois au startup)
void ble_init();

// Connecte de façon persistente au périphérique MAC (scanne, connecte, s'abonne)
String ble_connectPersistent(const String &mac, unsigned long scanTimeoutMs = 6000, unsigned long notifyTimeoutMs = 4000);

// Déconnecte proprement
void ble_disconnect();

// Indique si connecté
bool ble_isConnected();

// Retourne la dernière valeur lue en entier (-1 si aucune)
long ble_getLastValue();

// Retourne la dernière notification brute reçue (string), vide si aucune
String ble_getLastNotificationString();

// Vide/clear la dernière notification string
void ble_clearLastNotificationString();

// Ecrit une valeur de calibration sur la caractéristique calib (retour JSON string)
String ble_writeCalib(long value);

// Attend une confirmation ASCII du périphérique sous la forme "<name> OK <value>" jusque timeout (ms).
// Retourne true si confirmation reçue et correspondante.
bool ble_waitForAck(const String &expectedName, long expectedValue, unsigned long timeoutMs = 4000);

// Doit être appelé régulièrement pour traiter notifications
void ble_update();

#endif // BLE_H