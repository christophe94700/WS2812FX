
void InitAlexa() {

  espalexa.addDevice(LectureStringEeprom(ADRESS_NOM_ALEXA, 32), LampeChange); //Définition, etat arrêt
  Serial.print("Nom Alexa: ");
  Serial.println(LectureStringEeprom(ADRESS_NOM_ALEXA, 32));
  espalexa.begin(&server);
}
//Retour Fonction Alexa
void LampeChange(uint8_t brightness, uint32_t rgb) {
  Serial.print("Changement d'état: ");
  Serial.println(LectureStringEeprom(ADRESS_NOM_ALEXA, 32));

  if (brightness) {
    Serial.print("Marche, Niveau: ");
    Serial.println(brightness);
    ws2812fx->setColor(EEPROMReadlong(ADRESS_LED_COL_ALEXA, 4));     // Lecture valeur Mémoire couleur
    ws2812fx->setBrightness(brightness);                     // Lecture valeur de la luminosité
    ws2812fx->start();
    EEPROM.write(ADRESS_ON_OFF, 1);
    EEPROM.commit();
  }
  else  {
    Serial.println("Arrêt");
    ws2812fx->stop();
    EEPROM.write(ADRESS_ON_OFF, 0);
    EEPROM.commit();
  }
  Serial.print(", Rouge: ");
  Serial.print((rgb >> 16) & 0xFF); // obtenir la composante rouge
  Serial.print(", Vert: ");
  Serial.print((rgb >>  8) & 0xFF); // obtenir la composante verte
  Serial.print(", Bleu: ");
  Serial.println(rgb & 0xFF); // obtenir la composante bleue
}
