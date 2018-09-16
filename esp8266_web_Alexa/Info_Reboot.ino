void Info_reboot() {
  // Type de reboot
  Serial.printf(S_REASON_FOR_RESTART, ESP.getResetInfoPtr()->reason, ESP.getResetReason().c_str());
  switch (ESP.getResetInfoPtr()->reason) {
    case REASON_DEFAULT_RST:
      Serial.println(S_POWER_ON);
      break;
    case REASON_WDT_RST:
      Serial.println(S_HARDWARE_WD);
      break;
    case REASON_EXCEPTION_RST:
      Serial.printf(S_EXCEPTION_OCCURED, ESP.getResetInfoPtr()->exccause);
      break;
    case REASON_SOFT_WDT_RST:
      Serial.println(S_SOFTWARE_WD);
      break;
    case REASON_SOFT_RESTART:
      Serial.println(S_RESTART_OR_RESET);
      break;
    case REASON_DEEP_SLEEP_AWAKE:
      Serial.println(S_ESP_WOKE);
      break;
    case REASON_EXT_SYS_RST:
      Serial.println(S_EXTERNAL_RESET);
      break;
    default:
      Serial.println(S_UNKNOWN_REASON);
      break;
  }
}
void Info_ESP() {
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
}

