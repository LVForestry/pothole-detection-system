#include <Arduino.h>
#include "BLEManager.h"
#include "AlarmeManager.h"
#include <SparkFun_u-blox_GNSS_v3.h>  // Bibliothèque GNSS
#include <SPI.h>  // Pour SPI
#include <SD.h>  // Pour SD

SFE_UBLOX_GNSS gnss;  // Instance GNSS déclarée avant
BLEManager bleManager;
AlarmeManager alarmeManager(&gnss);  // Passe l'instance GNSS

void setup() {
    Serial.begin(115200);
    alarmeManager.begin();

    // Inhibition des alarmes pendant 3 secondes pour éviter les fausses triggers à l'init
    delay(3000);
    alarmeManager.enableAlarms();

    // Initialisation GNSS sur Wire1 avec tempo pour fix
    Wire.begin();
    if (gnss.begin()) {  // Adresse par défaut 0x42
        Serial.println("GNSS connecté.");
        // Attendre jusqu'à 10 secondes pour un fix GPS
        unsigned long start = millis();
        bool fixObtenu = false;
        while (millis() - start < 10000) {  // 10 secondes max
            gnss.getPVT();
            if (gnss.getFixType() > 0) {
                Serial.println("Fix GPS obtenu.");
                fixObtenu = true;
                break;
            }
            delay(100);  // Petite pause entre checks
        }
        if (!fixObtenu) {
            Serial.println("Pas de fix GPS après 10s (normal en intérieur).");
        }
    } else {
        Serial.println("Erreur GNSS.");
    }

    // Initialisation SD
    if (!SD.begin(10)) {  // Pin CS pour SD
        Serial.println("Erreur SD.");
    } else {
        Serial.println("SD OK.");
    }

    if (!BLE.begin()) {
        Serial.println("BLE init failed!");
        while (1);
    }
    Serial.println("Central pret. Tapez le nom du capteur (ex: L1) dans le moniteur serie pour vous connecter.");
    BLE.scan();
}

void loop() {
    if (!bleManager.isConnected()) {
        bleManager.alreadyPrintedConnectMsg = false;
        alarmeManager.surveille();

        if (Serial.available()) {
            String cmd = Serial.readStringUntil('\n');
            cmd.trim();
            if (cmd.length() > 0) {
                if (bleManager.connectToDeviceByName(cmd.c_str())) {
                    Serial.println("Connecte! Tapez READ ou CALxxx");
                }
                else {
                    Serial.println("Peripherique non trouve.");
                }
            }
        }
        BLEDevice discovered = BLE.available();
        if (discovered) {
            Serial.print("Trouve: ");
            Serial.print(discovered.address());
            Serial.print(" - ");
            Serial.println(discovered.localName());
        }
    } else {
        // Si connecté à L1, écoute les commandes utilisateur
        if (Serial.available()) {
            String cmd = Serial.readStringUntil('\n');
            cmd.trim();
            if (cmd.equalsIgnoreCase("READ")) {
                BLECharacteristic& distChar = bleManager.getDistChar();
                if (distChar.canWrite() || (distChar.properties() & BLEWrite)) {
                    distChar.writeValue("R");
                    delay(150);
                }
                char buffer[32] = {0};
                int len = distChar.readValue((uint8_t*)buffer, sizeof(buffer) - 1);
                if (len > 0) {
                    buffer[len] = '\0';
                    Serial.print("Distance recue: ");
                    Serial.println(buffer);
                }
            } else if (cmd.startsWith("CAL")) {
                String val = cmd.substring(3);
                val.trim();
                int seuil = val.toInt();
                if (seuil > 0 && seuil < 10000) {
                    String payload = "SET:" + String(seuil);
                    BLECharacteristic& seuilChar = bleManager.getSeuilChar();
                    if (seuilChar.canWrite()) {
                        seuilChar.writeValue(payload.c_str());
                        delay(150);
                    }
                    BLECharacteristic& distChar = bleManager.getDistChar();
                    char buffer[32] = {0};
                    int len = distChar.readValue((uint8_t*)buffer, sizeof(buffer) - 1);
                    if (len > 0) {
                        buffer[len] = '\0';
                        Serial.print("Reponse seuil: ");
                        Serial.println(buffer);
                        Serial.println("Deconnexion du capteur (modification du seuil).");
                        bleManager.disconnect();
                        bleManager.resetConnectionFlag();
                        delay(2000);
                        BLE.scan();
                        return;
                    }
                } else {
                    Serial.println("Valeur de seuil non valide");
                }
            } else {
                Serial.println("Commande inconnue.");
            }
        }

        // Affiche le message "Connecte" une seule fois après la connexion
        if (!bleManager.alreadyPrintedConnectMsg) {
            BLECharacteristic& distChar = bleManager.getDistChar();
            char buffer[32] = {0};
            int len = distChar.readValue((uint8_t*)buffer, sizeof(buffer) - 1);
            if (len > 0) {
                buffer[len] = '\0';
                if (String(buffer).startsWith("Connecte")) {
                    Serial.print("Message du capteur: ");
                    Serial.println(buffer);
                    bleManager.alreadyPrintedConnectMsg = true;
                }
            }
        }
        delay(100);
    }
}