/*
  Modifier le fichier arduino_secrets.h pour ajouter les informations de connexion à votre réseau wifi
*/

#include <WiFi.h>
#include "esp_wpa2.h"        // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion
#include <ESP32Servo.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


WiFiClient netSocket;
bool btn;
bool flag = 0;
// simply print API URL: https://api.simplyprint.io/
// my API KEY : 6beb928d-2dbc-4cff-80ec-e85a26a72377
ESP32PWM pwm;
void setup()
{
  if (!Serial)
    delay(3000);
  ESP32PWM::allocateTimer(0);
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
  // config LED en sortie
  pinMode(13, OUTPUT); // LED
  pinMode(27, INPUT);
  pwm.attachPin(12, 50, 10); //PIN, Freq, 10 bits resolution

  //
  
  // Afficher les informations de connexion
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID : ");
  Serial.println(WiFi.SSID());
}

const String discord_webhook = SECRET_WEBHOOK;
const String discord_tts = SECRET_TTS;
//openssl s_client -showcerts -connect discordapp.com:443 (get last certificate)
const char* discordappCertificate = \
                                    "-----BEGIN CERTIFICATE-----\n"
                                    "MIIDrDCCA1KgAwIBAgIQI7XTQ9dw6e4Rt7Am9U6u/DAKBggqhkjOPQQDAjA7MQsw\n"
                                    "CQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMQwwCgYD\n"
                                    "VQQDEwNXRTEwHhcNMjQwOTE5MDMzNTE5WhcNMjQxMjE4MDMzNTE4WjAZMRcwFQYD\n"
                                    "VQQDEw5kaXNjb3JkYXBwLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABHBk\n"
                                    "gfnSs5c7xWBslQrPURj2HJWASNSi68BYs8EEsePKQ/PITPE+EQS4w6+sDQZASSN9\n"
                                    "RZq2zFM0kK2TrDVOCqmjggJYMIICVDAOBgNVHQ8BAf8EBAMCB4AwEwYDVR0lBAww\n"
                                    "CgYIKwYBBQUHAwEwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUuhBvie/2+rVJG2ft\n"
                                    "AlrxV+xw0bEwHwYDVR0jBBgwFoAUkHeSNWfE/6jMqeZ72YB5e8yT+TgwXgYIKwYB\n"
                                    "BQUHAQEEUjBQMCcGCCsGAQUFBzABhhtodHRwOi8vby5wa2kuZ29vZy9zL3dlMS9J\n"
                                    "N1UwJQYIKwYBBQUHMAKGGWh0dHA6Ly9pLnBraS5nb29nL3dlMS5jcnQwKwYDVR0R\n"
                                    "BCQwIoIOZGlzY29yZGFwcC5jb22CECouZGlzY29yZGFwcC5jb20wEwYDVR0gBAww\n"
                                    "CjAIBgZngQwBAgEwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2MucGtpLmdvb2cv\n"
                                    "d2UxL3Q2MkV0NFdFNVFZLmNybDCCAQMGCisGAQQB1nkCBAIEgfQEgfEA7wB2AHb/\n"
                                    "iD8KtvuVUcJhzPWHujS0pM27KdxoQgqf5mdMWjp0AAABkgiQMYMAAAQDAEcwRQIg\n"
                                    "MkOv7ZZuNlXHCA/y+IICny741JdghaWn6aXo1fs+xhgCIQDB0vdMwciEOLvSBF2Q\n"
                                    "GuwKXMGnM01DN/+Lf6D0cZIK3AB1AD8XS0/XIkdYlB1lHIS+DRLtkDd/H4Vq68G/\n"
                                    "KIXs+GRuAAABkgiQMWQAAAQDAEYwRAIgBq7SoQxfguTVwUCQboRGqVF401VwTN7x\n"
                                    "dXj+bJAxN4wCIEBUoJLq/fZB/yl1dkWq50AZePstlWZ93L1Q/3gbjql9MAoGCCqG\n"
                                    "SM49BAMCA0gAMEUCIQDUR1jLiaP1fyCxL0ma/AYP2vsiOmN4tlw3P9sHMV5M1gIg\n"
                                    "TdSDVN1MDuXjS2q0J2bu+LRB59b7zabDGs7K7/nORWo=\n"
                                    "-----END CERTIFICATE-----\n";

void sendDiscord(String content) {
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client -> setCACert(discordappCertificate);
    {
      HTTPClient https;
      Serial.println("[HTTP] Connecting to Discord...");
      Serial.println("[HTTP] Message: " + content);
      Serial.println("[HTTP] TTS: " + discord_tts);
      if (https.begin(*client, discord_webhook)) {  // HTTPS
        // start connection and send HTTP header
        https.addHeader("Content-Type", "application/json");
        int httpCode = https.POST("{\"content\":\"" + content + "\",\"tts\":" + discord_tts +"}");

        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.print("[HTTP] Status code: ");
          Serial.println(httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.print("[HTTP] Response: ");
            Serial.println(payload);
          }
        } else {
          Serial.print("[HTTP] Post... failed, error: ");
          Serial.println(https.errorToString(httpCode).c_str());
        }

        https.end();
      } else {
        Serial.printf("[HTTP] Unable to connect\n");
      }

      // End extra scoping block
    }

    delete client;
  } else {
    Serial.println("[HTTP] Unable to create client");
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    sendDiscord("Hello World!");
    
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

          int id = printerObj["id"];
          //if (id == PRINTER_ID)
          //{

            const char *printerName = printerObj["printer"]["name"];
            const char *printerState = printerObj["printer"]["state"];

            Serial.print("Nom de l'imprimante : ");
            Serial.print(printerName);
            Serial.print(" [");
            Serial.print(printerState);
            Serial.println("]");
            Serial.print("{");
            Serial.print(id);
            Serial.println("}");
               
            if (strcmp(printerState, "printing") == 0 && id == 21937)//si l'imprimante 402 est en train d'imprimer
            {
            
              Serial.println("Allume");
              if (flag == 0){
              btn = digitalRead(27);
              }
              Serial.println(btn);
              digitalWrite(13, btn); // Turn LED ON

                if (!btn)
                {
                  pwm.writeScaled(0.05);//1ms verrouiller

                } 
                else 
                {
                  pwm.writeScaled(0.1);//2ms deverouiller
                  btn = 1;
                  flag = 1;
                }
            }
            else
            {
              Serial.println("Éteind");
             // digitalWrite(13, LOW); // Turn LED OFF
            }

    

            
          //}
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

  }
}