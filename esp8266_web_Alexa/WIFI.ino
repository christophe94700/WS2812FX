/*
   Initialisation du WIFI
*/
void wifi_setup() {
  String WIFI_SSID;
  String WIFI_PASSWORD;
  WIFI_SSID = (LectureWifiEeprom(1));     // Lecture des paramètres WIFI SSID
  WIFI_PASSWORD = (LectureWifiEeprom(2)); // Lecture des paramètres WIFI Mot de passe
  WiFi.hostname(("MyLED" + String(ESP.getChipId())).c_str()); 
  Serial.println("Démarrage...");
  Serial.println("Initialisation du Wifi");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  if (EEPROM.read(ADRESS_RESEAU) == 1) {
    IPAddress ip(EEPROM.read(ADRESS_RESEAU + 1), EEPROM.read(ADRESS_RESEAU + 2), EEPROM.read(ADRESS_RESEAU + 3), EEPROM.read(ADRESS_RESEAU + 5));
    IPAddress gateway(EEPROM.read(ADRESS_RESEAU + 1), EEPROM.read(ADRESS_RESEAU + 2), EEPROM.read(ADRESS_RESEAU + 3), EEPROM.read(ADRESS_RESEAU + 4));
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet, gateway); //WiFi.config(ip, gateway, subnet,dns) dns=gateway
  }
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

  unsigned long connect_start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    conf_serie();                                                   // Pour paramètrage depuis liaison série
    if (millis() - connect_start > WIFI_TIMEOUT) {
      Serial.println();
      Serial.println("Erreur de connexion WIFI. Vérifier vos paramètres WIFI");
      Serial.print("Dépassement du temps de connexion: ");
      Serial.println(WIFI_TIMEOUT);
      Serial.println("Mode AP");                                // Mise en route du mode AP
      WiFi.disconnect(true);
      WiFi.mode(WIFI_AP);
      String ssid = "MyLED" + String(ESP.getChipId());
      String password = "MyLED" + String(ESP.getChipId());
      WiFi.softAP(ssid.c_str(), password.c_str());
      return;
    }
  }
  Serial.println("");
  Serial.println("WiFi est connecté");
  Serial.print("Addresse IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
// Vérification du WIFI
void wifi_verif() {
  unsigned long now = millis();
  if (now - last_wifi_check_time > WIFI_TIMEOUT) {
    Serial.print("Vérification du WiFi... ");
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Erreur de connexion WIFI ...");
      Serial.println("Taper wifi pour modifier les paramètres du WIFI");
      if (ReseauOut > WIFI_RESET) raz();
      ReseauOut++;
    } else {
      Serial.println("OK");
      Serial.printf("Etat du Wifi  %d\n", WiFi.isConnected());
      Serial.printf("Hostname est %s\n", WiFi.hostname().c_str());
      Serial.print("Addresse IP : "); Serial.println(WiFi.localIP());
      Serial.print("Passerelle IP: "); Serial.println(WiFi.gatewayIP());
      Serial.print("Masque sous réseau: "); Serial.println(WiFi.subnetMask());
      Serial.print("Adresse IP DNS: "); Serial.println(WiFi.dnsIP());
      if (Ping.ping(WiFi.gatewayIP())) {
        Serial.println("Ping Passerelle OK :)");
        ReseauOut = 0;
      } else {
        Serial.println("Ping Passerelle erreur :(");
        if (ReseauOut > WIFI_RESET) raz();
        ReseauOut++;
      }
    }
    last_wifi_check_time = now;
  }
}

// Lecture des paramètres WIFI si a=1 => SSID si a=2 => mot de passe
String LectureWifiEeprom(int a)
{
  // Lecture du SSID
  String ssid = "";
  for (int i = 0; i < 32; ++i)
  {
    ssid += char(EEPROM.read(i + ADRESS_WIFI));
  }
  // Lecture du mot de passe
  String password = "";
  for (int i = 32; i < 96; ++i)
  {
    password += char(EEPROM.read(i + ADRESS_WIFI));
  }
  if (a == 1) return String(ssid);
  if (a == 2) return String(password);
}

// conversion IP chaine en en valeur numérique
int IPnum(String valeur, int a) {
  int str_len = valeur.length() + 1;
  char *str = (char *)"255.255.255.255";

  valeur.toCharArray(str, str_len); // Transfert chaîne dans tableau de caractère
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

// Ecriture des paramètres WIFI
void EcritureWifiEeprom(String ssid, String password) {
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
