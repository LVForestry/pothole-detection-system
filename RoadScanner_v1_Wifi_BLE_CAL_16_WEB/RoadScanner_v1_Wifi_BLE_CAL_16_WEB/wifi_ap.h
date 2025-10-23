#ifndef WIFI_AP_H
#define WIFI_AP_H

#include <Arduino.h>
#include <IPAddress.h>

void wifi_ap_init();
void wifi_ap_handleClients();
IPAddress wifi_ap_ip();
bool wifi_ap_isActive();

#endif // WIFI_AP_H