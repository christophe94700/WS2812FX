
/*Fonction pour ecrire dans l'EEPROM des valeurs en 32 bits (4 mots) ou 16 bits (2 mots)
  nbit=2 en 16 bits nbit=4 en 32 bits
*/
void EEPROMWritelong(int address, long value, int nbit)
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
  if (nbit == 4) {
    EEPROM.write(address + 2, two);
    EEPROM.write(address + 3, one);
  }
  EEPROM.commit();
}

/*Fonction pour lire dans l'EEPROM des valeurs en 32 bits (4 mots) ou 16 bits (2 mots)
   nbit=2 en 16 bits nbit=4 en 32 bits
*/
long EEPROMReadlong(long address, int nbit)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  if (nbit == 4) return long((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
  if (nbit == 2) return uint16_t(((four << 0) & 0xFF) + ((three << 8) & 0xFFFF));
}
// Effacement de la zone mémoire paramètres WIFI avant écriture
void EffaceWifiEeprom() {
  for (int i = ADRESS_WIFI ; i < ADRESS_WIFI + 96; ++i) {
    EEPROM.write(i, 0);
  }
  //EEPROM.commit();
}

// Ecriture String EEPROM
void EcritureStringEeprom(String valeur, int adresse, int taille) {
  // Effacement de la zone mémoire
  for (int i = 0 ; i < taille; ++i) {
    EEPROM.write(i + adresse, 0);
  }
  EEPROM.commit();
  //Ecriture
  for (int i = 0; i < valeur.length(); ++i)
  {
    EEPROM.write(i + adresse, valeur[i]);
  }
  EEPROM.commit();
}

// Lecture String EEPROM
String LectureStringEeprom(int adresse, int taille)
{
  // Lecture de la valeur
  String valeur = "";
  for (int i = 0; i < taille; ++i)
  {
    if ((EEPROM.read(i + adresse)) == 0) return String(valeur);
    valeur += char(EEPROM.read(i + adresse));
  }
  return String(valeur);
}
// Initialisation des valeurs dans EEPROM
void InitEeprom(bool Force) {
  int8_t tmp = 0;
  tmp = EEPROM.read(ADRESS_EEPROM_INIT);
  if ((tmp != 20) or (Force == 1)) {
    // Efface EEPROM
    for (int i = 0; i < EEPROM.length(); ++i) {
      EEPROM.write(i, 0);
    }
    EEPROM.write(ADRESS_LED_LUM, DEF_LED_LUM);          // Sauvegarde de la luminosité
    EEPROM.write(ADRESS_LED_MOD, DEF_LED_MOD);          // Sauvegarde du mode
    EEPROMWritelong(ADRESS_LED_VIT, DEF_LED_VIT, 2);    // Sauvegarde de la vitesse
    EEPROMWritelong(ADRESS_LED_COL, DEF_LED_COL, 4);    // Sauvegarde de la couleur
    EEPROMWritelong(ADRESS_NLED, DEF_NLED, 2);          // Sauvegarde Nombre de LED
    EEPROM.write(ADRESS_ON_OFF, DEF_ON_OFF);            // Sauvegarde LED en marche
    EEPROMWritelong(ADRESS_MINUTEUR, DEF_MINUTEUR, 2);  // Sauvegarde valeur minuteur en seconde
    EEPROM.write(ADRESS_GMT, DEF_GMT);                  // Sauvegarde GMT
    // Defaut paramètres WIFI
    for (int i = ADRESS_WIFI; i < ADRESS_NLED; ++i) {
      EEPROM.write(i, 0);
    }
    // Defaut paramètres Custom Effect
    for (int i = ADRESS_CUSTOM_S1; i < ADRESS_CUSTOM_S3+9; ++i) {
      EEPROM.write(i, 0);
    }
    // Defaut paramètres Alarmes
    for (int i = 0; i < NB_ALARME; i++) {
      EEPROM.write((ADRESS_AL0 + ADRESS_ALB * i), 255);
    }
    EcritureStringEeprom("Alexa"+String(ESP.getChipId()), ADRESS_NOM_ALEXA, 32);  // Defaut paramètres Alexa
    EcritureStringEeprom("admin", ADRESS_PASSWORD, 32);  // Defaut mots de passe mise à jour OTA
    EEPROM.write(ADRESS_EEPROM_INIT, 20);
    EEPROM.write(ADRESS_PIN_LED, 2);             // PIN LED PAR DEFAUT 2
    EEPROM.commit();
  }
  Serial.println("+++ Valeur par défaut EEPROM +++");        //Saut de ligne
}
