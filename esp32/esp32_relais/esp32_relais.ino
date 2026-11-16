// ESP32 n°2 : commande des relais (ex. résistance chauffante)
// Reçoit la température de l'ESP32 n°1 par le réseau local (HTTP)
// et active le relais avec une petite hystérésis.

#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "NOM_DU_WIFI";
const char* WIFI_PASS = "MOT_DE_PASSE";

const int   PIN_RELAIS_CHAUFFAGE = 26;
const float T_MIN = 24.0;          // seuil d'exemple (°C) : à adapter
const float HYSTERESIS = 1.0;

WebServer server(80);
bool chauffage = false;

void handleUpdate() {
  if (!server.hasArg("temperature")) {
    server.send(400, "text/plain", "temperature manquante");
    return;
  }
  float t = server.arg("temperature").toFloat();

  if (t < T_MIN) chauffage = true;
  else if (t > T_MIN + HYSTERESIS) chauffage = false;

  digitalWrite(PIN_RELAIS_CHAUFFAGE, chauffage ? HIGH : LOW);  // inverser si relais actif à LOW
  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(PIN_RELAIS_CHAUFFAGE, OUTPUT);
  digitalWrite(PIN_RELAIS_CHAUFFAGE, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/update", handleUpdate);
  server.begin();
}

void loop() {
  server.handleClient();
}
