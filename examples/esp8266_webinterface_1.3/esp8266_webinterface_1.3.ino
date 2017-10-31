/*
  WS2812FX Webinterface.
  
  Harm Aldick - 2016
  www.aldick.org

  
  FEATURES
    * Webinterface with mode, color, speed and brightness selectors


  LICENSE

  The MIT License (MIT)

  Copyright (c) 2016  Harm Aldick 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  
  CHANGELOG
  2016-11-26 initial version
  
*/
/* Christophe Caron www.caron.ws 2017
 * Sauvegarde dans EEPROM paramètres bandeau LED 19/10/2017
 * Sauvegarde dans EEPROM paramètres WIFI 20/10/2017 V1.0
 * Sauvegarde dans EEPROM paramètres Réseau 22/10/2017 et traduction en français V1.1
 * Ajout des information sur ESP01 23/10/2017 V1.2
 * Traduction en français et modification de l'interface WEB V1.3 ajout de jscolor 31/10/2017
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WS2812FX_fr.h>
#include <EEPROM.h>

#include "index.html.h"
#include "main.js.h"

// QUICKFIX...See https://github.com/esp8266/Arduino/issues/263
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define LED_PIN 2                       // 0 = GPIO0, 2=GPIO2 Broche de connexion de la comunication du bandeu LED
#define LED_COUNT 60                    // Nombres de LED

#define WIFI_TIMEOUT 30000              // checks WiFi every ...ms. Reset after this time, if WiFi cannot reconnect.
#define HTTP_PORT 80                    // Port de connexion pour le serveur HTTP

#define BRIGHTNESS_STEP 15              // in/decrease brightness by this amount per click
#define SPEED_STEP 10                   // in/decrease brightness by this amount per click

#define ADRESS_LED 0                    // Adresse de départ paramètres LED (Fin ADRESS_LED + 7 mots)
#define ADRESS_WIFI 7                   // Adresse de départ paramètres WIFI Mot de passe et SSID (Fin ADRESS_WIFI + 96 mots)
#define ADRESS_RESEAU 96                // Adresse de départ paramètres WIFI Mot de passe et SSID (Fin ADRESS_WIFI + 5 mots)

unsigned long last_wifi_check_time = 0;
String modes = "";

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server = ESP8266WebServer(HTTP_PORT);

ADC_MODE(ADC_VCC); // utilisation de ESP.getVcc

void setup(){
  EEPROM.begin(512);                                //Initialise zone mémoire dans eeprom
  Serial.begin(115200);                             //Initialisation de la liaison série
  
  Serial.println();                                 //Saut de ligne 
  Serial.println();                                 //Saut de ligne 
  Serial.println("**** Information ESP01 ****");            
  Serial.print("Taille segment libre: "); Serial.print(ESP.getFreeHeap());Serial.println(" Octets");
  Serial.print("Version boot: "); Serial.println(ESP.getBootVersion());
  Serial.print("CPU: ");Serial.print(ESP.getCpuFreqMHz());Serial.println(" Mhz");
  Serial.print("Version SDK: "); Serial.println(ESP.getSdkVersion());
  Serial.print("ID de la puce: "); Serial.println(ESP.getChipId());
  Serial.print("ID de EEPROM: "); Serial.println(ESP.getFlashChipId());
  Serial.print("Taille de EEPROM: "); Serial.print(ESP.getFlashChipRealSize());Serial.println(" Octets");
  Serial.print("Alimentation Vcc: "); Serial.print((float)ESP.getVcc()/1024.0);Serial.println(" V");
  
  Serial.println();                                 //Saut de ligne
  Serial.println("Démarrage...");

  modes.reserve(5000);
  modes_setup();

  Serial.println("Initialisation du bandeau LED WS2812FX");
  ws2812fx.init();
  ws2812fx.setBrightness(EEPROM.read(ADRESS_LED));    // Lecture valeur Mémoire adress 0 pour la luminosité
  ws2812fx.setSpeed(EEPROM.read(ADRESS_LED + 1));     // Lecture valeur Mémoire adress 1 pour la vitesse
  ws2812fx.setMode(EEPROM.read(ADRESS_LED + 2));      // Lecture valeur Mémoire adress 2 pour le mode
  ws2812fx.setColor(EEPROMReadlong(ADRESS_LED + 3));  // Lecture valeur Mémoire adress 3 pour la couleur
  ws2812fx.start();

  Serial.println("Initialisation du Wifi");
  wifi_setup();
 
  Serial.println("Serveur HTTP initialisé");
  server.on("/", srv_handle_index_html);
  server.on("/main.js", srv_handle_main_js);
  server.on("/modes", srv_handle_modes);
  server.on("/set", srv_handle_set);
  server.onNotFound(srv_handle_not_found);
  server.begin();
  Serial.println("Serveur HTTP démarré.");
  Serial.println("Serveur HTTP prêt");
}


void loop() {
  unsigned long now = millis();
  String valeur;
  String WIFI_SSID;
  String WIFI_PASSWORD;

// Lecture port série
  while(Serial.available()) {
     valeur= Serial.readString();                             // Lecture des données sur la liaison série
     if (valeur=="wifi"){
        Serial.println("Modification des paramètres wifi");
        Serial.println("Entrer le SSID du wifi: ");          //Information de la saisie 
        while (Serial.available()==0) {                      //Attendre la saisie
        }
        WIFI_SSID=Serial.readString();                      //Lecture et enregistrement dans la variable
        Serial.println("Entrer le mot de passe du wifi: "); //Information de la saisie
        while (Serial.available()==0) {                     //Attendre la saisie
        }
        WIFI_PASSWORD=Serial.readString();                  //Lecture et enregistrement dans la variable
        EcitureWifiEeprom(WIFI_SSID,WIFI_PASSWORD);
        Serial.println("DHCP=0 ou IP FIXE=1?: ");           //Information de la saisie
        while (Serial.available()==0) {                     //Attendre la saisie
        }
        valeur=Serial.readString();                         //Lecture et enregistrement dans la variable
        if (valeur=="0") {
           EEPROM.write(ADRESS_RESEAU,0);                   //  DHCP valeur à 0
        }
        else
        {
          EEPROM.write(ADRESS_RESEAU,1);                         // IP statique à 1
          Serial.println("Adresse de la passerelle: ");          //Information de la saisie
          while (Serial.available()==0) {                        //Attendre la saisie
          }
          valeur=Serial.readString();                           //Lecture et enregistrement dans la variable                              
          for (int i = 0; i <4; ++i)
             {
             EEPROM.write(i + ADRESS_RESEAU +1, IPnum(valeur,i));
             }
          Serial.println("IP Statique : ");                       //Information de la saisie
          while (Serial.available()==0) {                        //Attendre la saisie
          }
          valeur=Serial.readString();                           //Lecture et enregistrement dans la variable  
          EEPROM.write(ADRESS_RESEAU +5, valeur.toInt());     
          }
     Serial.println("Vérification des paramètres de sauvegarde: "); //Information
     Serial.println(LectureWifiEeprom(1));
     Serial.println(LectureWifiEeprom(2));
     for (int i = 0; i <6; ++i)
       {
       Serial.printf("Valeur Réseau %d",EEPROM.read(ADRESS_RESEAU+i));
       }
     ESP.reset();                                          // Rest du module ESP01
}
}

  server.handleClient();
  ws2812fx.service();

 if(now - last_wifi_check_time > WIFI_TIMEOUT) {
    Serial.print("Vérification du WiFi... ");
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Erreur de connexion WIFI ...");
      Serial.println("Taper wifi pour modifier les paramètres du WIFI");
    } else {
      Serial.println("OK");
    }
    last_wifi_check_time = now;
  }
}

/*
 * Connect to WiFi. If no connection is made within WIFI_TIMEOUT, ESP gets resettet.
 */
