// Configuration des paramètres via la liaison série
void conf_serie() {
  String WIFI_SSID;
  String WIFI_PASSWORD;
  String valeur;

  // Lecture port série
  while (Serial.available()) {
    valeur = Serial.readString();                            // Lecture des données sur la liaison série
    if (valeur == "raz") {
      raz();
    }
    if (valeur == "iboot") {
      Info_reboot();
    }
    if (valeur == "init") {
      AlarmInit();
    }
    if (valeur == "eeprom") {
      InitEeprom(1);
    }
     if (valeur == "minuteur") {
       Alarm.enable(AlarmeMin);
    }

    if (valeur == "led") {
      Serial.println("Entrer le nombres de LED");
      while (Serial.available() == 0) {                    //Attendre la saisie
      }
      valeur = Serial.readString();
      EEPROMWritelong(ADRESS_NLED, valeur.toInt(), 2);                 //  valeur nombres de LED
      Serial.println();
      Serial.print("Nombres de Led: ");
      Serial.printf("%d", EEPROMReadlong(ADRESS_NLED, 2));
      EEPROM.commit();
      raz();                                          // Reset du module ESP01
    }
    if (valeur == "alarme") {
      Serial.println("Voulez vous effacer les alarmes? Taper oui");
      while (Serial.available() == 0) {                    //Attendre la saisie
      }
      valeur = Serial.readString();
      if (valeur == "oui") {
        Serial.println();
        Serial.print("Alarmes effacer: ");
        for (int i = 0; i < NB_ALARME; i++) {
          EEPROM.write((ADRESS_AL0 + ADRESS_ALB * i), 255);
        }
        EEPROM.commit();
      }
    }
    if (valeur == "alexa") {
      Serial.println("Modification du nom du périphérique");
      Serial.println("Entrer le Nom du périphérique: ");          //Information de la saisie
      while (Serial.available() == 0) {                    //Attendre la saisie
      }
      Serial.print("Nom Alexa: ");
      //Serial.println(Serial.readString());
      EcritureStringEeprom(Serial.readString(),ADRESS_NOM_ALEXA,32);
      Serial.println(LectureStringEeprom(ADRESS_NOM_ALEXA,32));
      
    }
    if (valeur == "wifi") {
      Serial.println("Modification des paramètres wifi");
      Serial.println("Entrer le SSID du wifi: ");          //Information de la saisie
      while (Serial.available() == 0) {                    //Attendre la saisie
      }
      WIFI_SSID = Serial.readString();                    //Lecture et enregistrement dans la variable
      Serial.println("Entrer le mot de passe du wifi: "); //Information de la saisie
      while (Serial.available() == 0) {                   //Attendre la saisie
      }
      WIFI_PASSWORD = Serial.readString();                //Lecture et enregistrement dans la variable
      EcritureWifiEeprom(WIFI_SSID, WIFI_PASSWORD);
      Serial.println("DHCP=0 ou IP FIXE=1?: ");           //Information de la saisie
      while (Serial.available() == 0) {                   //Attendre la saisie
      }
      valeur = Serial.readString();                       //Lecture et enregistrement dans la variable
      if (valeur == "0") {
        EEPROM.write(ADRESS_RESEAU, 0);                  //  DHCP valeur à 0
      }
      else
      {
        EEPROM.write(ADRESS_RESEAU, 1);                        // IP statique à 1
        Serial.println("Adresse de la passerelle: ");          //Information de la saisie
        while (Serial.available() == 0) {                      //Attendre la saisie
        }
        valeur = Serial.readString();                         //Lecture et enregistrement dans la variable
        for (int i = 0; i < 4; ++i)
        {
          EEPROM.write(i + ADRESS_RESEAU + 1, IPnum(valeur, i));
        }
        Serial.println("IP Statique : ");                       //Information de la saisie
        while (Serial.available() == 0) {                      //Attendre la saisie
        }
        valeur = Serial.readString();                         //Lecture et enregistrement dans la variable
        EEPROM.write(ADRESS_RESEAU + 5, valeur.toInt());
      }
      Serial.println("Vérification des paramètres de sauvegarde: "); //Information
      Serial.print("SSID: ");
      Serial.println(LectureWifiEeprom(1));
      Serial.print("Mot de passe: ");
      Serial.println(LectureWifiEeprom(2));
      Serial.print("Adresse IP:");
      if (EEPROM.read(ADRESS_RESEAU) == 1) {
        Serial.println("");
        Serial.print("Adresse IP Passerelle:");
        for (int i = 1; i < 5; ++i)
        {
          Serial.printf("%d", EEPROM.read(ADRESS_RESEAU + i));
          Serial.print(".");
        }
        Serial.println("");
        Serial.print("Fin de l'adresse IP Module:");
        Serial.printf("%d", EEPROM.read(ADRESS_RESEAU + 5));
      }
      else {
        Serial.print("DCHP");
      }
      EEPROM.commit();                                // Ecriture de la RAM vers EEPROM
      raz();                                          // Rest du module ESP01
    }
  }
}
