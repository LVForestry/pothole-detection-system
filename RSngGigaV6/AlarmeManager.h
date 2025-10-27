#pragma once
#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_v3.h>  // Pour GNSS

#define ALARM_COUNT 16

struct AlarmePin {
    uint8_t pin;
    const char* name;
    bool wasLow;
};

class AlarmeManager {
public:
    AlarmeManager(SFE_UBLOX_GNSS* gnssPtr);  // Constructeur avec pointeur GNSS
    void begin();
    void surveille();
    void enableAlarms();  // Nouvelle méthode pour activer les alarmes
private:
    AlarmePin alarmes[ALARM_COUNT];
    SFE_UBLOX_GNSS* gnss;  // Pointeur vers GNSS
    bool inhibitAlarms = true;  // Inhibition par défaut
};