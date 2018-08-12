#include <ESP8266WiFi.h>                    //Inclusion bibliothèque gestion du WIFI de l'ESP8266
#include <Espalexa.h>                       //Inclusion bibliothèque pour commande avec Alexa Amazone
#include <ESP8266WebServer.h>               //Inclusion bibliothèque gestion du serveur web de l'ESP8266
#include <WS2812FX_fr.h>                    //Inclusion bibliothèque gestion des LED WS2812
#include <EEPROM.h>                         //Inclusion bibliothèque gestion de l'EEPROM
#include <FS.h>                             //Inclusion bibliothèque SPIFFS
#include <WiFiUdp.h>                        //Inclusion bibliothèque pour la gestion de l'User Datagram Protocol
#include <NTPClient.h>                      //Inclusion bibliothèque gestion serveur NTP
#include <TimeLib.h>                        //Inclusion bibliothèque gestion des fonctionnalités de chronométrage
#include <TimeAlarms.h>                     //Inclusion bibliothèque gestion des fonctionnalités d'alarme et minuterie
#include <ESP8266Ping.h>                    //Inclusion bibliothèque pour le ping et modification de byte count = 1 dans la bibliothèque
#include <StringSplitter.h>                 //Inclusion bibliothèque pour création d'un tableau depuis chaine avec séparateur Modifier nbrs MAX 5 => 10
#include "Definition.h"                     //Inclusion des définitions

// Variables globales
String DateHeure = "";                      // Variable pour gestion de la date et de l'heure
String AffMinuteur = "";                    // Variable globale de la valeur du minuteur pour client web
String AffEtat = "";                        // Variable globale pour affichage état pour client web
int MinuteurStop = 0;                       // Variable globale de la valeur du minuteur
uint8_t AlarmeMin = dtINVALID_ALARM_ID;     // Identification de l'alarme minuteur 60 secondes dtINVALID_ALARM_ID= 255 dans bibliothèque
uint8_t Alarme[] = {dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID}; // Identification de l'alarme 0 à 5 (6 alarmes) valeur par defaut 255
uint8_t ReseauOut = 0;                      // Variable Compteur Nombre de de perte du wifi
boolean WifiAP = false;                     // Mode wifi AP
String WIFI_SSID_G = "";                    // Variable globale pour configuration SSID client Web
unsigned long last_wifi_check_time = 0;
String modes = "";                          // Variable pour la gestion des modes
uint8_t myModes[] = {};                     // Liste des modes
String getContentType(String filename);     // convert the file extension to the MIME type
bool handleFileRead(String path);           // send the right file to the client (if it exists)

//Instance des objets
WS2812FX ws2812fx = WS2812FX(1, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(HTTP_PORT);
ADC_MODE(ADC_VCC);                        // utilisation de ESP.getVcc tension d'alimentation de l'esp

WiFiUDP ntpUDP;
// Vous pouvez spécifier le nom du serveur de temps et le décalage en secondes, peut être
// modifié plus tard avec setTimeOffset (). De plus, vous pouvez spécifier l'
// intervalle de mise à jour en millisecondes, avec setUpdateInterval ().
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void LampeChange(uint8_t brightness);              // Retour des informations
Espalexa espalexa;                                 // Alexa

void setup() {
  EEPROM.begin(512);                                //Initialise zone mémoire dans eeprom
  Serial.begin(115200);                             //Vitesse liaison série 115200
  Serial.println();                                 //Saut de ligne
  Serial.println();                                 //Saut de ligne
  Serial.println("**** Information ESP8266 ****");
  Serial.print("Taille segment libre: "); Serial.print(ESP.getFreeHeap()); Serial.println(" Octets");
  Serial.print("Version boot: "); Serial.println(ESP.getBootVersion());
  Serial.print("CPU: "); Serial.print(ESP.getCpuFreqMHz()); Serial.println(" Mhz");
  Serial.print("Version SDK: "); Serial.println(ESP.getSdkVersion());
  Serial.print("ID de la puce: "); Serial.println(ESP.getChipId());
  Serial.print("ID de EEPROM: "); Serial.println(ESP.getFlashChipId());
  Serial.print("Taille de EEPROM: "); Serial.print(ESP.getFlashChipRealSize()); Serial.println(" Octets");
  Serial.print("Alimentation Vcc: "); Serial.print((float)ESP.getVcc() / 1024.0); Serial.println(" V");
  modes.reserve(5000);
  modes_setup();
  initLed();                                                      // Initialisation du bandeau LED
  wifi_setup();                                                   // Initialisation du wifi
  int8_t tmp = EEPROM.read(ADRESS_GMT);                           // Lecture du fuseau horaire
  if (tmp > -12 && tmp < 13) timeClient.setTimeOffset(3600 * tmp); // Initialisation du fuseau
  timeClient.begin();                                             // Démarrage du client NTP
  SPIFFS.begin();                                                 // Démarrage du SPI Flash Files System
  init_server();                                                  // Initialisation des serveurs
  Date_Heure();                                                   // initialisation de la date et de l'heure
  AlarmInit();                                                    // Initialisation des alarmes
  if (WifiAP == false) InitAlexa();                               // Initialisation d'Alexa
}

void loop() {
  conf_serie();                                        // Configuration via liaison série
  server.handleClient();
  ws2812fx.service();

  if (WifiAP == false) {                              // mode sur réseau WIFI avec routeur
    Date_Heure();
    Alarm.delay(0);
    wifi_verif();
    espalexa.loop();
  }
}

// Raz esp8266
void raz() {
  ws2812fx.stop();         // Arrêt LED
  delay(500);
  WiFi.mode(WIFI_OFF);     // Arrêt du WIFI
  ESP.restart();           // Redémarrage
}



