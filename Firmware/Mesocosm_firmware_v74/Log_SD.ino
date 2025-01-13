bool SdExist(void) {
  //Serial.println("Creating test.txt file to detect the SD card");
  if (SD.open("/test.txt", "w"))
  {
    SD.remove("/test.txt");
    //Serial.println("Succesfuly created and removed.");
    //Serial.println("SD card detected");
    SdExist_ = true;
    return true;
  }
  else
  {
    //Serial.println("Error in creating file");
    //Serial.println("SD card not detected");
    SdExist_ = false;
    return false;
  }
}

bool InitSDCard() {
  // Initialize SD card
  char log_1[25];
  if (!SD.begin()) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Card Mount Failed.\n")); Serial.print(log_1);
    //Serial.println("Card Mount Failed.");
    return 0;
  } else {
    return 1;
    snprintf_P(log_1, sizeof(log_1), PSTR("Card Mount Successful.\n")); Serial.print(log_1);
    //Serial.println("Card Mount Successful.");
  }
}

bool Religar() {
  char log_1[80];
  SD.end();//Teste.
  InitSDCard();
  if (SdExist()) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Ja consegui ligar ao cartão.\n")); Serial.print(log_1);
    //Serial.println("Ja consegui ligar ao cartão.");
    return true;
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Ainda não consegui ligar ao cartão.\n")); Serial.print(log_1);
    //Serial.println("Ainda não consegui ligar ao cartão.");
    return false;
  }
}

void writeFile(fs::FS &fs,  char * path, const char * message, bool novo_file) {//const char * path
  char log_1[80];
  snprintf_P(log_1, sizeof(log_1), PSTR("Writing file: %s\n"), path); Serial.print(log_1);
  //Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Failed to open file for writing.\n")); Serial.print(log_1);
    //Serial.println("Failed to open file for writing.");
    return;
  }
  file = fs.open(path, FILE_WRITE);
  if (novo_file) {
    file.println("------------------------------,--------------------");
    file.println("www.electricblue.eu, Portugal");
    file.println("------------------------------,--------------------");
    file.printf("Mesocosm version,V%.2f\n", Version);
    file.println("------------------------------,--------------------");
    if (Myself.Role == 'V')file.printf("Role,Viewer\n");
    if (Myself.Role == 'S')file.printf("Role,Slave\n");
    if (Myself.Role == 'M')file.printf("Role,Master\n");
    file.printf("ID,%i\n", Myself.ID);
    file.println("------------------------------,--------------------");
  }

  if (Temp_offset_updated) {
    Serial.print("Temp_offset em writeFile:"); Serial.println(Temp_offset);
    Serial.printf("Updating Temp_offset no local:%d to %f .\n", local_offset, Temp_offset);
    file.seek(local_offset);
  }

  if (!file.print(message)) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Write failed.\n")); Serial.print(log_1);
    //Serial.println("Write failed.");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, char message[], bool paragrafo) {
  Wire.begin();//Teste2
  char log_1[80];
  //const char * message

  //Serial.print("path:"); Serial.println(path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Failed to open file for appending.\n")); Serial.print(log_1);
    //Serial.println("Failed to open file for appending.");
    return;
  }
  if (paragrafo) {
    if (!file.println(message)) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Append failed.\n")); Serial.print(log_1);
      //Serial.println("Append failed.");
    } //Serial.println("Appendln suseful.");
  } else {
    if (!file.print(message)) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Append failed.\n")); Serial.print(log_1);
      //Serial.println("Append failed.");
    } //Serial.println("Append suseful.");
  }

  file.close();
}

