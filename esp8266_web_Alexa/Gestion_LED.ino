
// Initialisation du bandeau LED
void initLed() {
  ws2812fx.setCustomMode(myCustomEffect);             // Initialisation de l'effect custom
  Serial.print("Nombres de Led: ");
  Serial.printf("%d", EEPROMReadlong(ADRESS_NLED, 2));
  Serial.println();
  Serial.println("Initialisation de WS2812FX");
  if (EEPROM.read(ADRESS_ON_OFF) == 0) ws2812fx.stop(); // Arrêt du bandeau
  if (EEPROM.read(ADRESS_ON_OFF) == 1) ws2812fx.start(); // Marche du bandeau
  ws2812fx.setLength(EEPROMReadlong(ADRESS_NLED, 2));
  ws2812fx.init();
  ws2812fx.setBrightness(EEPROM.read(ADRESS_LED_LUM));      // Lecture valeur Mémoire luminosité
  ws2812fx.setSpeed(EEPROMReadlong(ADRESS_LED_VIT, 2));     // Lecture valeur Mémoire vitesse
  ws2812fx.setMode(EEPROM.read(ADRESS_LED_MOD));            // Lecture valeur Mémoire mode
  ws2812fx.setColor(EEPROMReadlong(ADRESS_LED_COL, 4));     // Lecture valeur Mémoire couleur
  MinuteurStop = (EEPROMReadlong(ADRESS_MINUTEUR, 4)); // Lecture valeur minuteur dams Mémoire.
}

// Configuration de l'effet custom
uint16_t myCustomEffect(void) {                           //
  // parameters: index, start, stop, mode, couleur, vitesse, inverse
  uint16_t LED_COUNT = EEPROMReadlong(ADRESS_NLED, 2);    // Nombres de LED

  for (int i = 0; i < 3; i++) {
    ws2812fx.setSegment(1 + i, i * LED_COUNT / 3, (1 + i)*LED_COUNT / 3 - 1,
                        EEPROM.read(ADRESS_CUSTOM_S1 + 1 + ADRESS_CUSTOMB * (i)),
                        EEPROMReadlong((ADRESS_CUSTOM_S1 + 2 + ADRESS_CUSTOMB * (i)), 4),
                        EEPROMReadlong((ADRESS_CUSTOM_S1 + 6 + ADRESS_CUSTOMB * (i)), 2),
                        EEPROM.read((ADRESS_CUSTOM_S1 + 8 + ADRESS_CUSTOMB * (i))));
  }
}

// Gestion Custom Effet

void Custom_Effet(String ParaAl) {
  int NumSegment = 0, NumSens = 0, NumMode = 0, NumVitesse = 0;
  uint32_t NumCouleur = 0;
  // Lecture des paramètres
  StringSplitter *splitter = new StringSplitter(ParaAl, ',', 5);  // Initialisation de la classe avec 5 paramètres
  int itemCount = splitter->getItemCount();
  String para[itemCount];
  Serial.print("Paramètres Custom Effet reçu= ");
  for (int i = 0; i < itemCount; i++) {
    String item = splitter->getItemAtIndex(i);
    Serial.print("Valeur" + String(i) + ": " + (item) + " ");
    para[i] = item;
  }
  Serial.println("Segment: " + (para[0]) + " ");
  Serial.println("Paramètres Custom Effet requete " + (para[0]) + ": " + ParaAl);

  NumSegment = (para[0]).toInt();                                // Paramètre N° Segement de 0 à 2

  if ((para[1]).toInt() < 57 && (para[1]).toInt() >= 0) {        // Paramètre Mode 0 à 56
    NumMode = (para[1]).toInt();
  } else return; //

  uint32_t tmp = strtol(para[2].c_str(), NULL, 16);
  if (tmp >= 0x000000 && tmp <= 0xFFFFFF) {                      // Paramètre Couleur de 0 à FFFFFF
    NumCouleur = tmp;
  } else return; //
  if ((para[3]).toInt() < 65000 && (para[3]).toInt() >= 0) {      // Paramètre Vitesse 0 à 65000
    NumVitesse = (para[3]).toInt();
  } else return; //

  if ((para[4]).toInt() < 2 && (para[4]).toInt() >= 0) {         // Paramètre Sens 0 à 1
    NumSens = (para[4]).toInt();
  } else return; //


  // Sauvegarde paramètres EEPROM
  EEPROM.write((ADRESS_CUSTOM_S1 + ADRESS_CUSTOMB * (NumSegment)), NumSegment);           // Paramètre 0 Segment
  EEPROM.write((ADRESS_CUSTOM_S1 + 1 + ADRESS_CUSTOMB * (NumSegment)), NumMode);          // Paramètre 1 Mode
  EEPROMWritelong(ADRESS_CUSTOM_S1 + 2 + ADRESS_CUSTOMB * (NumSegment), NumCouleur, 4);   // Paramètre 2 de la couleur
  EEPROMWritelong(ADRESS_CUSTOM_S1 + 6 + ADRESS_CUSTOMB * (NumSegment), NumVitesse, 2);   // Paramètre 3 de la vitesse
  EEPROM.write(ADRESS_CUSTOM_S1 + 8 + ADRESS_CUSTOMB * (NumSegment), NumSens);            // Paramètre 4 Sens
  EEPROM.commit();                                                                        // Ecriture de la RAM vers EEPROM
  initLed();                                                                              // Initialisation des paramètres LED
}

