// EXTRAIT : acquisition et filtrage du pH (ATMega328P)
// 10 mesures -> tri -> on ignore les 2 plus faibles et les 2 plus fortes
// -> moyenne -> filtre exponentiel. La valeur reste stable malgré le bruit.

float VPHC() {
  for (int i = 0; i < NUM_SAMPLES; i++) {
    samples[i] = analogRead(analogInPin);
    delay(10);
  }

  // tri croissant des échantillons (tri par sélection)
  // ...

  long sum = 0;
  for (int i = DISCARD_SAMPLES; i < NUM_SAMPLES - DISCARD_SAMPLES; i++)
    sum += samples[i];
  float averageReading = (float)sum / (NUM_SAMPLES - 2 * DISCARD_SAMPLES);

  measuredVoltage = averageReading * 5.0 / 1023.0;      // conversion en tension (0-5 V)

  // filtre passe-bas exponentiel
  vphmoy1 = FILTER_ALPHA * measuredVoltage + (1.0 - FILTER_ALPHA) * vphmoy1;
  return vphmoy1;
}
