#include "AlarmeManager.h"
#include <SPI.h>  // Pour SPI
#include <SD.h>   // Bibliothèque SD standard

extern SFE_UBLOX_GNSS gnss;

AlarmeManager::AlarmeManager(SFE_UBLOX_GNSS* gnssPtr) : gnss(gnssPtr) {
    // Initialisation champ par champ (pas de liste d'initialisation possible ici)
    alarmes[0].pin = 53; alarmes[0].name = "L1"; alarmes[0].wasLow = false;
    alarmes[1].pin = 51; alarmes[1].name = "L2"; alarmes[1].wasLow = false;
    alarmes[2].pin = 49; alarmes[2].name = "L3"; alarmes[2].wasLow = false;
    alarmes[3].pin = 47; alarmes[3].name = "L4"; alarmes[3].wasLow = false;
    alarmes[4].pin = 45; alarmes[4].name = "L5"; alarmes[4].wasLow = false;
    alarmes[5].pin = 43; alarmes[5].name = "L6"; alarmes[5].wasLow = false;
    alarmes[6].pin = 41; alarmes[6].name = "L7"; alarmes[6].wasLow = false;
    alarmes[7].pin = 39; alarmes[7].name = "L8"; alarmes[7].wasLow = false;

    alarmes[8].pin = 37; alarmes[8].name = "R8"; alarmes[8].wasLow = false;
    alarmes[9].pin = 35; alarmes[9].name = "R7"; alarmes[9].wasLow = false;
    alarmes[10].pin = 33; alarmes[10].name = "R6"; alarmes[10].wasLow = false;
    alarmes[11].pin = 31; alarmes[11].name = "R5"; alarmes[11].wasLow = false;
    alarmes[12].pin = 29; alarmes[12].name = "R4"; alarmes[12].wasLow = false;
    alarmes[13].pin = 27; alarmes[13].name = "R3"; alarmes[13].wasLow = false;
    alarmes[14].pin = 25; alarmes[14].name = "R2"; alarmes[14].wasLow = false;
    alarmes[15].pin = 23; alarmes[15].name = "R1"; alarmes[15].wasLow = false;
}

void AlarmeManager::begin() {
    for (int i = 0; i < ALARM_COUNT; ++i) {
        pinMode(alarmes[i].pin, INPUT_PULLUP);
        // Force wasLow = true à l'init pour ignorer tout état initial et éviter les fausses alarmes
        alarmes[i].wasLow = true;
    }
    inhibitAlarms = true;  // Inhibition à l'init (optionnel)
}

void AlarmeManager::surveille() {
    if (inhibitAlarms) return;  // Ignore les alarmes si inhibé

    for (int i = 0; i < ALARM_COUNT; ++i) {
        int state = digitalRead(alarmes[i].pin);
        if (state == LOW && !alarmes[i].wasLow) {
            Serial.print("Alarme ");
            Serial.println(alarmes[i].name);
            alarmes[i].wasLow = true;

            // Logging avec nom court DDMMYY.txt (e.g., 271025.txt)
            gnss->getPVT();
            String filename = String(gnss->getDay()) + String(gnss->getMonth()) + String(gnss->getYear()) + ".txt";
            File dataFile = SD.open(filename.c_str(), FILE_WRITE);
            if (dataFile) {
                dataFile.print(String(alarmes[i].name));
                dataFile.print(",");
                dataFile.print(String(gnss->getDay()) + "-" + String(gnss->getMonth()) + "-" + String(gnss->getYear()));
                dataFile.print(",");
                dataFile.print(String(gnss->getHour()) + ":" + String(gnss->getMinute()) + ":" + String(gnss->getSecond()));
                dataFile.print(",");
                dataFile.print(gnss->getLatitude() / 10000000.0, 7);
                dataFile.print(",");
                dataFile.print(gnss->getLongitude() / 10000000.0, 7);
                dataFile.print(",");
                dataFile.print(gnss->getGroundSpeed() / 1000.0);
                dataFile.print(",");
                dataFile.println(gnss->getHeading() / 100000.0);
                dataFile.close();
                Serial.println("Log sauvegardé.");
            } else {
                Serial.println("Erreur log SD.");
            }
        } else if (state == HIGH && alarmes[i].wasLow) {
            alarmes[i].wasLow = false;
        }
    }
}

void AlarmeManager::enableAlarms() {
    inhibitAlarms = false;
}