
// Gestion des alarmes

void Timer1S() {
  TimerON = 1;
  if (MinuteurStop <= 0) {
    ws2812fx.stop();
    TimerON = 0;
    Alarm.disable(AlarmeMin);
    MinuteurStop = (EEPROMReadlong(ADRESS_MINUTEUR, 4)); // Lecture valeur minuteur dams Mémoire.
  } else {
    --MinuteurStop;
  }

  if ((MinuteurStop - 60) > 0) {
    AffMinuteur = "Minuteur temps restant: ";
    AffMinuteur += ((MinuteurStop / 60) + 1);
    AffMinuteur += " mn";
  }
  else {
    AffMinuteur = "Minuteur temps restant: ";
    AffMinuteur += MinuteurStop;
    AffMinuteur += " s";
  }
}

void Active_Alarme(String ParaAl) {
  int NumAlarme = 0, NumHeure = 0, NumMinute = 0, NumOnOff = 0, NumJour = 0, NumCouleur = 0, NumMode = 0, NumLumi = 0, NumTimer = 0;
  // Lecture des paramètres
  StringSplitter *splitter = new StringSplitter(ParaAl, ',', 9);  // Initialisation de la classe avec 9 paramètres
  int itemCount = splitter->getItemCount();
  String para[itemCount];
  Serial.print("Paramètres alarme reçu= ");
  for (int i = 0; i < itemCount; i++) {
    String item = splitter->getItemAtIndex(i);
    Serial.print("Valeur" + String(i) + ": " + (item) + " ");
    para[i] = item;
  }
  Serial.println("Alarme" + (para[0]) + " ");
  Serial.println("Paramètres Alarme requete " + (para[0]) + ": " + ParaAl);

  NumAlarme = (para[0]).toInt();                                 // Paramètre N° alarme de 1 à 6
  if ((para[1]).toInt() < 24 && (para[1]).toInt() >= 0) {        // Paramètre Heure de 0 à 23
    NumHeure = (para[1]).toInt();
  } else return; //
  if ((para[2]).toInt() < 60 && (para[2]).toInt() >= 0) {        // Paramètre Minute de 0 à 59
    NumMinute = (para[2]).toInt();
  } else return; //
  if ((para[3]).toInt() < 2 && (para[3]).toInt() >= 0) {         // Paramètre Marche=1 Arrêt=0
    NumOnOff = (para[3]).toInt();
  } else
  {
    EEPROM.write((ADRESS_AL0 + ADRESS_ALB * (NumAlarme)), 255);  // Paramètre 0 ID à 255 alarme libre
    EEPROM.commit();
    return; //
  }
  if ((para[4]).toInt() < 10 && (para[4]).toInt() >= 0) {         // Paramètre Jour 0 à 9 Toujours=0
    NumJour = (para[4]).toInt();
  } else return; //
  if ((para[5]).toInt() < 9 && (para[5]).toInt() >= 0) {         // Paramètre Couleur 0 à 8
    NumCouleur = (para[5]).toInt();
  } else return; //
  if ((para[6]).toInt() < 57 && (para[6]).toInt() >= 0) {        // Paramètre Mode 0 à 56
    NumMode = (para[6]).toInt();
  } else return; //
  if ((para[7]).toInt() < 256) {                                  // Paramètre Luminosité 0 à 255
    NumLumi = (para[7]).toInt();
  } else return; //
  if ((para[8]).toInt() < 256 && (para[7]).toInt() >= 0) {        // Paramètre Timer 0 à 255 Si 0 Pas de timer
    NumTimer = (para[8]).toInt();
  } else return; //

  // Sauvegarde paramètres EEPROM
  EEPROM.write((ADRESS_AL0 + ADRESS_ALB * (NumAlarme)), (NumAlarme + 1));        // Paramètre 0 ID alarme +1
  EEPROM.write((ADRESS_AL0 + 5 + ADRESS_ALB * (NumAlarme)), NumHeure);           // Paramètre 5 Heure
  EEPROM.write((ADRESS_AL0 + 6 + ADRESS_ALB * (NumAlarme)), NumMinute);          // Paramètre 6 Minute
  EEPROM.write((ADRESS_AL0 + 7 + ADRESS_ALB * (NumAlarme)), NumOnOff);           // Paramètre 7 on/off;
  EEPROM.write((ADRESS_AL0 + 8 + ADRESS_ALB * (NumAlarme)), NumJour);            // Paramètre 8 Jour;
  EEPROM.write((ADRESS_AL0 + 9 + ADRESS_ALB * (NumAlarme)), NumCouleur);         // Paramètre 9 Couleur;
  EEPROM.write((ADRESS_AL0 + 10 + ADRESS_ALB * (NumAlarme)), NumMode);           // Paramètre 10 Mode;
  EEPROM.write((ADRESS_AL0 + 11 + ADRESS_ALB * (NumAlarme)), NumLumi);           // Paramètre 11 Luminosité;
  EEPROM.write((ADRESS_AL0 + 12 + ADRESS_ALB * (NumAlarme)), NumTimer);           // Paramètre 12 Timer;
  EEPROM.commit();                                                                // Ecriture de la RAM vers EEPROM
  AlarmInit();                                                                    // Initialisation des alarmes
}

