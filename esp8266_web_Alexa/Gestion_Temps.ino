void Date_Heure() {

// Passage heure d'été hivers
time_t t, utc;
TimeChangeRule *tcr;

// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

utc = now();  //current time from the Time Library
int8_t tmp = EEPROM.read(ADRESS_GMT);
if (tmp==13) t = CE.toLocal(utc, &tcr);
if (tmp==14) t = UK.toLocal(utc, &tcr);
if (tmp > -12 && tmp < 13) timeClient.setTimeOffset(3600 * tmp); else timeClient.setTimeOffset((t-utc));// Initialisation du fuseau

// Mise en forme de la date et de l'heure
  String temp = "Date: ";
  timeClient.update();
  if (day() < 10) {
    temp += "0";
    temp += day();
  } else temp += day();
  temp += "/";
  if (month() < 10) {
    temp += "0";
    temp += month();
  } else temp += month();
  temp += "/";
  temp += year();
  temp += " Heure: ";
 DateHeure = temp + timeClient.getFormattedTime();
  
  if (year()>2037) raz(); // Bug date
  setSyncProvider(getNtpTime);
}
/*-------- NTP code ----------*/
time_t getNtpTime() {
  return (timeClient.getEpochTime());
}
