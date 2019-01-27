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
  if (year()>2037) raz(); // Bug date
  setSyncProvider(getNtpTime);
}
/*-------- NTP code ----------*/
time_t getNtpTime() {
  return (timeClient.getEpochTime());
}