void AlarmInit() {
  int ID = 0, NumAlarme = 0, NumHeure = 0, NumMinute = 0, NumOnOff = 0, NumJour = 0;
  // Initialisation minuterie
  for (int i = 0; i <= NB_ALARME; i++) {
    Alarm.free(i);                                        // Libération de l'alarme 0 à NB_ALARME
  }
  Serial.print("ID alarme Minuteur: ");
  AlarmeMin = (Alarm.timerRepeat(1, Timer1S));          // Timer toute les 60 secondes
  Serial.println(AlarmeMin);                            // ID de la minuterie
  Alarm.disable(AlarmeMin);                             // Arrêt de la minuterie après initialisation
  if (EEPROM.read(ADRESS_TIMER) == 1) {
    Alarm.enable(AlarmeMin);                             // Mise en marche apres raz
  }
  // Intialisation des alarmes
  for (int i = 0; i < NB_ALARME; i++) {                 // Intiatisation des alarmes
    ID = EEPROM.read(ADRESS_AL0 + ADRESS_ALB * i);
    NumHeure = EEPROM.read(ADRESS_AL0 + 5 + ADRESS_ALB * i);
    NumMinute = EEPROM.read(ADRESS_AL0 + 6 + ADRESS_ALB * i);
    NumOnOff = EEPROM.read(ADRESS_AL0 + 7 + ADRESS_ALB * i);
    NumJour = EEPROM.read(ADRESS_AL0 + 8 + ADRESS_ALB * i);
    if (ID != 255) {
      Alarm_Select(i, NumHeure, NumMinute, NumOnOff, NumJour);
      Serial.println("Alarme " + String (ID) + " : ID " + String (ID) + "= Active");
    }
    Serial.println("Paramètres Alarme " + String(ID) + " : " + EtatAl(i));
  }
  Serial.println("Valeur Temps Timer: " + String(MinuteurStop));

}

