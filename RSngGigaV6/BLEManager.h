#pragma once
#include <ArduinoBLE.h>

class BLEManager {
public:
    BLEManager();
    void begin();
    void scan();
    void stopScan();
    BLEDevice available();
    bool connectToDeviceByName(const char* targetName);
    bool isConnected() const;
    void disconnect();
    BLECharacteristic& getSeuilChar();
    BLECharacteristic& getDistChar();
    BLEDevice& getPeripheral();
    void resetConnectionFlag();
    bool alreadyPrintedConnectMsg = false;
private:
    BLEDevice peripheral;
    BLECharacteristic seuilChar;
    BLECharacteristic distChar;
    bool connected = false;
};