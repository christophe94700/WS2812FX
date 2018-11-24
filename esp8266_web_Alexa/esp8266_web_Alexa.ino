#include <ESP8266WiFi.h>                    //Inclusion bibliothèque gestion du WIFI de l'ESP8266
#include <ESP8266mDNS.h>                    // Inclusion bibliothèque mDNS
#include <Espalexa.h>                       //Inclusion bibliothèque pour commande avec Alexa Amazone
#include <ESP8266WebServer.h>               //Inclusion bibliothèque gestion du serveur web de l'ESP8266
#include <WS2812FX_fr.h>                    //Inclusion bibliothèque gestion des LED WS2812
#include <EEPROM.h>                         //Inclusion bibliothèque gestion de l'EEPROM
#include <FS.h>                             //Inclusion bibliothèque SPIFFS
#include <WiFiUdp.h>                        //Inclusion bibliothèque pour la gestion de l'User Datagram Protocol
#include <ArduinoOTA.h>                     //Inclusion bibliothèque pour mise à via le WIFI
#include <NTPClient.h>                      //Inclusion bibliothèque gestion serveur NTP
#include <TimeLib.h>                        //Inclusion bibliothèque gestion des fonctionnalités de chronométrage
#include <TimeAlarms.h>                     //Inclusion bibliothèque gestion des fonctionnalités d'alarme et minuterie
#include <ESP8266Ping.h>                    //Inclusion bibliothèque pour le ping et modification de byte count = 1 dans la bibliothèque
#include <StringSplitter.h>                 //Inclusion bibliothèque pour création d'un tableau depuis chaine avec séparateur Modifier nbrs MAX 5 => 10
#include "Definition.h"                     //Inclusion des définitions

extern "C" {
#include "user_interface.h"
}

// Variables globales
String DateHeure = "";                      // Variable pour gestion de la date et de l'heure
String AffMinuteur = "";                    // Variable globale de la valeur du minuteur pour client web
String AffEtat = "";                        // Variable globale pour affichage état pour client web
int MinuteurStop = 0;                       // Variable globale de la valeur du minuteur
uint8_t AlarmeMin = dtINVALID_ALARM_ID;     // Identification de l'alarme minuteur 60 secondes dtINVALID_ALARM_ID= 255 dans bibliothèque
uint8_t Alarme[] = {dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID, dtINVALID_ALARM_ID}; // Identification de l'alarme 0 à 5 (6 alarmes) valeur par defaut 255
uint8_t ReseauOut = 0;                      // Variable Compteur Nombre de de perte du wifi
String WIFI_SSID_G = "";                    // Variable globale pour configuration SSID client Web
unsigned long Twifiap = 0;                   // Variable temps d'utilisation du mode WIFI AP
unsigned long last_wifi_check_time = 0;
String modes = "";                          // Variable pour la gestion des modes
uint8_t myModes[] = {};                     // Liste des modes
String getContentType(String filename);     // convert the file extension to the MIME type
bool handleFileRead(String path);           // send the right file to the client (if it exists)
bool TimerON = 0;
bool Admin = 0;                             // 0= pas de login 1=login validé

//Instance des objets

WS2812FX *ws2812fx;
ESP8266WebServer server(HTTP_PORT);
ADC_MODE(ADC_VCC);                        // utilisation de ESP.getVcc tension d'alimentation de l'esp

WiFiUDP ntpUDP;
// Vous pouvez spécifier le nom du serveur de temps et le décalage en secondes, peut être
// modifié plus tard avec setTimeOffset (). De plus, vous pouvez spécifier l'
// intervalle de mise à jour en millisecondes, avec setUpdateInterval ().
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 3600000);

void LampeChange(uint8_t brightness);              // Retour des informations
Espalexa espalexa;                                 // Alexa

void setup() {
  EEPROM.begin(512);                                //Initialise zone mémoire dans eeprom
  InitEeprom(0);                                     //Initialisation EEprom apres effacement
  ws2812fx = new WS2812FX(1, EEPROM.read(ADRESS_PIN_LED), NEO_GRB + NEO_KHZ800); // Instance pour gestion du bandeau LED
  Serial.begin(115200);                             //Vitesse liaison série 115200
  Info_reboot();                                    // Information sur l'origine du reboot
  Info_ESP();                                       // Information esp8266
  modes.reserve(5000);
  modes_setup();
  initLed();                                                      // Initialisation du bandeau LED
  wifi_setup();                                                   // Initialisation du wifi
  int8_t tmp = EEPROM.read(ADRESS_GMT);                           // Lecture du fuseau horaire
  if (tmp > -12 && tmp < 13) timeClient.setTimeOffset(3600 * tmp);// Initialisation du fuseau
  timeClient.begin();                                             // Démarrage du client NTP
  SPIFFS.begin();                                                 // Démarrage du SPI Flash Files System
  init_server();                                                  // Initialisation des serveurs
  Date_Heure();                                                   // initialisation de la date et de l'heure
  AlarmInit();                                                    // Initialisation des alarmes
  Twifiap = millis();                                             // Initialisation du temps en mode AP
  if (WiFi.status() == WL_CONNECTED) {                            // Initialisation si connexion WIFI
    InitAlexa();                                                  // Initialisation d'Alexa
    ArduinoOTA.setHostname(("MyLED" + String(ESP.getChipId())).c_str());          // Nom du module pour mise à jour et pour le mDNS
    ArduinoOTA.setPassword((LectureStringEeprom(ADRESS_PASSWORD, 32)).c_str());  // Mot de passe pour mise à jour
    ArduinoOTA.onEnd([]() {                                                      // Effacement EEPROM après mise à jour
      InitEeprom(1);
    });
    ArduinoOTA.begin();                                                           // Initialisation de l'OTA
  }
}

void loop() {
  conf_serie();                                        // Configuration via liaison série
  server.handleClient();
  ws2812fx->service();
  delay(1);

  if (WiFi.status() == WL_CONNECTED) {                 // mode sur réseau WIFI avec routeur
    Date_Heure();
    Alarm.delay(0);
    wifi_verif();
    espalexa.loop();
    Twifiap = millis();
    ArduinoOTA.handle();
  } else {
    if (millis() - Twifiap > WIFIAP_TIMEOUT) {        // Temps en mode AP
      raz();
    }
  }
}

// Raz esp8266
void raz() {
  ws2812fx->stop();         // Arrêt LED
  if (TimerON == 1) {
    EEPROM.write(ADRESS_TIMER, 1);  // Sauvegarde de la marche timer
  } else {
    EEPROM.write(ADRESS_TIMER, 0);  // Sauvegarde l'arrêt timer
  }
  EEPROM.commit();
  delay(500);
  WiFi.mode(WIFI_OFF);     // Arrêt du WIFI
  Serial.println("+++ Redémarrage du module +++");        //Saut de ligne
  ESP.restart();           // Redémarrage
}