void Create_n_Log_data_to_SD() {

  char log_1[80];
  Ano = 0;
  mes = 0;
  dia = 0;
  hora = 0;
  minuto = 0;
  segundo = 0;
  Wire.begin();
  if (Outrtc_working) {
    //Serial.println("0 Outrtc_working=1");

    if (Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201 )) {
      Outrtc_adjusted = true;
      current_epoch = Outrtc.getEpoch();
      Inrtc.setTime(current_epoch);
      Ano = Inrtc.getYear();
      mes = Inrtc.getMonth() + 1;
      dia = Inrtc.getDay();
      hora = Inrtc.getHour(true);
      if (Prev_hora != hora) {
        if (hora == 12)NTP_updated = true;
        Resumo_sensores_Flag = true;
      }


      Prev_hora = hora;
      minuto = Inrtc.getMinute();
      segundo = Inrtc.getSecond();
      Serial.print("Outrtc is working. Got epochTime via Outrtc:"); Serial.println(current_epoch);
    } else {
      Outrtc_adjusted = false;
      if (!NTP_updated && Myself.Role == 'M' )NTP_updated = true;
      Ano = Inrtc.getYear();
      mes = Inrtc.getMonth() + 1;
      dia = Inrtc.getDay();
      hora = Inrtc.getHour(true);
      minuto = Inrtc.getMinute();
      segundo = Inrtc.getSecond();
      Serial.println("Outrtc.updateTime() failed.");
    }
  } else if (Outrtc.begin())Outrtc_working = true;


  //Criar ficheiro com nome do dia atual caso não exista:
  String lLog = "Data/";

  snprintf_P(File_name, sizeof(File_name), PSTR("/Data/%i_%i_%i__%c_%i.CSV"), Ano, mes, dia, Myself.Role, Myself.ID);
  //
  //snprintf_P(log_1, sizeof(log_1), PSTR("File name1:%s\n"), File_name); Serial.print(log_1);
  Serial.println("File name:"); Serial.println(File_name);
  //Opening or creating file:
  File file = SD.open(File_name);



  if (!file) {//Nao vejo o file mas estava ligado ao cartão?
    snprintf_P(log_1, sizeof(log_1), PSTR("File doesn't exist.\n")); Serial.print(log_1);
    //Serial.println("File doesn't exist.");
    snprintf_P(log_1, sizeof(log_1), PSTR("Creating file:%s\n"), File_name); Serial.print(log_1);
    //Serial.println("Creating file:"); Serial.println(File_name);
    if (Myself.Role != 'M') {

      writeFile(SD, File_name, "Day,Time,Relays state,Target,T_0,T_1,T_2 \r\n", true);
    }

    if (Myself.Role == 'M') {

      //Definir cabeçalho do ficheiro:
      char header[40 + (23 * Slave_Heaters_size) + (23 * Viewers_size)] = "Day,Time,Relays state,Target,TMH_0,TMH_1,TMH_2";
      char novo[23] = {};

      for (int i = 1; i <= Slave_Heaters_size; i++) {
        sprintf(novo, ",TSH%i_0,TSH%i_1,TSH%i_2", i, i, i);
        strcat(header, novo);
        //  Serial.print("header:");    Serial.println(header);//Para confirmar que o cabeçalho esta a ser bem criado.
      }
      for (int i = 1; i <= Viewers_size; i++) {
        if (i < Viewers_size)sprintf(novo, ",TV%i_0,TV%i_1,TV%i_2", i, i, i); else sprintf(novo, ",TV%i_0,TV%i_1,TV%i_2 \r\n", i, i, i);
        strcat(header, novo);
        //  Serial.print("header:");    Serial.println(header);//Para confirmar que o cabeçalho esta a ser bem criado.
      }
      //strcat(header, "/r/n");//Para o log iniciar na linha abaixo do cabeçalho header.
      writeFile(SD, File_name, header, true);
      // writeFile(SD, File_name, "Day,Time,Target,TM1,TM2,TS1,TS2 \r\n");
    }
  }
  else {
    //Serial.print("File:"); Serial.print(File_name); Serial.print(" already exists.");
  }
  file.close();


  if (Myself.Role != 'M') {

    snprintf_P(log_1, sizeof(log_1), PSTR("Im logging as Slave or Viewer.\n")); Serial.print(log_1);
    //Serial.println("Im logging as Slave or Viewer.");
    char Log_data_S[80] = {};
    snprintf_P(Log_data_S, sizeof(Log_data_S), PSTR("%02d/%02d/%d,%d:%d:%d,%d,%d.%02d,%d.%02d,%d.%02d,%d.%02d"), dia, mes, Ano,
               hora, minuto, segundo,
               Relay_state,
               (int)Master_struct.Target, (int)(Master_struct.Target * 100) % 100,
               (int)Myself.Temp_0, (int)(Myself.Temp_0 * 100) % 100,
               (int)Myself.Temp_1, (int)(Myself.Temp_1 * 100) % 100,
               (int)Myself.Temp_2, (int)(Myself.Temp_2 * 100) % 100);
    //Serial.print("Criei o Log:");  Serial.println(Log_data);
    //Append the data to file:
    appendFile(SD, File_name , Log_data_S, 1); //dataMessage.c_str()//File_name//"/data.txt"
    memset(Log_data_S, 0, sizeof Log_data_S);

  }

  if (Myself.Role == 'M') {
    if (!Reset)appendFile(SD, File_name , "Reset", 1);

    snprintf_P(log_1, sizeof(log_1), PSTR("Im logging as Master.\n")); Serial.print(log_1);
    //Serial.println("Im logging as Master.");
    //char Log_data_M[38 + (14 * Slave_Heaters_size) + (14 * Viewers_size)] = {};
    //Serial.println("Criar Log_data_M");
    char Log_data_M[1000];
    //Serial.println("Preencher Log_data_M");
    snprintf_P(Log_data_M, sizeof(Log_data_M), PSTR("%02d/%02d/%d,%d:%d:%d,%d,%d.%02d,%d.%02d,%d.%02d,%d.%02d"), dia, mes, Ano,
               hora, minuto, segundo,
               Relay_state,
               (int)Master_struct.Target, (int)(Master_struct.Target * 100) % 100,
               (int)Myself.Temp_0, (int)(Myself.Temp_0 * 100) % 100,
               (int)Myself.Temp_1, (int)(Myself.Temp_1 * 100) % 100,
               (int)Myself.Temp_2, (int)(Myself.Temp_2 * 100) % 100);
    //Serial.print("Log_data_M:");
    //Serial.print(Log_data_M);
    //Serial.println("ENDLog_data_M:");

    char novo_tmp[15] = {};
    Serial.print("SD saved slaves: ");
    for (int i = 1; i <= Slave_Heaters_size; i++) {
      if (Heaters[i].Received_state == 1) {
        sprintf(novo_tmp, ",%d.%02d,%d.%02d,%d.%02d", (int)Heaters[i].Temp_0, (int)(Heaters[i].Temp_0 * 100) % 100 , (int)Heaters[i].Temp_1, (int)(Heaters[i].Temp_1 * 100) % 100, (int)Heaters[i].Temp_2, (int)(Heaters[i].Temp_2 * 100) % 100 );
        strcat(Log_data_M, novo_tmp);
        memset(novo_tmp, 0, sizeof novo_tmp);
        Heaters[i].Received_state = 2;
        if (!Online_mode)Heaters[i].Received_state = 0;
        //Serial.printf("S%i, ",i);
      } else {
        Serial.printf("ns%i(%i), ", i, Heaters[i].Received_state);
        sprintf(novo_tmp, ",,,");
        strcat(Log_data_M, novo_tmp);
        memset(novo_tmp, 0, sizeof novo_tmp);

      }

      //Serial.print("Log_data:");    Serial.println(Log_data);//Para confirmar que o Log esta a ser bem criado.
    }
    Serial.printf("\n");
    //Serial.println("File name2:"); Serial.println(File_name);
    //appendFile(SD, File_name , "Teste", 1);
    /*
        Serial.print("Log_data_M_heaters:");
        Serial.print(Log_data_M);
        Serial.println("ENDLog_data_M_heaters:");
    */
    appendFile(SD, File_name , Log_data_M, 0);
    memset(Log_data_M, 0, sizeof Log_data_M);
    Serial.print("SD saved viewers: ");
    for (int i = 1; i <= Viewers_size; i++) {
      if (Viewers[i].Received_state == 1) {
        sprintf(novo_tmp, ",%d.%02d,%d.%02d,%d.%02d", (int)Viewers[i].Temp_0, (int)(Viewers[i].Temp_0 * 100) % 100, (int)Viewers[i].Temp_1, (int)(Viewers[i].Temp_1 * 100) % 100, (int)Viewers[i].Temp_2, (int)(Viewers[i].Temp_2 * 100) % 100 );
        strcat(Log_data_M, novo_tmp);
        memset(novo_tmp, 0, sizeof novo_tmp);
        Viewers[i].Received_state = 2;
        if (!Online_mode)Viewers[i].Received_state = 0;

        //Serial.printf("V%i, ",i);

      } else {
        Serial.printf("nv%i(%i), ", i, Viewers[i].Received_state);
        sprintf(novo_tmp, ",,,");
        strcat(Log_data_M, novo_tmp);
        memset(novo_tmp, 0, sizeof novo_tmp);
      }
      //Serial.print("Log_data:");    Serial.println(Log_data);//Para confirmar que o Log esta a ser bem criado.
    }
    Serial.printf("\n");
    //Serial.print("Criei o Log:");  Serial.println(Log_data);
    //Append the data to file:
    //Serial.println("File name3:"); Serial.println(File_name);
    //appendFile(SD, File_name , "Teste", 1);
    /*
        Serial.print("Log_data_M_viewers:");
        Serial.print(Log_data_M);
        Serial.println("ENDLog_data_M_viewers:");
    */
    appendFile(SD, File_name , Log_data_M, 1); //dataMessage.c_str()//File_name//"/data.txt"
    memset(Log_data_M, 0, sizeof Log_data_M);
  }
  //char str_temp[10]={};
  //dtostrf(23.1, 4, 2, str_temp);//Funciona
  //Serial.print("str_temp:");  Serial.println(str_temp);

  //float f = 31.10;
  //sprintf(str_temp, "%d.%02d", (int)f, (int)(f * 100) % 100);
  //Serial.print("f:");  Serial.println(str_temp);

  /* int int_Target = (int)Target; int float_Target = 100 * (Target - int_Target);
    int int_TS1 = (int)TS1; int float_TS1 = 100 * (TS1 - int_TS1);
    int int_TS2 = (int)TS2; int float_TS2 = 100 * (TS2 - int_TS2);
    int int_TM1 = (int)TM1; int float_TM1 = 100 * (TS1 - int_TM1);
    int int_TM2 = (int)TM2; int float_TM2 = 100 * (TS2 - int_TM2);

    snprintf_P(Log_data, sizeof(Log_data), PSTR("%02d/%02d/%d,%d:%d:%d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d"), Inrtc.getDay(), Inrtc.getMonth() + 1, Inrtc.getYear(),
              Inrtc.getHour(true), Inrtc.getMinute(), Inrtc.getSecond(), int_Target, float_Target, int_TM1, float_TM1, int_TM2, float_TM2, int_TS1, float_TS1, int_TS2, float_TS2);
  */

}

