void Request_Temperatures() {
  char log_1[25];

  Temp_requested = true;

  //  Serial.println("");
  //Serial.println("Requested Temperature.");
  Sensor_1.requestTemperatures();
  Sensor_2.requestTemperatures();

  //Serial.println("Requested temperatures.");
}
void Sensors_begin() {
  Sensor_1.begin();
  Sensor_1.getAddress(Sensor_1_Address, 0);
  Sensor_1.setResolution(Sensor_1_Address, resolution);
  Sensor_1.setWaitForConversion(false);

  Sensor_2.begin();
  Sensor_2.getAddress(Sensor_2_Address, 0);
  Sensor_2.setResolution(Sensor_2_Address, resolution);
  Sensor_2.setWaitForConversion(false);
}
void Get_Temperatures() {
  Myself.Times++;

  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Myself.Temp_1 = Sensor_1.getTempCByIndex(0);
    if (Myself.Temp_1 > -126)break;
  }
  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Myself.Temp_2 = Sensor_2.getTempCByIndex(0);
    if (Myself.Temp_1 > -126)break;
  }

  if (Myself.Temp_1 > -126 && Myself.Temp_1 < 84) {
    Myself.Good_Temp_1++;
    Myself_Temp_1_ok = true;
  } else Myself_Temp_1_ok = false;

  if (Myself.Temp_2 > -126 && Myself.Temp_2 < 84) {
    Myself.Good_Temp_2++;
    Myself_Temp_2_ok = true;

  } else  Myself_Temp_2_ok = false;

  Temp_requested = false;
  Functional = true;
  if (!Myself_Temp_1_ok && !Myself_Temp_2_ok) {
    Functional = false;
    Serial.println("Functional set to false.");
  }

  //Serial.printf("Myself.Temp_1:%.2f Myself.Temp_2:%.2f Functional:%d \n", Myself.Temp_1, Myself.Temp_2, Functional);

  if (Functional) {
    if (Myself_Temp_1_ok && Myself_Temp_2_ok)Myself.Temp_0 = (Myself.Temp_1 + Myself.Temp_2) / 2;
    if (!Myself_Temp_1_ok && Myself_Temp_2_ok)Myself.Temp_0 = Myself.Temp_2;
    if (Myself_Temp_1_ok && !Myself_Temp_2_ok)Myself.Temp_0 = Myself.Temp_1;
  } else {
    Myself.Temp_0 = -127;
  }

  //Serial.printf("M com Times:%.0f,Good_Temp_1: %.0f ºC,Good_Temp_2:%.0f .\n", Myself.Times, Myself.Good_Temp_1, Myself.Good_Temp_2);

}

void Apply_relays() {
  char log_1[50];
  if (Functional) {
    if (!Master_struct.No_Viewers ) {
      //Serial.println("No_Viewers is false_relay_start.");
      if (Myself.Temp_0 >= (Master_struct.Target + P_Temp_ripple)) {
        digitalWrite(Rele_1, LOW);
        digitalWrite(Rele_2, LOW);
        //Serial.println("Turn relays Off.");
        Relay_state = 0;
      } else if (Myself.Temp_0 <= (Master_struct.Target - N_Temp_ripple)) {
        digitalWrite(Rele_1, HIGH);
        digitalWrite(Rele_2, HIGH);
        //Serial.println("Turn relays ON.");
        Relay_state = 1;
      } //else Serial.println("No relay state.");
      else {
        digitalWrite(Rele_1, LOW);
        digitalWrite(Rele_2, LOW);
        snprintf_P(log_1, sizeof(log_1), PSTR("Sensors fail - Not functional - Realys Off\n")); Serial.print(log_1);
        Serial.println("Sensors fail - Not functional - Realys Off");
      }
    } else {
      digitalWrite(Rele_1, LOW);
      digitalWrite(Rele_2, LOW);
      Serial.println("No_Viewers is true_on relays.");
    }
  } else {
    digitalWrite(Rele_1, LOW);
    digitalWrite(Rele_2, LOW);
    Serial.println("No sensors.");
  }
}

