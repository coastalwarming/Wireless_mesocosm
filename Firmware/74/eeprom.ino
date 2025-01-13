void Update_eeprom() {
  char log_1[80];
  preferences.begin("Mesocosmo", false);

  //Wifi_present
  if (preferences.getInt("Wifi_present", 0) != Wifi_present) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Wifi_present was diferent in SD than in EEPROM, now is:%i.\n"), Wifi_present); Serial.print(log_1);
    preferences.putInt("Wifi_present", Wifi_present);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Wifi_present was the same in SD as EEPROM:%i.\n"), Wifi_present); Serial.print(log_1);
  }
  //ssid
  if (!(String(ssid).equals(preferences.getString("ssid", "")))) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ssid was diferent in SD than in EEPROM, now is:%s.\n"), ssid); Serial.print(log_1);
    preferences.putString("ssid", String(ssid));
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ssid was the same in SD as EEPROM:%s.\n"), ssid); Serial.print(log_1);
  }
  //password
  if (!(String(password).equals(preferences.getString("password", "")))) {
    snprintf_P(log_1, sizeof(log_1), PSTR("password was diferent in SD than in EEPROM, now is:%s.\n"), password); Serial.print(log_1);
    preferences.putString("password", String(password));
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("password was the same in SD as EEPROM:%s.\n"), password); Serial.print(log_1);
  }
  //BLYNK_TEMPLATE_ID_
  if (!(String(BLYNK_TEMPLATE_ID_).equals(preferences.getString("BLYNK_ID_", "")))) {
    snprintf_P(log_1, sizeof(log_1), PSTR("BLYNK_TEMPLATE_ID was diferent in SD than in EEPROM, now is:%s.\n"), BLYNK_TEMPLATE_ID_); Serial.print(log_1);
    preferences.putString("BLYNK_ID_", String(BLYNK_TEMPLATE_ID_));
  } else     {
    snprintf_P(log_1, sizeof(log_1), PSTR("BLYNK_TEMPLATE_ID was the same in SD as EEPROM:%s.\n"), BLYNK_TEMPLATE_ID_); Serial.print(log_1);
  }
  //BLYNK_DEVICE_NAME_
  if (!(String(BLYNK_DEVICE_NAME_).equals(preferences.getString("BLYNK_NAME_", "")))) {
    snprintf_P(log_1, sizeof(log_1), PSTR("BLYNK_DEVICE_NAME was diferent in SD than in EEPROM, now is:%s.\n"), BLYNK_DEVICE_NAME_); Serial.print(log_1);
    preferences.putString("BLYNK_NAME_", String(BLYNK_DEVICE_NAME_));
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("BLYNK_DEVICE_NAME was the same in SD as EEPROM:%s.\n"), BLYNK_DEVICE_NAME_); Serial.print(log_1);
  }
  //auth
  if (!(String(auth).equals(preferences.getString("auth", "")))) {
    snprintf_P(log_1, sizeof(log_1), PSTR("auth was diferent in SD than in EEPROM, now is:%s.\n"), auth); Serial.print(log_1);
    preferences.putString("auth", String(auth));
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("auth was the same in SD as EEPROM:%s.\n"), auth); Serial.print(log_1);
  }
  //Role
  /*
    if (preferences.getInt("Role", 0) != Myself.Role) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Role was diferent in SD than in EEPROM, now is:%i.\n"),Myself.Role); Serial.print(log_1);
    preferences.putInt("Role", Myself.Role);
    } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Role was the same in SD as EEPROM:%i.\n"),Myself.Role); Serial.print(log_1);
    }*/

  char R = preferences.getChar("Role", 0);
  if ( R != Myself.Role) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Role was diferent in SD than in EEPROM, now is:%c.\n"), Myself.Role); Serial.print(log_1);
    preferences.putChar("Role", Myself.Role);

  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Role was the same in SD as EEPROM:%c.\n"), Myself.Role); Serial.print(log_1);
  }
  //ID
  if (preferences.getInt("ID", 0) != Myself.ID) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ID was diferent in SD than in EEPROM, now is:%i.\n"), Myself.ID); Serial.print(log_1);
    preferences.putInt("ID", Myself.ID);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ID was the same in SD as EEPROM:%i.\n"), Myself.ID); Serial.print(log_1);
  }

  //Targeted_Master
  if (preferences.getInt("Targeted_Master", 0) != Targeted_Master) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Targeted_Master was diferent in SD than in EEPROM, now is:%i.\n"), Targeted_Master); Serial.print(log_1);
    preferences.putInt("Targeted_Master", Targeted_Master);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Targeted_Master was the same in SD as EEPROM:%i.\n"), Targeted_Master); Serial.print(log_1);
  }
  //Slave_Heaters_size
  if (preferences.getInt("Heaters_size", 0) != Slave_Heaters_size) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Slave_Heaters_size was diferent in SD than in EEPROM, now is:%i.\n"), Slave_Heaters_size); Serial.print(log_1);
    preferences.putInt("Heaters_size", Slave_Heaters_size);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Slave_Heaters_size was the same in SD as EEPROM:%i.\n"), Slave_Heaters_size); Serial.print(log_1);
  }

  //Viewers_size
  if (preferences.getInt("Viewers_size", 0) != Viewers_size) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Viewers_size was diferent in SD than in EEPROM, now is:%i.\n"), Viewers_size); Serial.print(log_1);
    preferences.putInt("Viewers_size", Viewers_size);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Viewers_size was the same in SD as EEPROM:%i.\n"), Viewers_size); Serial.print(log_1);
  }
  //Temp_offset
  if (preferences.getFloat("Temp_offset", 0) != Temp_offset) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Temp_offset was diferent in SD than in EEPROM, now is:%f.\n"), Temp_offset); Serial.print(log_1);
    preferences.putFloat("Temp_offset", Temp_offset);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Temp_offset was the same in SD as EEPROM:%f.\n"), Temp_offset); Serial.print(log_1);
  }
  //P_Temp_ripple
  if (preferences.getFloat("P_Temp_ripple", 0) != P_Temp_ripple) {
    snprintf_P(log_1, sizeof(log_1), PSTR("P_Temp_ripple was diferent in SD than in EEPROM, now is:%f.\n"), P_Temp_ripple); Serial.print(log_1);
    preferences.putFloat("P_Temp_ripple", P_Temp_ripple);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("P_Temp_ripple was the same in SD as EEPROM:%f.\n"), P_Temp_ripple); Serial.print(log_1);
  }
  //N_Temp_ripple
  if (preferences.getFloat("N_Temp_ripple", 0) != N_Temp_ripple) {
    snprintf_P(log_1, sizeof(log_1), PSTR("N_Temp_ripple was diferent in SD than in EEPROM, now is:%f.\n"), N_Temp_ripple); Serial.print(log_1);
    preferences.putFloat("N_Temp_ripple", N_Temp_ripple);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("N_Temp_ripple was the same in SD as EEPROM:%f.\n"), N_Temp_ripple); Serial.print(log_1);
  }
  //Log_period_minutes
  if (preferences.getInt("Log_period", 0) != Log_period_minutes) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Log_period_minutes was diferent in SD than in EEPROM, now is:%if.\n"), Log_period_minutes); Serial.print(log_1);
    preferences.putInt("Log_period", Log_period_minutes);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Log_period_minutes was the same in SD as EEPROM:%i.\n"), Log_period_minutes); Serial.print(log_1);
  }
  //Temp_period_seconds
  if (preferences.getULong("Temp_period", 0) != Temp_period_seconds) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Temp_period_seconds was diferent in SD than in EEPROM, now is:%li.\n"), Temp_period_seconds); Serial.print(log_1);
    preferences.putULong("Temp_period", Temp_period_seconds);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Temp_period_seconds was the same in SD as EEPROM:%li.\n"), Temp_period_seconds); Serial.print(log_1);
  }
    //Online_pin
  if (preferences.getInt("VP", 0) != Myself.VP) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Myself.VP was diferent in SD than in EEPROM, now is:%i.\n"), Myself.VP); Serial.print(log_1);
    preferences.putInt("VP", Myself.VP);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Myself.VP was the same in SD as EEPROM:%i.\n"), Myself.VP); Serial.print(log_1);
  }

  preferences.end();
}

