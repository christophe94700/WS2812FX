void InitOTA() {
  ArduinoOTA.setHostname(("MyLED" + String(ESP.getChipId())).c_str());          // Nom du module pour mise à jour et pour le mDNS
  ArduinoOTA.setPassword((LectureStringEeprom(ADRESS_PASSWORD, 32)).c_str());  // Mot de passe pour mise à jour
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "de l'esquisse";
    } else { // U_SPIFFS
      type = "des data";
    }
    Serial.println("Mise à jour " + type);
  });
  ArduinoOTA.onEnd([]() {                                                      // Effacement EEPROM après mise à jour
    InitEeprom(1);
    //SPIFFS.begin();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erreur[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Échoué");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Démarrage a échoué");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Echec de connexion");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Échec de la réception");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Echec à la fin");
    }
  });
  ArduinoOTA.begin();
}