void Log_send_post() {
  if (Myself.Role == 'M')Send_to_slaves();

  char log_1[100];
  if (SdExist()) {
    Create_n_Log_data_to_SD();
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Não consegui logar pk nao detetei o cartão, vou tentar relogar.\n")); Serial.print(log_1);
    if (Religar())Create_n_Log_data_to_SD();
  }
  if ((!Reset) && !Online_mode)Reset = true;



  if ((Myself.Role == 'M') && Online_mode) {
    Send_online_data();
  } else {
    if (Myself.Role != 'M') {
      //snprintf_P(log_1, sizeof(log_1), PSTR("Nao vou postar online porque sou %c.\n"), Myself.Role);
      Serial.print(log_1);
    }
    if (Myself.Role == 'M') {
      snprintf_P(log_1, sizeof(log_1), PSTR("Nao vou postar online porque nao estou online:%i.\n"), Online_mode);
      Serial.print(log_1);
    }
  }
  if (Myself.Role == 'M')Master_struct.Target_updated = false;

  if ((Myself.Role != 'M') && Connected_to_Master) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Sending to master.\n")); Serial.print(log_1);
    //Serial.println("Sending to master");
    Send_to_master();
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Myself.Role:%c\n"), Myself.Role); Serial.print(log_1);
    //Serial.print("Myself.Role"); Serial.println(Myself.Role);
  }
}

