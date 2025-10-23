#include <WiFi.h>
#include "sensor_manager.h"
#include "config.h"

// Déclare un serveur HTTP sur le port 80
static WiFiServer _server(80);
static bool _apActive = false;

// Page HTML statique avec JavaScript intégré
static const char INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
<head><meta charset="utf-8"><title>RoadScanner</title></head>
<body>
  <h2>RoadScanner (AP + BLE)</h2>
  <div id="sensors">
    <p>Chargement des capteurs...</p>
  </div>

  <script>
    // Fonction pour calibrer un capteur
    function calibrateSensor(sensorName) {
      console.log('Calibration du capteur :', sensorName);
      fetch(`/calibrate?sensorId=${encodeURIComponent(sensorName)}`)
        .then(response => {
          if (!response.ok) {
            throw new Error('Erreur avec la requête de calibration');
          }
          return response.json();
        })
        .then(data => {
          console.log('Réponse de calibration :', data);
          if (data.ok) {
            alert(`Capteur ${sensorName} calibré avec succès !`);
            refreshSensors();
          } else {
            alert(`Erreur lors de la calibration du capteur ${sensorName}: ${data.error || 'Erreur inconnue'}`);
          }
        })
        .catch(error => {
          console.error('Erreur lors de la calibration :', error);
          alert(`Erreur lors de la calibration du capteur ${sensorName}`);
        });
    }

    // Fonction pour actualiser la liste des capteurs
    function refreshSensors() {
      console.log('Envoi de requête pour /sensors');
      fetch('/sensors')
        .then(response => {
          if (!response.ok) {
            throw new Error('Erreur avec la requête réseau');
          }
          return response.json();
        })
        .then(data => {
          console.log('Capteurs reçus :', data);
          const container = document.getElementById('sensors');
          container.innerHTML = ''; // Vider le contenu existant
          if (data.length === 0) {
            container.innerHTML = '<p>Aucun capteur trouvé</p>';
            return;
          }
          data.forEach(sensor => {
            const div = document.createElement('div');
            div.style.marginBottom = '10px';
            if (sensor.status === "initialized") {
              div.innerHTML = `<strong>${sensor.name}</strong> - Seuil : ${sensor.threshold} <span style="color:green;">(Initialisé)</span>`;
            } else {
              div.innerHTML = `<strong>${sensor.name}</strong> - <span style="color:red;">Non initialisé</span>`;
              if (sensor.error) {
                div.innerHTML += `<br><em style="color:gray;">Erreur : ${sensor.error}</em>`;
              }
            }
            // Ajouter le bouton "Calibrer"
            const calibrateBtn = document.createElement('button');
            calibrateBtn.textContent = 'Calibrer';
            calibrateBtn.style.marginLeft = '10px';
            calibrateBtn.onclick = () => calibrateSensor(sensor.name);
            div.appendChild(calibrateBtn);
            container.appendChild(div);
          });
        })
        .catch(error => {
          console.error('Erreur lors de la récupération des capteurs :', error);
          document.getElementById('sensors').innerHTML = '<p>Erreur lors du chargement des capteurs</p>';
        });
    }

    // Appeler la fonction au chargement de la page
    window.addEventListener('load', refreshSensors);
  </script>
</body>
</html>
)rawliteral";

// Fonction pour initialiser le point d'accès WiFi
void wifi_ap_init() {
  Serial.println("Initialisation du point d'accès WiFi...");
  const char* ssid = WIFI_AP_SSID;
  const char* password = WIFI_AP_PASSWORD;

  // Démarrer le point d'accès
  if (WiFi.beginAP(ssid, password)) { // Utilise le retour booléen de WiFi.beginAP()
    _apActive = true;
    Serial.println("Point d'accès WiFi activé");
    Serial.print("SSID : ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Échec de l'activation du point d'accès WiFi.");
    _apActive = false;
  }

  // Démarrer le serveur HTTP
  _server.begin();
}

// Fonction pour gérer les requêtes des clients
void wifi_ap_handleClients() {
  if (!_apActive) return;

  WiFiClient client = _server.available();
  if (!client) return;

  // Lire la première ligne de la requête HTTP
  String reqLine = client.readStringUntil('\r');
  client.flush(); // Nettoyer le buffer

  Serial.print("Requête reçue : ");
  Serial.println(reqLine);

  // Traiter la requête
  if (reqLine.startsWith("GET /calibrate")) {
    Serial.println("Traitement de /calibrate...");
    
    // Extraire le paramètre sensorId de l'URL
    int sensorIdStart = reqLine.indexOf("sensorId=");
    String sensorId = "";
    if (sensorIdStart >= 0) {
      sensorIdStart += 9; // Longueur de "sensorId="
      int sensorIdEnd = reqLine.indexOf(" ", sensorIdStart);
      if (sensorIdEnd < 0) {
        sensorIdEnd = reqLine.indexOf("&", sensorIdStart);
      }
      if (sensorIdEnd < 0) {
        sensorIdEnd = reqLine.length();
      }
      sensorId = reqLine.substring(sensorIdStart, sensorIdEnd);
      // Décoder les caractères encodés en URL (basique)
      sensorId.replace("%20", " ");
    }
    
    String jsonResponse;
    if (sensorId.length() > 0) {
      Serial.print("Calibration du capteur : ");
      Serial.println(sensorId);
      jsonResponse = sensor_calibrate(sensorId);
    } else {
      jsonResponse = "{\"ok\":false,\"error\":\"Paramètre sensorId manquant\"}";
    }
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: ");
    client.print(jsonResponse.length());
    client.print("\r\n\r\n");
    client.print(jsonResponse);
    Serial.println("Réponse de calibration envoyée");
  } else if (reqLine.startsWith("GET /sensors")) {
    Serial.println("Traitement de /sensors...");
    String jsonResponse = sensor_manager_getSummaryJson();
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: ");
    client.print(jsonResponse.length());
    client.print("\r\n\r\n");
    client.print(jsonResponse);
    Serial.println("JSON envoyé pour /sensors");
  } else if (reqLine.startsWith("GET /")) {
    Serial.println("Envoi de la page HTML...");
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html; charset=utf-8\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: ");
    client.print(strlen(INDEX_HTML));
    client.print("\r\n\r\n");
    client.print(INDEX_HTML);
    Serial.println("Page HTML envoyée");
  } else {
    // Gérer les requêtes non reconnues
    client.print("HTTP/1.1 404 Not Found\r\n");
    client.print("Connection: close\r\n\r\n");
    Serial.println("Requête non reconnue");
  }

  // Terminer la connexion avec le client
  client.stop();
}

// Fonction pour vérifier si le point d'accès est actif
bool wifi_ap_isActive() {
  return _apActive;
}