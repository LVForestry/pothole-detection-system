#include "BLEManager.h"

#define SERVICE_UUID    "180A"
#define SEUIL_CHAR_UUID "2A57"
#define DIST_CHAR_UUID  "2A58"

BLEManager::BLEManager() {}

void BLEManager::begin() {
    BLE.begin();
}

void BLEManager::scan() {
    BLE.scan();
}

void BLEManager::stopScan() {
    BLE.stopScan();
}

BLEDevice BLEManager::available() {
    return BLE.available();
}

bool BLEManager::connectToDeviceByName(const char* targetName) {
    stopScan();
    scan();
    unsigned long start = millis();
    while (millis() - start < 6000) {
        BLEDevice dev = BLE.available();
        if (dev && dev.localName() == String(targetName)) {
            if (dev.connect()) {
                if (dev.discoverService(SERVICE_UUID)) {
                    seuilChar = dev.characteristic(SEUIL_CHAR_UUID);
                    distChar  = dev.characteristic(DIST_CHAR_UUID);
                    if (seuilChar && distChar) {
                        peripheral = dev;
                        connected = true;
                        alreadyPrintedConnectMsg = false;
                        return true;
                    }
                }
                dev.disconnect();
            }
            break;
        }
    }
    stopScan();
    scan();
    return false;
}

bool BLEManager::isConnected() const {
    return connected;
}

void BLEManager::disconnect() {
    if (connected) {
        peripheral.disconnect();
        connected = false;
    }
}

BLECharacteristic& BLEManager::getSeuilChar() {
    return seuilChar;
}

BLECharacteristic& BLEManager::getDistChar() {
    return distChar;
}

BLEDevice& BLEManager::getPeripheral() {
    return peripheral;
}

void BLEManager::resetConnectionFlag() {
    connected = false;
}