void createDir(fs::FS & fs, const char * path) {
  char log_1[50];
  snprintf_P(log_1, sizeof(log_1), PSTR("Creating Dir: %s\n"), path); Serial.print(log_1);
  //Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Dir created.\n"), path); Serial.print(log_1);
    //Serial.println("Dir created.");
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("mkdir failed.\n"), path); Serial.print(log_1);
    //Serial.println("mkdir failed.");
  }
}

void readFile(fs::FS & fs, const char * path, bool Atualizar_Temp_offset) {
  char log_1[80];
  //Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Failed to open file for reading.\n")); Serial.print(log_1);
    //Serial.println("Failed to open file for reading.");
    return;
  }

  //Serial.println("Read from file: ");
  char buffer[50];
  uint8_t i = 0;
  caracteres = 0;
  while (file.available() && i < 49)    // note how this also prevents the buffer from overflowing (18 max to leave space for '\0'!)
  {
    caracteres++;
    buffer[i] = file.read();
    //Serial.print("i:"); Serial.println(i);
    //Serial.print("buffer[i]:"); Serial.println(buffer[i]);
    i++;

    if (file.peek() == '\n') {
      //buffer[i + 1] = '\0';
      //Serial.print("buffer:"); Serial.println(buffer);
      //  Serial.print("buffer size:"); Serial.println(strlen(buffer));

      if (strstr(buffer, ":") > 0) {

        if (strstr(buffer, "WiFi") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Wifi_present = atoi(p);
          Serial.print("Wifi_present:"); Serial.println(Wifi_present);
        }
        if (strstr(buffer, "ssid") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          strncpy(ssid_, p, (strlen(p) - 1));
          ssid = &ssid_[0];
        }
        if (strstr(buffer, "password") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          strncpy(password_, p, (strlen(p) - 1));
          password = &password_[0];//Forma de copiar char* do strtok para char* global.
        }
        if (strstr(buffer, "BLYNK_TEMPLATE_ID") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          strncpy(BLYNK_TEMPLATE_ID_, p, sizeof BLYNK_TEMPLATE_ID_ - 1); /* OK ... but `dst` needs to be NUL terminated */
          BLYNK_TEMPLATE_ID_[sizeof BLYNK_TEMPLATE_ID_ - 1] = '\0';//Forma de copiar char* do strtok para char global.
        }

        if (strstr(buffer, "BLYNK_DEVICE_NAME") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          strncpy(BLYNK_DEVICE_NAME_, p, sizeof BLYNK_DEVICE_NAME_ - 1); /* OK ... but `dst` needs to be NUL terminated */
          BLYNK_DEVICE_NAME_[sizeof BLYNK_DEVICE_NAME_ - 1] = '\0';//Forma de copiar char* do strtok para char global.
        }

        if (strstr(buffer, "auth") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          strncpy(auth, p, (strlen(p) - 1));
          auth[sizeof auth - 1] = '\0';
        }
        //        if (strstr(buffer, "Role") > 0) {
        //          p = strtok(buffer, ":");
        //          p = strtok(NULL, ":");
        //          strncpy(Myself.Role, p, sizeof(Myself.Role - 1));
        //          Myself.Role[sizeof(Myself.Role - 1)] = '\0';
        //          Serial.print("Myself.Role:"); Serial.println(Myself.Role);
        //        }

        if (strstr(buffer, "Role") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          if (p != NULL) Myself.Role = *p;
          //Serial.print("Myself.Role_*p:"); Serial.println(*p);
          Serial.print("Myself.RoleSD:"); Serial.println(Myself.Role);
        }

        if (strstr(buffer, "ID") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          if (p != NULL) Myself.ID = atoi(p);
          //Serial.print("Myself.ID:"); Serial.println(Myself.ID);
        }
        if (strstr(buffer, "Master") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          if (p != NULL) Targeted_Master = atoi(p);
          //Serial.print("Out_Estrutura.Master:"); Serial.println(Out_Estrutura.Master);
        }
        if (strstr(buffer, "Slave_Heaters_size") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Slave_Heaters_size = atoi(p);
        }
        if (strstr(buffer, "Viewers_size") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Viewers_size = atoi(p);
        }
        if (strstr(buffer, "Temp_offset") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          if (Atualizar_Temp_offset)Temp_offset = atof(p);
          local_offset = caracteres - strlen(p);
        }
        if (strstr(buffer, "P_Temp_ripple") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          P_Temp_ripple = atof(p);
          //Serial.print("P_Temp_ripple:"); Serial.println(P_Temp_ripple);
        }
        if (strstr(buffer, "N_Temp_ripple") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          N_Temp_ripple = atof(p);
          //Serial.print("N_Temp_ripple:"); Serial.println(N_Temp_ripple);
        }
        if (strstr(buffer, "Log_period_minutes") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Log_period_minutes = atoi(p);
          //Serial.print("Log_period_minutes:"); Serial.println(Log_period_minutes);
        }
        if (strstr(buffer, "Temp_period_seconds") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Temp_period_seconds = atol(p);
          //Serial.print("Temp_period_seconds:"); Serial.println(Temp_period_seconds);
        }
        if (strstr(buffer, "Online_pin") > 0) {
          p = strtok(buffer, ":");
          p = strtok(NULL, ":");
          Myself.VP = atoi(p);
          Serial.print("Myself.VP:"); Serial.println(Myself.VP);
        }
  
      }
      i = 0;
      memset(buffer, 0, sizeof buffer);
    }
  }

  file.close();
}

