# AquaMonitor — micro-ferme aquaponique instrumentée

Projet de fin d'études, Master Instrumentation, USTHB (2024/2025).
Mesure de 5 paramètres de l'eau (pH, température, conductivité, oxygène dissous, turbidité),
affichage local (OLED) et à distance (Wi-Fi), commande de relais.

## Chaîne de mesure

Capteurs → conditionnement → **ATMega328P** (acquisition, étalonnage)
→ I²C → OLED (affichage local) + **ESP32 n°1** (interface web)
→ Wi-Fi → **ESP32 n°2** (relais, ex. résistance chauffante).

Organigrammes : voir le dossier `docs/`.

## Contenu

| Fichier | Rôle |
|---|---|
| `extraits_atmega328p/1_filtrage_ph.ino` | Acquisition du pH : tri, moyenne des valeurs centrales, filtre exponentiel |
| `extraits_atmega328p/2_calibration_ph.ino` | Calibration 3 points (pH 4, 7, 10) sauvegardée en EEPROM |
| `extraits_atmega328p/3_turbidite.ino` | Conversion tension → NTU (régression exponentielle) |
| `extraits_atmega328p/4_envoi_i2c.ino` | Envoi des 5 mesures à l'ESP32 en I²C |
| `esp32/esp32_afficheur_web/` | ESP32 n°1 : réception I²C, interface web, envoi à l'ESP32 n°2 |
| `esp32/esp32_relais/` | ESP32 n°2 : commande du relais de chauffage |

## Remarques

- Les fichiers de `extraits_atmega328p/` sont des **extraits** du firmware complet, simplifiés pour la lecture (ce ne sont pas des sketchs autonomes).
- Rapport complet du projet : disponible sur demande.
