
/* #####################################################
  #  Serveur web
  ##################################################### */

/*
   Création de la liste des modes pour le client WEB.
*/
void modes_setup() {
  modes = "";
  uint8_t num_modes = sizeof(myModes) > 0 ? sizeof(myModes) : ws2812fx->getModeCount();
  for (uint8_t i = 0; i < num_modes; i++) {
    uint8_t m = sizeof(myModes) > 0 ? myModes[i] : i;
    modes += "<li><a href='#' class='m' id='";
    modes += m;
    modes += "'>";
    modes += ws2812fx->getModeName(m);
    modes += "</a></li>";
  }
}



void init_server() {
  Serial.println("initialisation du serveur HTTP");
  server.onNotFound([]() {                              // If the client requests any URI
    if (!espalexa.handleAlexaApiCall(server.uri(), server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
      if (!handleFileRead(server.uri()))                // send it if it exists
        server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server.on("/modes", srv_handle_modes);                // Serveur pour la liste des modes
  server.on("/set", srv_handle_set);                    // Serveur lecture des commandes depuis client
  server.on("/DateHeure", srv_handle_dateheure);        // Serveur pour affichage de la date et l'heure
  server.on("/Minuteur", srv_handle_minuteur);          // Serveur pour affichage tempo
  server.on("/Etat", srv_handle_etat);                  // Serveur pour affichage des paramètres
  server.begin();                                       // Démarrage du serveur web
  Serial.println("Serveur HTTP démarré.");
  Serial.println("Serveur HTTP prêt");
}


String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if ((path == "/parametres.html")and (Admin==0)) return false;             // Blocage de l'accès aux paramètres
  if (path.endsWith("/")) path += "index.html";                             // If a folder is requested, send the index file
  if (path == "/index.html") Admin=0;                                      // Blocage de l'accès aux paramètres apres retour à la page de base
  String contentType = getContentType(path);                                // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {                   // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                                          // If there's a compressed version available
      path += ".gz";                                                        // Use the compressed version
    File file = SPIFFS.open(path, "r");                                     // Open the file
    size_t sent = server.streamFile(file, contentType);                     // Send it to the client
    file.close(); // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false; // If the file doesn't exist, return false
}

void srv_handle_modes() {
  server.send(200, "text/plain", modes);
}
void srv_handle_dateheure() {
  server.send(200, "text/plain", DateHeure);
}

void srv_handle_minuteur() {
  server.send(200, "text/plain", AffMinuteur);
}

// Serveur pour la lecture des états
void srv_handle_etat() {
  String valeur;
  char buf[10];
  long temp;
  for (uint8_t i = 0; i < server.args(); i++) {
    // Etat wifi
    if (server.argName(i) == "wifi") {
      valeur = (&server.arg(i)[0]);
      if (valeur == "ssid") {
        server.send(200, "text/plain", (LectureWifiEeprom(1).c_str()));        // Lecture WIFI SSID
      }
      if (valeur == "ip") {
        server.send(200, "text/plain", (WiFi.localIP().toString().c_str()));  // Lecture WIFI IP
      }
      if (valeur == "routeur") {
        server.send(200, "text/plain", (WiFi.gatewayIP().toString().c_str())); // Lecture WIFI IP passerelle
      }
      if (valeur == "host") {
        server.send(200, "text/plain", (WiFi.hostname().c_str())); // Lecture WIFI hostname
      }
      if (valeur == "ipdns") {
        server.send(200, "text/plain", (WiFi.dnsIP().toString().c_str()));           // Lecture WIFI IP DNS
      }
      if (valeur == "mask") {
        server.send(200, "text/plain", (WiFi.subnetMask().toString().c_str()));     // Lecture WIFI masque sous reseau
      }
    }
    // Etat configuration
    if (server.argName(i) == "conf") {
      valeur = (&server.arg(i)[0]);
      if (valeur == "nbled") {
        temp = (EEPROMReadlong(ADRESS_NLED, 2));
        ltoa(temp, buf, 10);
        server.send(200, "text/plain", buf);        // Lecture nb led
      }
      if (valeur == "timer") {
        temp = (EEPROMReadlong(ADRESS_MINUTEUR, 2));
        if (temp > 0) temp = (temp / 60); else temp = 0;
        ltoa(temp, buf, 10);
        server.send(200, "text/plain", buf);        // Lecture valeur timer
      }
      if (valeur == "brled") {
        int8_t temp = EEPROM.read(ADRESS_PIN_LED);
        ltoa(temp, buf, 10);
        server.send(200, "text/plain", buf);        // Lecture Broche de sortie led
      }
      if (valeur == "gmt") {
        int8_t temp = EEPROM.read(ADRESS_GMT);
        ltoa(temp, buf, 10);
        server.send(200, "text/plain", buf);        // Lecture valeur GMT
      }
      if (valeur == "alexa") {
        server.send(200, "text/plain", (LectureStringEeprom(ADRESS_NOM_ALEXA, 32)));       // Lecture valeur nom alexa
      }
    }
    // Etat alarmes
    if (server.argName(i) == "alarme") {
      valeur = (&server.arg(i)[0]);
      for (i = 0; i < NB_ALARME; i++) {
        if (valeur.toInt() == i) {
          server.send(200, "text/plain", EtatAl(i));        // Lecture Alarme
        }
      }
    }
    // Etat liste
    if (server.argName(i) == "liste") {
      valeur = (&server.arg(i)[0]);
      // Liste des modes
      if (valeur == "modes") {
        String modes = "<datalist id=\"modes\">";
        uint8_t num_modes = sizeof(myModes) > 0 ? sizeof(myModes) : ws2812fx->getModeCount();
        for (uint8_t i = 0; i < num_modes; i++) {
          uint8_t m = sizeof(myModes) > 0 ? myModes[i] : i;
          Serial.println(ws2812fx->getModeName(m));
          modes = modes + "<option value=\"" + (ws2812fx->getModeName(m)) + "\"" + ">" + "\n";
        }
        server.send(200, "text/plaint", modes);        //Liste modes
      }
    }
  }

}

// Reception des commandes depuis le client web

void srv_handle_set() {
  // c: pour la couleur m:pour le mode b: Luminosité s:Vitesse a: cycle auto p: marche/arrêt ta:minuteur
  String WIFI_PASSWORD;
  String WIFI_IP;

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "c") {
      uint32_t tmp = (uint32_t) strtol(&server.arg(i)[0], NULL, 16);
      if (tmp >= 0x000000 && tmp <= 0xFFFFFF) {
        ws2812fx->setColor(tmp);
        EEPROMWritelong(ADRESS_LED_COL, tmp, 4); // Sauvegarde de la couleur
        EEPROM.commit();
      }
    }

    if (server.argName(i) == "m") {
      uint8_t tmp = (uint8_t) strtol(&server.arg(i)[0], NULL, 10);
      ws2812fx->setNumSegments(1);                     // utilisation du segment LED 1
      ws2812fx->setMode(tmp % ws2812fx->getModeCount());
      Serial.print("Mode: "); Serial.println(ws2812fx->getModeName(ws2812fx->getMode()));
      EEPROM.write(ADRESS_LED_MOD, ws2812fx->getMode()); // Sauvegarde du mode
      EEPROM.commit();
    }

    if (server.argName(i) == "b") {
      uint8_t tmp = (uint8_t) strtol(&server.arg(i)[0], NULL, 10);
      if (server.arg(i)[0] == 'm') {
        ws2812fx->setBrightness(max((int)(ws2812fx->getBrightness() * 0.8), 5));
        tmp = 0;
      } else if (server.arg(i)[0] == 'p') {
        ws2812fx->setBrightness(min((int)(ws2812fx->getBrightness() * 1.2), 255));
        tmp = 0;
      } else if (tmp > 0) {
        ws2812fx->setBrightness(tmp);
      }
      Serial.print("Puissance lumineuse: "); Serial.println(ws2812fx->getBrightness());
      EEPROM.write(ADRESS_LED_LUM, ws2812fx->getBrightness()); // Sauvegarde de la luminosité
      EEPROM.commit();
    }

    if (server.argName(i) == "s") {
      uint32_t tmp = (uint32_t) strtol(&server.arg(i)[0], NULL, 10);
      if (server.arg(i)[0] == 'm') {
        ws2812fx->setSpeed(ws2812fx->getSpeed() * 0.8);
        tmp = 0;
      } else if (server.arg(i)[0] == 'p') {
        ws2812fx->setSpeed(ws2812fx->getSpeed() * 1.2);
        tmp = 0;
      } else if (tmp > 0) {
        ws2812fx->setSpeed(tmp);
      }
      Serial.print("Vitesse:  "); Serial.println(ws2812fx->getSpeed());
      EEPROMWritelong(ADRESS_LED_VIT, ws2812fx->getSpeed(), 2); // Sauvegarde de la vitesse
      EEPROM.commit();
    }

    if (server.argName(i) == "p") {                                // Marche arrêt bandeau LED
      if (server.arg(i)[0] == 'm') {
        MinuteurStop = (EEPROMReadlong(ADRESS_MINUTEUR, 2));       // Lecture valeur minuteur dans Mémoire.
        Alarm.disable(AlarmeMin);
        ws2812fx->start();
        EEPROM.write(ADRESS_ON_OFF, 1);                            // Sauvegarde LED en marche
      }
      if (server.arg(i)[0] == 'a') {
        Alarm.enable(AlarmeMin);                                 // Mise en marche de la minuterie
        if (EEPROM.read(ADRESS_ON_OFF) == 0) {                   // Arrêt du bandeau apres deuxieme appuie sur arrêt
          MinuteurStop = (EEPROMReadlong(ADRESS_MINUTEUR, 2)); // Lecture valeur minuteur dams Mémoire.
          Alarm.disable(AlarmeMin);
          ws2812fx->stop();
        }
        EEPROM.write(ADRESS_ON_OFF, 0);                         // Sauvegarde LED à l'arrêt
      }
      EEPROM.commit();
    }
    if (server.argName(i) == "ta") { // Valeur de la minuterie 0- 600 mn pour arrêt en minutes
      MinuteurStop = 60 * ((int)strtol(&server.arg(i)[0], NULL, 10));
      EEPROMWritelong(ADRESS_MINUTEUR, MinuteurStop, 2); // Sauvegarde valeur minuteur en seconde
    }
    // Alarme
    if (server.argName(i) == "tal") { // Alarme
      Active_Alarme(String(&server.arg(i)[0]));
    }
    // Custom Effet
    if (server.argName(i) == "cus") { // Custom effet
      Custom_Effet(String(&server.arg(i)[0]));
    }
    // Réinitialisation
    if (server.argName(i) == "raz") {
      raz();
    }
    // WIFI SSID
    if (server.argName(i) == "ssid") {
      WIFI_SSID_G = (&server.arg(i)[0]);
      Serial.println("Configuration Client Web SSID: " + WIFI_SSID_G);
    }
    // WIFI mot de passe
    if (server.argName(i) == "password") {
      WIFI_PASSWORD = (&server.arg(i)[0]);
      Serial.println("Configuration Client Web Mdp: " + WIFI_PASSWORD);
      EcritureWifiEeprom(WIFI_SSID_G, WIFI_PASSWORD);
    }
    // WIFI IP FIXE ou DHCP
    if (server.argName(i) == "ipfixe") {
      WIFI_IP = (&server.arg(i)[0]);
      EEPROM.write(ADRESS_RESEAU, 0);
      if (WIFI_IP != "0") {
        EEPROM.write(ADRESS_RESEAU, 1);
        EEPROM.write(ADRESS_RESEAU + 5, IPnum(WIFI_IP, 3));
      }
      EEPROM.commit();
      Serial.println("Configuration Client Web FIXE/DHCP: " + String(EEPROM.read(ADRESS_RESEAU)));
      Serial.println("Configuration Client Web IP FIXE: " + String(EEPROM.read(ADRESS_RESEAU + 5)));
    }
    // WIFI IP Passerelle
    if (server.argName(i) == "ippasse") {
      WIFI_IP = (&server.arg(i)[0]);
      Serial.print("Configuration Client Web IP Passerelle: ");
      for (int i = 0; i < 4; ++i)
      {
        EEPROM.write(i + ADRESS_RESEAU + 1, IPnum(WIFI_IP, i));
        EEPROM.commit();
        Serial.print(String(EEPROM.read(i + ADRESS_RESEAU + 1)) + ".");
      }
      Serial.println("");
    }
    // Nombres de LED
    if (server.argName(i) == "nbled") {
      EEPROMWritelong(ADRESS_NLED, String(&server.arg(i)[0]).toInt(), 2);
      EEPROM.commit();
      Serial.println("Configuration Client Web Nb LED: " + String(EEPROM.read(ADRESS_NLED)));
    }
    // Broche de sortie LED
    if (server.argName(i) == "brled") {
      EEPROMWritelong(ADRESS_PIN_LED, String(&server.arg(i)[0]).toInt(), 2);
      EEPROM.commit();
      Serial.println("Configuration Client Web Broche LED: " + String(EEPROM.read(ADRESS_PIN_LED)));
    }
    // Heure GMT
    if (server.argName(i) == "gmt") {
      int8_t tmp = (int8_t) strtol(&server.arg(i)[0], NULL, 10);
      EEPROM.write(ADRESS_GMT, tmp);
      EEPROM.commit();
      tmp = EEPROM.read(ADRESS_GMT);
      if (tmp > -12 && tmp < 13) timeClient.setTimeOffset(3600 * tmp); // Initialisation du fuseau
      Serial.println("Configuration GMT: " + String(tmp));
    }
    // Nom du périphérique pour Alexa Commande Vocal
    if (server.argName(i) == "alexa") {
      WIFI_SSID_G = (&server.arg(i)[0]);
      EcritureStringEeprom((&server.arg(i)[0]), ADRESS_NOM_ALEXA, 32);
      Serial.println("Configuration Nom périphérique Alexa: " + LectureStringEeprom(ADRESS_NOM_ALEXA, 32));
    }
    // Mot de passe pour OTA et paramètrage
    if (server.argName(i) == "mdp") {
      WIFI_SSID_G = (&server.arg(i)[0]);
      EcritureStringEeprom((&server.arg(i)[0]), ADRESS_PASSWORD, 32);
      Serial.println("Mot de passe pour OTA et paramètrage: " + LectureStringEeprom(ADRESS_PASSWORD, 32));
    }
    // Validation du mot de passe pour accès aux paramètres
    if (server.argName(i) == "login") {
      WIFI_SSID_G = (&server.arg(i)[0]);
      //EcritureStringEeprom((&server.arg(i)[0]),ADRESS_PASSWORD,32);
      if (LectureStringEeprom(ADRESS_PASSWORD, 32) == WIFI_SSID_G) {
      Serial.println("Accès aux paramètres validés");
        Admin = true;
      } else {
        Serial.println("Accès aux paramètres invalidés");
        Admin = false;
      }

    }
  }
  server.send(200, "text/plain", "OK");
}