void Read_from_eeprom() {
  char log_1[50];

  snprintf_P(log_1, sizeof(log_1), PSTR("Read settings from eeprom.\n")); Serial.print(log_1);
  preferences.begin("Mesocosmo", true);//(namespace,Read only =true, Read-write=false).

  //Wifi_present
  Wifi_present = preferences.getInt("Wifi_present", 0);
  Serial.print("Wifi_present:"); Serial.println(Wifi_present);

  //ssid
  myString_ssid = preferences.getString ("ssid", "");
  ssid = myString_ssid.c_str();
  snprintf_P(log_1, sizeof(log_1), PSTR("ssid:%s\n"), ssid); Serial.print(log_1);

  //password
  myString_password = preferences.getString ("password", "");
  password = myString_password.c_str();
  Serial.print("password:"); Serial.println(password);

  //BLYNK_TEMPLATE_ID
  (preferences.getString("BLYNK_ID_", "")).toCharArray(BLYNK_TEMPLATE_ID_, sizeof(BLYNK_TEMPLATE_ID_));
  Serial.print("BLYNK_ID:"); Serial.println(BLYNK_TEMPLATE_ID_);

  //BLYNK_DEVICE_NAME_
  (preferences.getString("BLYNK_NAME_", "")).toCharArray(BLYNK_DEVICE_NAME_, sizeof(BLYNK_DEVICE_NAME_));
  Serial.print("BLYNK_NAME_:"); Serial.println(BLYNK_DEVICE_NAME_);

  //auth
  (preferences.getString("auth", "")).toCharArray(auth, sizeof(auth));
  Serial.print("auth:"); Serial.println(auth);

  //Role
  /*
    Myself.Role = preferences.getInt("Role", 0);
    snprintf_P(log_1, sizeof(log_1), PSTR("Myself.Role:%i\n"), Myself.Role); Serial.print(log_1);
  */
  Myself.Role = preferences.getChar("Role", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Myself.Role:%c\n"), Myself.Role); Serial.print(log_1);
  //ID
  Myself.ID = preferences.getInt("ID", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Myself.ID:%i\n"), Myself.ID); Serial.print(log_1);

  //Targeted_Master
  Targeted_Master = preferences.getInt("Targeted_Master", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Targeted_Master:%i\n"), Targeted_Master); Serial.print(log_1);

  //Slave_Heaters_size
  Slave_Heaters_size = preferences.getInt("Heaters_size", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Slave_Heaters_size:%i\n"), Slave_Heaters_size); Serial.print(log_1);

  //Viewers_size
  Viewers_size = preferences.getInt("Viewers_size", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Viewers_size:%i\n"), Viewers_size); Serial.print(log_1);

  //Temp_offset
  Temp_offset = preferences.getFloat("Temp_offset", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Temp_offset:%f\n"), Temp_offset); Serial.print(log_1);

  //P_Temp_ripple
  P_Temp_ripple = preferences.getFloat("P_Temp_ripple", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("P_Temp_ripple:%f\n"), P_Temp_ripple); Serial.print(log_1);

  //N_Temp_ripple
  N_Temp_ripple = preferences.getFloat("N_Temp_ripple", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("N_Temp_ripple:%f\n"), N_Temp_ripple); Serial.print(log_1);

  //Log_period_minutes
  Log_period_minutes = preferences.getInt("Log_period", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Log_period_minutes:%i\n"), Log_period_minutes); Serial.print(log_1);

  //Temp_period_seconds
  Temp_period_seconds = preferences.getULong("Temp_period", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Temp_period_seconds:%li\n"), Temp_period_seconds); Serial.print(log_1);
  //Serial.print("Temp_period_seconds:"); Serial.println(Temp_period_seconds);

  //Online_pin
  Myself.VP = preferences.getInt("VP", 0);
  snprintf_P(log_1, sizeof(log_1), PSTR("Myself.VP:%i\n"), Myself.VP); Serial.print(log_1);


  preferences.end();
}
