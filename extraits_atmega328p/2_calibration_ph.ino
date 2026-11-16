// EXTRAIT : calibration automatique du pH en 3 points (tampons pH 4, 7 et 10)
// La pente (a) et l'ordonnée (b) sont moyennées puis sauvegardées en EEPROM :
// la calibration est conservée après une coupure d'alimentation.

void calibration_ph() {
  // v1, v2, v3 : tensions mesurées dans les tampons pH 4, 7 et 10
  a = ((3) / (v2 - v1) + (3) / (v3 - v2) + (6) / (v3 - v1)) / 3;
  b = ((4 - a * v1) + (7 - a * v2) + (10 - a * v3)) / 3;
  saveepprom();
}

float PH() {
  return a * VPHC() + b;          // pH = a·V + b
}

void saveepprom() {
  EEPROM.put(0, a);
  EEPROM.put(10, b);
}
