// EXTRAIT : envoi des mesures en I²C vers l'ESP32 (esclave, adresse 0x08)
// Les 5 paramètres sont regroupés dans une structure (20 octets),
// envoyée toutes les 2 secondes depuis loop().

struct SensorData {
  float temperature;
  float ph;
  float ec;
  float turbidity;
  float oxygen;
};

void sendI2CData() {
  SensorData data;
  data.temperature = readwTemperature();
  data.ph          = PH();
  data.ec          = TDS();
  data.turbidity   = readwTurbidity();
  data.oxygen      = O2();

  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write((byte*)&data, sizeof(data));
  Wire.endTransmission();
}
