#include "usb_mac.h"
#include "globals.h"

#include <Arduino_USBHostMbed5.h>
#include <DigitalOut.h>
#include <FATFileSystem.h>
#include <errno.h>
#include <string.h>

USBHostMSD msd;                      // objet MSD fourni par Arduino_USBHostMbed5
mbed::FATFileSystem usbfs("usb");    // mount point "/usb"

// Si tu utilises Portenta Machine Control uncomment la ligne suivante (exemple)
// mbed::DigitalOut otg(PB_14, 0);

static bool _mounted = false;

void usb_init() {
  if (_mounted) return;

#if ENABLE_DEBUG
  Serial.println("usb_init(): starting USB host / MSD...");
#endif

  // Activer l'alimentation du port USB-A si nécessaire (Portenta Giga / Carrier)
  // NOTE: adapte la pin PA_15 à ton carrier. Si tu n'as pas cette pin, commente les deux lignes.
#if defined(PA_15)
  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, HIGH); // enable USB-A port power
#if ENABLE_DEBUG
  Serial.println("usb_init(): PA_15 set HIGH to enable USB-A (if applicable)");
#endif
#endif

  // Attendre connexion du MSD via msd.connect()
  unsigned long start = millis();
  const unsigned long totalTimeout = 8000; // timeout global d'attente de connexion
  bool connected = false;
  while (millis() - start < totalTimeout) {
    if (msd.connect()) {
#if ENABLE_DEBUG
      Serial.println("usb_init(): msd.connect() success");
#endif
      connected = true;
      break;
    }
    delay(250);
  }

  if (!connected) {
#if ENABLE_DEBUG
    Serial.println("usb_init(): msd.connect() failed (no device)");
#endif
    _mounted = false;
    return;
  }

  // Tentative de montage
  int err = usbfs.mount(&msd);
  if (err) {
#if ENABLE_DEBUG
    Serial.print("usb_init(): Error mounting USB device: ");
    Serial.println(err);
#endif
    _mounted = false;
    return;
  }

  _mounted = true;
#if ENABLE_DEBUG
  Serial.println("usb_init(): FAT mounted at /usb");
#endif
}

bool usb_isMounted() {
  if (!_mounted) usb_init();
  return _mounted;
}

static String _readFilePath(const char* filename) {
  char path[128];
  snprintf(path, sizeof(path), "/usb/%s", filename);
  return String(path);
}

String usb_readFile(const char* filename) {
  if (!usb_isMounted()) return String("");

  String path = _readFilePath(filename);
  FILE *f = fopen(path.c_str(), "r");
  if (!f) {
#if ENABLE_DEBUG
    Serial.print("usb_readFile(): fopen failed for ");
    Serial.println(path);
#endif
    return String("");
  }

  String out;
  char buf[256];
  while (fgets(buf, sizeof(buf), f) != NULL) {
    out += String(buf);
  }
  int cerr = fclose(f);
  if (cerr < 0) {
#if ENABLE_DEBUG
    Serial.print("usb_readFile(): fclose error ");
    Serial.println(strerror(errno));
#endif
  }
  out.trim();
  return out;
}

String usb_readFirstLine(const char* filename) {
  if (!usb_isMounted()) return String("");

  String path = _readFilePath(filename);
  FILE *f = fopen(path.c_str(), "r");
  if (!f) {
#if ENABLE_DEBUG
    Serial.print("usb_readFirstLine(): fopen failed for ");
    Serial.println(path);
#endif
    return String("");
  }

  char line[256] = {0};
  if (fgets(line, sizeof(line)-1, f) == NULL) {
    fclose(f);
#if ENABLE_DEBUG
    Serial.print("usb_readFirstLine(): empty file ");
    Serial.println(path);
#endif
    return String("");
  }
  int cerr = fclose(f);
  if (cerr < 0) {
#if ENABLE_DEBUG
    Serial.print("usb_readFirstLine(): fclose error ");
    Serial.println(strerror(errno));
#endif
  }
  String s(line);
  s.trim();
  return s;
}

bool usb_writeFile(const char* filename, const String &content) {
  if (!usb_isMounted()) return false;

  String path = _readFilePath(filename);
  FILE *f = fopen(path.c_str(), "w");
  if (!f) {
#if ENABLE_DEBUG
    Serial.print("usb_writeFile(): fopen(w) failed for ");
    Serial.println(path);
#endif
    return false;
  }

  size_t wrote = fwrite(content.c_str(), 1, content.length(), f);
  int cerr = fclose(f);
  if (cerr < 0) {
#if ENABLE_DEBUG
    Serial.print("usb_writeFile(): fclose error ");
    Serial.println(strerror(errno));
#endif
  }
  return (wrote == (size_t)content.length());
}