void Send_online_data() {
  char log_1[30] = {};


  //Enviar resumo de sensores falhados:
  if (Resumo_sensores_Flag) {
    char Mensagem[61 + (22 * Slave_Heaters_size) + (22 * Viewers_size)] = {};
    char novo[30] = {};

    // strcat(Mensagem, "Sensors:\n");
    sprintf(Mensagem, "\nFW:V%.2f\n%i/%i/%i %i:%i\nSensors success %%:\n", Version, dia, mes, Ano, hora, minuto);

    //Master:
    if (Myself.Times >= Myself.Good_Temp_1 && Myself.Times != 0) {
      sprintf(novo, "M_1:%.0f",  ((Myself.Good_Temp_1 / Myself.Times) * 100));
      //Serial.printf("M1 Good_Temp_1:%f Times:%f M_1:%.0f\n", Myself.Good_Temp_1, Myself.Times, ((Myself.Good_Temp_1 / Myself.Times) * 100));
      Myself.Good_Temp_1 = 0;
      strcat(Mensagem, novo);
    }

    if (Myself.Times >= Myself.Good_Temp_2&& Myself.Times != 0) {
      if (Myself.Times > Myself.Good_Temp_1)sprintf(novo, "   M_2:%.0f\n", ((Myself.Good_Temp_2 / Myself.Times) * 100)); else sprintf(novo, "M_2:%.0f\n", ((Myself.Good_Temp_2 / Myself.Times) * 100));
      //Serial.printf("M2 Good_Temp_2:%f Times:%f M_2:%.0f\n", Myself.Good_Temp_2, Myself.Times, ((Myself.Good_Temp_2 / Myself.Times) * 100));

      Myself.Good_Temp_2 = 0;
      strcat(Mensagem, novo);
    } else if (Myself.Times > Myself.Good_Temp_1 && Myself.Times > 0) strcat(Mensagem, "\n");

    if (Myself.Times == 0)strcat(Mensagem, "M_1:0   M_2:0\n");

    Myself.Times = 0;
    //Serial.print("0 Mensagem:"); Serial.println(Mensagem);

    //Slaves:
    for (int i = 1 ; i <= Slave_Heaters_size; i++) {

      if (Heaters[i].Times >= Heaters[i].Good_Temp_1 && Heaters[i].Times != 0) {
        sprintf(novo, "S%i_1:%.0f", i,  ((Heaters[i].Good_Temp_1 / Heaters[i].Times) * 100));
        Heaters[i].Good_Temp_1 = 0;
        strcat(Mensagem, novo);
      }

      if (Heaters[i].Times >= Heaters[i].Good_Temp_2 && Heaters[i].Times != 0) {
        if (Heaters[i].Times >= Heaters[i].Good_Temp_2 && Heaters[i].Times != 0)sprintf(novo, "   S%i_2:%.0f\n", i, ((Heaters[i].Good_Temp_2 / Heaters[i].Times) * 100)); else sprintf(novo, "S%i_2:%.0f\n", i, ((Heaters[i].Good_Temp_2 / Heaters[i].Times) * 100));
        Heaters[i].Good_Temp_2 = 0;
        strcat(Mensagem, novo);
      } else if (Heaters[i].Times >= Heaters[i].Good_Temp_1 && Heaters[i].Times > 0) strcat(Mensagem, "\n");

      if (Heaters[i].Times == 0) {
        sprintf(novo, "S%i_1:0   S%i_2:0\n", i, i);
        strcat(Mensagem, novo);
      }

      Heaters[i].Times = 0;
    }

    //Viewers:

    for (int i = 1 ; i <= Viewers_size; i++) {

      if (Viewers[i].Times >= Viewers[i].Good_Temp_1 && Viewers[i].Times != 0) {
        sprintf(novo, "V%i_1:%.0f", i,  ((Viewers[i].Good_Temp_1 / Viewers[i].Times) * 100));
        //Serial.printf("V1 Good_Temp_1:%f Times:%f V_1:%.0f\n", Viewers[i].Good_Temp_1, Viewers[i].Times, ((Viewers[i].Good_Temp_1 / Viewers[i].Times) * 100));

        Viewers[i].Good_Temp_1 = 0;
        strcat(Mensagem, novo);
      }

      if (Viewers[i].Times >= Viewers[i].Good_Temp_2 && Viewers[i].Times != 0) {
        if (Viewers[i].Times >= Viewers[i].Good_Temp_1 && Viewers[i].Times != 0)sprintf(novo, "   V%i_2:%.0f\n", i, ((Viewers[i].Good_Temp_2 / Viewers[i].Times) * 100)); else sprintf(novo, "V%i_2:%.0f\n", i, ((Viewers[i].Good_Temp_2 / Viewers[i].Times) * 100));
        //Serial.printf("V2 Good_Temp_2:%f Times:%f V_2:%.0f\n", Viewers[i].Good_Temp_2, Viewers[i].Times, ((Viewers[i].Good_Temp_2 / Viewers[i].Times) * 100));

        Viewers[i].Good_Temp_2 = 0;
        strcat(Mensagem, novo);
      } else if (Viewers[i].Times >= Viewers[i].Good_Temp_1 && Viewers[i].Times > 0) strcat(Mensagem, "\n");

      if (Viewers[i].Times == 0) {
        sprintf(novo, "V%i_1:0   V%i_2:0\n", i, i);
        strcat(Mensagem, novo);
      }

      Viewers[i].Times = 0;
    }

    //
    Blynk.virtualWrite(8, Mensagem);
    //Serial.println("1 Mensagem:"); Serial.println(Mensagem);
    Resumo_sensores_Flag = false;
  }


  //
  //Enviar Target e valores do Master:
  //if (Master_struct.Target_updated)Blynk.virtualWrite(0, Master_struct.Target);

  /*Sera que justifica testar Master_struct.Target_updated antes de atualizar online? Pk isso causa um target 0 por n haver viewers a nunca ser colocado online. /**/
  Blynk.virtualWrite(0, Master_struct.Target);
  Blynk.virtualWrite(2, Temp_offset);

  if (Myself.Temp_0 > -126 && Myself.Temp_0 < 84)Blynk.virtualWrite(Myself.VP, Myself.Temp_0);

  //Serial.printf("Temp_1:%f  Temp_2:%f", Myself.Temp_1, Myself.Temp_2);
  //
  byte Slaves_received = 0, Valid_slaves = 0;
  //Enviar valores dos Heaters:
  Serial.print("Blynk Posted slaves: ");
  if (SdExist_) {
    for (int i = 1 ; i <= Slave_Heaters_size; i++) {//0=desatualizado, 1 atualizado, 2 gravado no SD.
      if (Heaters[i].Received_state == 2) {
        Heaters_received++;
        if (Heaters[i].Temp_0 > -126 && Heaters[i].Temp_0 < 84)Blynk.virtualWrite(Heaters[i].VP, Heaters[i].Temp_0);
        Heaters[i].Received_state = 0;
        Slaves_received++;
        if (Heaters[i].Temp_0 > -126 && Heaters[i].Temp_0 < 84)Valid_slaves++;
      } else {
        Serial.printf("ns%i(%i) ,", i, Heaters[i].Received_state);
      }
    }
  } else {
    for (int i = 1 ; i <= Slave_Heaters_size; i++) {//Se nao houver SD card, posto na net basta que esteja Received_state=1, atualizado mas n gravado no cartao.
      if (Heaters[i].Received_state == 1) {
        Heaters_received++;
        if (Heaters[i].Temp_0 > -126 && Heaters[i].Temp_0 < 84)Blynk.virtualWrite(Heaters[i].VP, Heaters[i].Temp_0);
        Heaters[i].Received_state = 0;
        Slaves_received++;
        if (Heaters[i].Temp_0 > -126 && Heaters[i].Temp_0 < 84)Valid_slaves++;

      } else {
        Serial.printf("ns%i(%i) ,", i, Heaters[i].Received_state);
      }
    }
  }
  Blynk.virtualWrite(4, Slaves_received);
  Blynk.virtualWrite(5, Valid_slaves);

  //Serial.println();

  snprintf_P(log_1, sizeof(log_1), PSTR("Heaters_posted: %i.\n"), Heaters_received); Serial.print(log_1);
  Heaters_received = 0;
  //

  byte Viewers_received = 0, Valid_viewers = 0;
  //Enviar valores dos Viewers:
  Serial.print("Blynk Posted viewers: ");
  if (SdExist_) {
    for (int i = 1 ; i <= Viewers_size; i++) {
      if (Viewers[i].Received_state == 2) {

        //Serial.print("[i]:"); Serial.println(i);
        //Serial.print("Viewers[i].Temp_1: "); Serial.println(Viewers[i].Temp_1);
        if (Viewers[i].Temp_0 > -126 && Viewers[i].Temp_0 < 84)Blynk.virtualWrite(Viewers[i].VP, Viewers[i].Temp_0);
        Viewers[i].Received_state = 0;
        Viewers_received++;
        if (Viewers[i].Temp_0 > -126 && Viewers[i].Temp_0 < 84)Valid_viewers++;
      } else {
        Serial.printf("nv%i(%i) ,", i, Viewers[i].Received_state);
      }
    }
  } else {
    for (int i = 1 ; i <= Viewers_size; i++) {
      if (Viewers[i].Received_state == 1) {

        //Serial.print("[i]:"); Serial.println(i);
        //Serial.print("Viewers[i].Temp_1: "); Serial.println(Viewers[i].Temp_1);
        if (Viewers[i].Temp_0 > -126 && Viewers[i].Temp_0 < 84)Blynk.virtualWrite(Viewers[i].VP, Viewers[i].Temp_0);
        Viewers[i].Received_state = 0;
        Viewers_received++;
        if (Viewers[i].Temp_0 > -126 && Viewers[i].Temp_0 < 84)Valid_viewers++;

      } else {
        Serial.printf("nv%i(%i) ,", i, Viewers[i].Received_state);
      }
    }
  }
  Blynk.virtualWrite(6, Viewers_received);
  Blynk.virtualWrite(7, Valid_viewers);
  Blynk.virtualWrite(3, SdExist_);

  //Serial.println();
  snprintf_P(log_1, sizeof(log_1), PSTR("Viewers_posted: %i.\n"), Viewers_received); Serial.print(log_1);
  Viewers_received = 0;
  //
}