void Update_Temp_offset() {
  /*Update EEPROM*/
  Update_eeprom();
  //Update_eeprom();//Tinha 2x Update_eeprom?
  //Read_from_eeprom();
  /**/

  /*Update SD*/
  char log_1[100];
  if (SdExist() && local_offset < 100)readFile(SD, "/settings.txt", false);
  //Caso tenha iniciado sem detetar cartao, tera local_offset=0 em vez de 351 ou algo parecido.
  //Et tenta ir ao cartao atualizar a coordenada de temp_offset no settings.txt .

  if (SdExist() && local_offset > 100) {
    char str[32];
    snprintf(str, sizeof(str), "%.2f\n//End", Temp_offset);
    //dtostrf(Temp_offset, 1, 2, str);//Torna floats em strings.
    writeFile(SD, "/settings.txt", str, false);
    Temp_offset_updated = false;
    Serial.println("Just updated SD card on Temp_offset");
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Tentar religar() para fazer SD update Temp_offset.\n")); Serial.print(log_1);
    if (Religar()) {
      if (local_offset < 100)readFile(SD, "/settings.txt", false); //Se true atualiza Temp_offset.

      if (local_offset > 100) {
        char str[32];
        snprintf(str, sizeof(str), "%.2f\n//End", Temp_offset);
        //dtostrf(Temp_offset, 1, 2, str);//Torna floats em strings.
        writeFile(SD, "/settings.txt", str, false);
        Temp_offset_updated = false;
        Serial.println("Just updated SD card on Temp_offset");
      }

    } else Serial.println("Ou nao conseguiu religar, ou o local_offset continua abaixo de 100.");
  }



}