void wifi_setup() {
  String WIFI_SSID;
  String WIFI_PASSWORD;
  WIFI_SSID=(LectureWifiEeprom(1));       // Lecture des paramètres WIFI SSID
  WIFI_PASSWORD=(LectureWifiEeprom(2));   // Lecture des paramètres WIFI Mot de passe
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
  WiFi.mode(WIFI_STA);

  if (EEPROM.read(ADRESS_RESEAU)==1){               // Lecture des paramètes IP fixe
     IPAddress ip(EEPROM.read(ADRESS_RESEAU+1),EEPROM.read(ADRESS_RESEAU+2),EEPROM.read(ADRESS_RESEAU+3),EEPROM.read(ADRESS_RESEAU+5));
     IPAddress gateway(EEPROM.read(ADRESS_RESEAU+1),EEPROM.read(ADRESS_RESEAU+2),EEPROM.read(ADRESS_RESEAU+3),EEPROM.read(ADRESS_RESEAU+4));
     IPAddress subnet(255,255,255,0);
     WiFi.config(ip, gateway, subnet);
  }


  unsigned long connect_start = millis();
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if(millis() - connect_start > WIFI_TIMEOUT) {
      Serial.println();
      Serial.println("Erreur de connexion WIFI. Vérifier vos paramètres WIFI");
      Serial.print("Dépassement du temps de connexion: ");
      Serial.println(WIFI_TIMEOUT);
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi est connecté");  
  Serial.print("Addresse IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Ip en mode ");
  if (EEPROM.read(ADRESS_RESEAU)==1){
    Serial.println("Statique");
  }
  else {
    Serial.println("DHCP");
  }
  Serial.println();
}


/*
 * Build <li> string for all modes.
 */
void modes_setup() {
  modes = "";
  for(uint8_t i=0; i < ws2812fx.getModeCount(); i++) {
    modes += "<li><a href='#' class='m' id='";
    modes += i;
    modes += "'>";
    modes += ws2812fx.getModeName(i);
    modes += "</a></li>";
  }
}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }


/* #####################################################
#  Webserver Functions
##################################################### */

void srv_handle_not_found() {
  server.send(404, "text/plain", "File Not Found");
}

void srv_handle_index_html() {
  server.send_P(200,"text/html", index_html);
}

void srv_handle_main_js() {
  server.send_P(200,"application/javascript", main_js);
}

void srv_handle_modes() {
  server.send(200,"text/plain", modes);
}

void srv_handle_set() {
  for (uint8_t i=0; i < server.args(); i++){
    if(server.argName(i) == "c") {
      uint32_t tmp = (uint32_t) strtol(&server.arg(i)[0], NULL, 16);
      if(tmp >= 0x000000 && tmp <= 0xFFFFFF) {
        ws2812fx.setColor(tmp);
        EEPROMWritelong(ADRESS_LED + 3,tmp); // Sauvegarde de la couleur
        EEPROM.commit();
      }
    }

    if(server.argName(i) == "m") {
      uint8_t tmp = (uint8_t) strtol(&server.arg(i)[0], NULL, 10);
      ws2812fx.setMode(tmp % ws2812fx.getModeCount());
      EEPROM.write(ADRESS_LED + 2,ws2812fx.getMode()); // Sauvegarde du mode 
      EEPROM.commit();
    }

    if(server.argName(i) == "b") {
      if(server.arg(i)[0] == '-') {
        ws2812fx.decreaseBrightness(BRIGHTNESS_STEP);
      } else {
        ws2812fx.increaseBrightness(BRIGHTNESS_STEP);
      }
      EEPROM.write(ADRESS_LED,ws2812fx.getBrightness()); // Sauvegarde de la luminosité
      EEPROM.commit();
    }

    if(server.argName(i) == "s") {
      if(server.arg(i)[0] == '-') {
        ws2812fx.decreaseSpeed(SPEED_STEP);
      } else {
        ws2812fx.increaseSpeed(SPEED_STEP);
      }
      EEPROM.write(ADRESS_LED + 1,ws2812fx.getSpeed());   // Sauvegarde de la vitesse
      EEPROM.commit();
    }
  }
  server.send(200, "text/plain", "OK");
}


// Paramètres WIFI
// Effacement de la zone mémoire avant écriture
void EffaceWifiEeprom() {
  for (int i =ADRESS_WIFI ; i < ADRESS_WIFI + 96; ++i) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

// Ecriture des paramètres WIFI
void EcitureWifiEeprom(String ssid,String password) {
   EffaceWifiEeprom();
   //Ecriture du SSID Max 32 caractères
   for (int i = 0; i < ssid.length(); ++i)
    {
      EEPROM.write(i + ADRESS_WIFI, ssid[i]);
    }
    //Ecriture du mot de passe Max 64 caractères
    for (int i = 0; i < password.length(); ++i)
    {
      EEPROM.write(32 + ADRESS_WIFI + i, password[i]);
    }
  EEPROM.commit();
}

// Lecture des paramètres WIFI si a=1 => SSID si a=2 => mot de passe
String LectureWifiEeprom(int a)
{
  // Lecture du SSID 
 String ssid = "";
 for (int i = 0; i < 32; ++i)
  {
    ssid += char(EEPROM.read(i+ADRESS_WIFI));
  }
  // Lecture du mot de passe
 String password = "";
  for (int i = 32; i < 96; ++i)
  {
    password += char(EEPROM.read(i + ADRESS_WIFI));
  } 
  EEPROM.commit();
  if (a==1) return String(ssid);
  if (a==2) return String(password);
}

// conversion IP chaine en en valeur numérique
int IPnum(String valeur,int a){
  int str_len = valeur.length() + 1; 
  char *str="255.255.255.255";

  valeur.toCharArray(str, str_len); // Transfert chaine dans tableau de caractère
    unsigned char value[4] = {0};
    size_t index = 0;
    while (*str) {
        if (isdigit((unsigned char)*str)) {
            value[index] *= 10;
            value[index] += *str - '0';
        } else {
            index++;
        }
        str++;
    }
    return int(value[a]);
}

