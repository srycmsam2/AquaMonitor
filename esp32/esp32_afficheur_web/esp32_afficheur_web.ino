// ESP32 n°1 : réception I²C + interface web + envoi à l'ESP32 n°2
// Reconstitué d'après l'organigramme du rapport (Fig. III.29) :
// init -> connexion Wi-Fi -> adresse IP -> interface web
//      -> réception I²C -> affichage -> envoi à l'autre ESP (réseau local)

#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "NOM_DU_WIFI";
const char* WIFI_PASS = "MOT_DE_PASSE";
const char* ESP_RELAIS_URL = "http://192.168.1.50/update";   // IP de l'ESP32 n°2

#define I2C_SLAVE_ADDRESS 0x08

struct SensorData {            // même structure que côté ATMega328P
  float temperature, ph, ec, turbidity, oxygen;
};

SensorData data;
uint8_t rxBuf[sizeof(SensorData)];
volatile bool newData = false;
WebServer server(80);

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="fr"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>AquaMonitor</title></head><body style="font-family:sans-serif;text-align:center">
<h2>AquaMonitor</h2>
<p>Température : <b id="t">--</b> °C</p>
<p>pH : <b id="ph">--</b></p>
<p>Conductivité : <b id="ec">--</b> µS/cm</p>
<p>Turbidité : <b id="tr">--</b> NTU</p>
<p>Oxygène dissous : <b id="o2">--</b> %</p>
<script>
setInterval(async () => {
  const d = await (await fetch('/data')).json();
  t.textContent = d.temperature; ph.textContent = d.ph; ec.textContent = d.ec;
  tr.textContent = d.turbidity; o2.textContent = d.oxygen;
}, 2000);
</script></body></html>
)rawliteral";

// Appelée à chaque trame reçue de l'ATMega328P
void onReceive(int len) {
  if (len != (int)sizeof(SensorData)) {            // trame incomplète : on la jette
    while (Wire.available()) Wire.read();
    return;
  }
  Wire.readBytes(rxBuf, sizeof(rxBuf));
  newData = true;
}

void handleRoot() { server.send_P(200, "text/html", PAGE); }

void handleData() {
  String json = "{\"temperature\":" + String(data.temperature, 1) +
                ",\"ph\":"          + String(data.ph, 2) +
                ",\"ec\":"          + String(data.ec, 0) +
                ",\"turbidity\":"   + String(data.turbidity, 1) +
                ",\"oxygen\":"      + String(data.oxygen, 0) + "}";
  server.send(200, "application/json", json);
}

void sendToRelayESP() {
  HTTPClient http;
  http.setTimeout(1000);
  http.begin(String(ESP_RELAIS_URL) + "?temperature=" + String(data.temperature, 1));
  http.GET();
  http.end();
}

void setup() {
  Serial.begin(115200);

  Wire.onReceive(onReceive);
  Wire.begin((uint8_t)I2C_SLAVE_ADDRESS);          // l'ESP32 est esclave I²C

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500); // « connecté au Wi-Fi ? »
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());                   // « envoyer l'adresse IP »

  server.on("/", handleRoot);                       // interface web
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
  if (newData) {                                    // « données récupérées ? »
    newData = false;
    memcpy(&data, rxBuf, sizeof(data));
    sendToRelayESP();                               // envoi à l'ESP32 n°2
  }
}