void Alarm_Select(int NumAlarme, int NumHeure, int NumMinute, int NumOnOff, int NumJour) {
  //dowSunday, dowMonday, dowTuesday, dowWednesday, dowThursday, dowFriday, dowSaturday
  switch (NumJour) {
    case 0:
      // Toujours
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 1:
      // Dimanche
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowSunday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowSunday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 2:
      // Lundi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowMonday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowMonday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 3:
      // Mardi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowTuesday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowTuesday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 4:
      // Mercredi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowWednesday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowWednesday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 5:
      // Jeudi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowThursday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowThursday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 6:
      // Vendredi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowFriday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowFriday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 7:
      // Samedi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowSaturday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowSaturday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      break;
    case 8:
      // Weekend
      if (NumOnOff == 1) {
        Alarme[NumAlarme] = Alarm.alarmRepeat(dowSaturday, NumHeure, NumMinute, 0, Alarm_On);
        EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
        Alarme[NumAlarme] = Alarm.alarmRepeat(dowSunday, NumHeure, NumMinute, 0, Alarm_On);
        EEPROM.write(ADRESS_AL0 + 1 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      }
      if (NumOnOff == 0) {
        Alarme[NumAlarme] = Alarm.alarmRepeat(dowSaturday, NumHeure, NumMinute, 0, Alarm_Off);
        EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
        Alarme[NumAlarme] = Alarm.alarmRepeat(dowSunday, NumHeure, NumMinute, 0, Alarm_Off);
        EEPROM.write(ADRESS_AL0 + 1 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      }
      break;
    case 9:
      // Semaine
      // Lundi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowMonday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowMonday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
      // Mardi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowTuesday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowTuesday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + 1 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      // Mercredi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowWednesday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowWednesday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + 2 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      // Jeudi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowThursday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowThursday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + 3 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      // Vendredi
      if (NumOnOff == 1) Alarme[NumAlarme] = Alarm.alarmRepeat(dowFriday, NumHeure, NumMinute, 0, Alarm_On);
      if (NumOnOff == 0) Alarme[NumAlarme] = Alarm.alarmRepeat(dowFriday, NumHeure, NumMinute, 0, Alarm_Off);
      EEPROM.write(ADRESS_AL0 + 4 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);      // Sauvegarde ID de l'alarme
      break;
    default:
      //Erreur Initialisation Alarmes
      Serial.println("Erreur Initialisation Alarme " + String (NumAlarme) + " :");
  }
  //EEPROM.write(ADRESS_AL0 + ADRESS_ALB * NumAlarme, Alarme[NumAlarme]);        // Sauvegarde ID de l'alarme
  //EEPROM.commit();                                                             // Ecriture de la RAM vers EEPROM
}

void Alarm_On() {
  // Allume le bandeau LED
  Serial.println("Alarme Enclechement ID :" + String(Alarm.getTriggeredAlarmId()));
  int ID = 0, NumCouleur = 0, NumMode = 0, NumLumi = 0, NumTimer = 0;
  for (int i = 0; i < NB_ALARME; i++) {
    for (int y = 0; y < 5; y++) {                                             // Scan des ID pour weekend ou semaine
      ID = EEPROM.read(ADRESS_AL0 + y + ADRESS_ALB * i);
      if (ID == Alarm.getTriggeredAlarmId()) {
        NumCouleur = EEPROM.read(ADRESS_AL0 + 9 + ADRESS_ALB * i);
        NumMode = EEPROM.read(ADRESS_AL0 + 10 + ADRESS_ALB * i);               // Lecture valeur Mémoire mode
        NumLumi = EEPROM.read(ADRESS_AL0 + 11 + ADRESS_ALB * i);               // Lecture valeur Mémoire Luminosité
        NumTimer = EEPROM.read(ADRESS_AL0 + 12 + ADRESS_ALB * i);               // Lecture valeur Mémoire Timer
        Serial.println("Mode alarme: " + String(NumMode));
        switch (NumCouleur) {
          case 0:
            ws2812fx.setColor(GREEN);                                             // Lecture valeur couleur verte
            break;
          case 1 :
            ws2812fx.setColor(BLUE);                                              // Lecture valeur couleur Bleue
            break;
          case 2 :
            ws2812fx.setColor(RED);                                              // Lecture valeur couleur Rouge
            break;
          case 3 :
            ws2812fx.setColor(MAGENTA);                                          // Lecture valeur couleur Fuschia
            break;
          case 4 :
            ws2812fx.setColor(PURPLE);                                          // Lecture valeur couleur Violette
            break;
          case 5 :
            ws2812fx.setColor(ORANGE);                                          // Lecture valeur couleur Orange
            break;
          case 6 :
            ws2812fx.setColor(CYAN);                                          // Lecture valeur couleur Cyan
            break;
          case 7 :
            ws2812fx.setColor(YELLOW);                                          // Lecture valeur couleur Jaune
            break;
          case 8 :
            ws2812fx.setColor(HOTWHITE);                                        // Lecture valeur couleur Blanc chaud
            break;
        }
        ws2812fx.setNumSegments(1);
        ws2812fx.setMode(NumMode);
        ws2812fx.setBrightness(NumLumi);
      }
    }
  }
  Alarm.disable(AlarmeMin);
  ws2812fx.start();
  EEPROM.write(ADRESS_ON_OFF, 1);         // Sauvegarde LED en marche
  EEPROM.commit();                        // Ecriture de la RAM vers EEPROM
  if (NumTimer > 0) {                     // Si timer enclechement du Timer
    MinuteurStop = NumTimer * 60;         // Conversion en second
    Alarm.enable(AlarmeMin);
  }


}
void Alarm_Off() {
  // Etient le badeau LED
  Serial.println("Alarme Enclechement ID : " + String(Alarm.getTriggeredAlarmId()));
  ws2812fx.stop();
  EEPROM.write(ADRESS_ON_OFF, 0);         // Sauvegarde LED Arrêt
  EEPROM.commit();                        // Ecriture de la RAM vers EEPROM
  initLed();                              // Initialisation des paramètres LED
}

String EtatAl(uint8_t NumAlarme) {
  String texte;
  if (EEPROM.read(ADRESS_AL0 + ADRESS_ALB * NumAlarme) == 255) {
    texte = "Libre";
    return texte;
  }
  texte = "ID: " + String(EEPROM.read(ADRESS_AL0 + ADRESS_ALB * NumAlarme))
          + " H: " + String(EEPROM.read(ADRESS_AL0 + 5 + ADRESS_ALB * NumAlarme))
          + " M: " + String(EEPROM.read(ADRESS_AL0 + 6 + ADRESS_ALB * NumAlarme));
  if ((EEPROM.read(ADRESS_AL0 + 7 + ADRESS_ALB * NumAlarme)) == 1) texte = texte + " On "; else texte = texte + " Off ";
  texte = texte + " Jour: " + String(EEPROM.read(ADRESS_AL0 + 8 + ADRESS_ALB * NumAlarme))
          + " Couleur: " + String(EEPROM.read(ADRESS_AL0 + 9 + ADRESS_ALB * NumAlarme))
          + " Mode: " + (ws2812fx.getModeName((EEPROM.read(ADRESS_AL0 + 10 + ADRESS_ALB * NumAlarme))))
          + " Puissance: " + String(EEPROM.read(ADRESS_AL0 + 11 + ADRESS_ALB * NumAlarme))
          + " Timer: " + String(EEPROM.read(ADRESS_AL0 + 12 + ADRESS_ALB * NumAlarme));
  return texte;
}

