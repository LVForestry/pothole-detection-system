#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Paramètres AP
#define WIFI_AP_SSID     "RoadScanner"
#define WIFI_AP_PASSWORD "RoadScanner0123" // mettre "" pour AP ouvert
#define WIFI_AP_CHANNEL  1

// Valeur informative / par défaut
#define WIFI_AP_IP_ADDR  "192.168.3.1"
#define WIFI_AP_NETMASK  "255.255.255.0"

#endif // CONFIG_H