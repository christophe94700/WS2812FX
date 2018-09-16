void Date_Heure() {
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
  //if (timeClient.getFormattedTime() == "04:00:00") raz(); // Raz à 4h00 du matin
  setSyncProvider(getNtpTime);
}
/*-------- NTP code ----------*/
time_t getNtpTime() {
  return (timeClient.getEpochTime());
}

