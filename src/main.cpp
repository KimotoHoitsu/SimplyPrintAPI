/*
  Modifier le fichier arduino_secrets.h pour ajouter les informations de connexion à votre réseau wifi
*/

#include <WiFi.h>
#include "esp_wpa2.h"        // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion

#include <HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient netSocket;

void setup()
{
  if (!Serial)
    delay(3000);
  Serial.begin(9600);
  // Connexion au réseau wifi sécurisé avec identifiant et mot de passe
  WiFi.begin(SECRET_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

  // Attendre la connexion au réseau wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID); // print the network name (SSID)
    delay(2000);
  }

  // Afficher les informations de connexion
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID : ");
  Serial.println(WiFi.SSID());
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    // Spécification de l'URL
    http.begin("https://api.simplyprint.io/12305/printers/Get");

    // Ajout des headers
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);

    // Envoyer la requête POST
    int httpResponseCode = http.POST("{}");

    // Vérifier la réponse
    if (httpResponseCode > 0)
    {
      String response = http.getString();
      // Serial.println("Réponse du serveur: " + response);

      // Parse de la réponse JSON
      const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc, response);

      // Vérifier le statut
      if (doc["status"] == true)
      {
        JsonArray data = doc["data"];

        // Parcourir les objets dans "data"
        for (JsonObject printerObj : data)
        {
          const char *printerName = printerObj["printer"]["name"];
          const char *printerState = printerObj["printer"]["state"];
          Serial.print("Nom de l'imprimante : ");
          Serial.print(printerName);
          Serial.print(" [");
          Serial.print(printerState);
          Serial.println("]");
        }
      }
      else
      {
        Serial.println("Erreur lors de la requête POST: " + String(httpResponseCode));
      }

      // Terminer la connexion
      http.end();
    }
    else
    {
      Serial.println("Non connecté au WiFi");
    }

    while (1);
  }
}