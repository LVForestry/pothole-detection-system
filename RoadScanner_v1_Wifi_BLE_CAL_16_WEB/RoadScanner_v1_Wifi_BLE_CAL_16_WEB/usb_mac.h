#ifndef USB_MAC_H
#define USB_MAC_H

#include <Arduino.h>

// Initialise le sous-système USB / FAT (appelé depuis setup)
void usb_init();

// Indique si la clé USB / filesystem est montée
bool usb_isMounted();

// Lit le contenu entier d'un fichier sur la clé USB ("/usb/<filename>")
// Retourne "" si erreur / fichier absent
String usb_readFile(const char* filename);

// Lit la première ligne d'un fichier (utile pour *.mac ou liste.txt lines)
String usb_readFirstLine(const char* filename);

// Ecrit (remplace) le contenu d'un fichier sur la clé USB ("/usb/<filename>")
// Retourne true si OK
bool usb_writeFile(const char* filename, const String &content);

#endif // USB_MAC_H