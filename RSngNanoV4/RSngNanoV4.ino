#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Wire.h>
#include <TFLI2C.h>
#include <NanoBLEFlashPrefs.h>

#define DEVICE_NAME      "L1"
#define TFLUNA_I2C_ADDR  0x3E
#define PIN_OUT          2
#define SEUIL_INIT       1000

typedef struct {
  int seuil;
} flashPrefsStruct;

NanoBLEFlashPrefs myFlashPrefs;
flashPrefsStruct globalPrefs;

unsigned long pulseStart = 0;
bool pulseActive = false;
unsigned long lastFreqPrint = 0;
unsigned long mesureCount = 0;

TFLI2C tflI2C;
BLEService roadScannerService("180A");
BLEStringCharacteristic seuilCharacteristic("2A57", BLERead | BLEWrite, 8);
BLEStringCharacteristic distanceCharacteristic("2A58", BLERead | BLEWrite, 8);

void setup() {
  pinMode(PIN_OUT, OUTPUT);
  digitalWrite(PIN_OUT, LOW);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  Serial.begin(115200);
  Wire.begin();

  // === INITIALISATION FLASH PREFS ===
  flashPrefsStruct localPrefs;
  int rc = myFlashPrefs.readPrefs(&localPrefs, sizeof(localPrefs));
  if (rc == FDS_SUCCESS) {
    globalPrefs = localPrefs;
    Serial.print("Seuil lu en flash : ");
    Serial.println(globalPrefs.seuil);
  } else {
    globalPrefs.seuil = SEUIL_INIT;
    myFlashPrefs.writePrefs(&globalPrefs, sizeof(globalPrefs));
    Serial.println("Valeur seuil par défaut stockée en flash");
  }

  // === INITIALISATION BLE ===
  if (!BLE.begin()) {
    Serial.println("Erreur initialisation BLE!");
    while (1);
  }
  BLE.setLocalName(DEVICE_NAME);
  BLE.setAdvertisedService(roadScannerService);

  roadScannerService.addCharacteristic(seuilCharacteristic);
  roadScannerService.addCharacteristic(distanceCharacteristic);
  BLE.addService(roadScannerService);

  seuilCharacteristic.writeValue(String(globalPrefs.seuil));
  distanceCharacteristic.writeValue("0");

  BLE.advertise();
  Serial.println("BLE prêt & advertising");

  lastFreqPrint = millis();
  mesureCount = 0;
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    digitalWrite(LEDG, LOW);
    Serial.print("Connexion centrale: ");
    Serial.println(central.address());
    distanceCharacteristic.writeValue("Connecte, seuil=" + String(globalPrefs.seuil));

    bool stayConnected = true;
    while (central.connected() && stayConnected) {
      if (seuilCharacteristic.written()) {
        String value = seuilCharacteristic.value();
        if (value.startsWith("SET:")) {
          int newSeuil = value.substring(4).toInt();
          globalPrefs.seuil = newSeuil;
          myFlashPrefs.writePrefs(&globalPrefs, sizeof(globalPrefs)); // Sauvegarde persistante
          seuilCharacteristic.writeValue(String(globalPrefs.seuil));
          distanceCharacteristic.writeValue("OK:" + String(globalPrefs.seuil));
          Serial.print("Nouveau seuil reçu et stocké : ");
          Serial.println(globalPrefs.seuil);
          delay(300);
          stayConnected = false;
          break;
        }
      }
      if (distanceCharacteristic.written()) {
        int distance = getDistance();
        distanceCharacteristic.writeValue(String(distance));
      }
      delay(30);
    }
    BLE.advertise();
    digitalWrite(LEDG, HIGH);
    Serial.println("Central déconnecté, reprise lecture distance");
  } else {
    int distance = getDistance();
    mesureCount++;
    if (distance < 3) {
      digitalWrite(LEDR, LOW);
    } else {
      digitalWrite(LEDR, HIGH);
    }

    if (millis() - lastFreqPrint >= 5000) {
      float freq = mesureCount / 5.0;
      Serial.print(distance);
      Serial.print(" ");
      Serial.print(freq);
      Serial.println(" Hz");
      mesureCount = 0;
      lastFreqPrint = millis();
    }

    if (distance > globalPrefs.seuil + 5) {
      if (!pulseActive) {
        digitalWrite(PIN_OUT, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("Alarme ");
        Serial.print(distance);
        Serial.println("cm");
        pulseStart = millis();
        pulseActive = true;
      }
      if (pulseActive && (millis() - pulseStart > 2000)) {
        digitalWrite(PIN_OUT, LOW);
        pulseActive = false;
      }
    } else {
      digitalWrite(PIN_OUT, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      pulseActive = false;
    }
    delay(5);
  }
}

int getDistance() {
  int16_t dist = 0;
  if (tflI2C.getData(dist, TFLUNA_I2C_ADDR)) {
    return dist;
  }
  return 0;
}