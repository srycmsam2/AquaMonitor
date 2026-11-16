// EXTRAIT : conversion tension -> turbidité (NTU)
// Courbe d'étalonnage obtenue par régression sous Matlab (double exponentielle).
// at, bt, ct, dt : coefficients de la régression.

float readwTurbidity() {
  ecm = 0;
  for (int i = 0; i < 10; i++) ECB[i] = analogRead(sensorPin);
  for (int i = 0; i < 10; i++) ecm += (float)ECB[i];
  ecm = (ecm / 10) * 5.00 / 1023;          // tension moyenne (V)

  m = ecm * 1000;                          // tension en mV
  double trb = (at * exp(bt * m) + ct * exp(dt * m)) - 6.37;
  return trb;
}
