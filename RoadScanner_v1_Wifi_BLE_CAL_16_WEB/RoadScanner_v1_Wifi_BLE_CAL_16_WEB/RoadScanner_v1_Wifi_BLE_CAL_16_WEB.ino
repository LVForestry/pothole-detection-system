// RoadScanner_v1_Wifi_BLE.ino (Giga main) - modified startup sequence
#include "globals.h"
#include "config.h"
#include "wifi_ap.h"
#include "ble.h"
#include "usb_mac.h"
#include "sensor_manager.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("RoadScanner Giga starting...");

  // 1) init BLE
  ble_init();

  // 2) init USB (mount) so we can read liste.txt and .mac files
  usb_init();

  // 3) process sensor list: read liste.txt and initialise each sensor (MAC errors logged on Serial)
  sensor_manager_init();

  // 4) when sensors processed, start WiFi AP and serve page
  wifi_ap_init();   // starts AP (BLE already inited)
}

void loop() {
  // Process BLE events first so notifications are handled promptly
  ble_update();

  // Handle incoming HTTP clients (serves page and calibration endpoints)
  wifi_ap_handleClients();

  // small yield
  delay(1);
}