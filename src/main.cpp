/*
  Modifier le fichier arduino_secrets.h pour ajouter les informations de connexion à votre réseau wifi
*/

#include <WiFi.h>
#include "esp_wpa2.h" // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion

WiFiClient netSocket;

void setup() {
  if (!Serial) delay(3000);
  Serial.begin(9600);
  // Connexion au réseau wifi sécurisé avec identifiant et mot de passe
  WiFi.begin(SECRET_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

  // Attendre la connexion au réseau wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);  // print the network name (SSID)
    delay(2000);
  }

  // Afficher les informations de connexion
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID : ");
  Serial.println(WiFi.SSID());
}
void loop() {
  // Afficher la force du signal wifi toutes les 2 secondes
  Serial.print("Signal Wifi: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  delay(2000);
}