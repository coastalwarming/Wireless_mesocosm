void Enviar_canal_aos_slaves() {
  char log_1[100];

  Master_struct.Canal = Canal_wifi;
  snprintf_P(log_1, sizeof(log_1), PSTR("Tou a enviar o novo canal para onde vou pk estou no %i mas vou para o %i.\n "),  WiFi.channel(), Canal_wifi); Serial.print(log_1);
  //Serial.printf("Tou a enviar o novo canal para onde vou pk estou no %i mas vou para o %i.\n ",  WiFi.channel(), Canal_wifi);
  //snprintf_P(log_1, sizeof(log_1), PSTR("Supostamente estou a enviar o canal %i.\n"), Master_struct.Canal); Serial.print(log_1);
  //Serial.printf("Supostamente estou a enviar o canal %i.\n", Master_struct.Canal);
  uint8_t broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_err_t result = esp_now_send(broadcast, (uint8_t*) &Master_struct, sizeof(Master_struct));

  if (result == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Enviado Enviar_canal_aos_slaves().\n")); Serial.print(log_1);
    //Serial.println("Enviado.");//So dá erro se houver problemas na construção dos dados slaves.Não confirma entrega.}
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].\n")); Serial.print(log_1);
    //Serial.println("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].");
  }

}

void ajustar_canal_no_master(int canal) {
  char log_1[100];

  WiFi.disconnect();
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();//adicionado.
  primaryChan = canal;
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  ESP_ERROR_CHECK(esp_wifi_set_channel(primaryChan, secondChan));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));
  WiFi.disconnect();
  //Serial.printf("Vou tentar mudar o canal para:%i no ajustar_canal_no_master().\n", canal);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou tentar mudar o canal para:%i no ajustar_canal_no_master().\n"), canal); Serial.print(log_1);

  if (esp_now_init() == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);
    //Serial.println("ESP NOW INIT!");
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
    //Serial.println("ESP NOW INIT FAILED....");
  }
  memcpy(slave.peer_addr, broadcastAddress, 6);//memcpy( &master.peer_addr, masterDeviceMac, 6 );
  slave.channel = primaryChan;
  slave.encrypt = 0;
  slave.ifidx = WIFI_IF_STA;
  if (esp_now_mod_peer(slaveNode) == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer slave to channel: %i .\n"), primaryChan); Serial.print(log_1);

    //Serial.printf("Modified Peer slave to channel: %i .\n!", primaryChan);
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("FALHOU Modified Peer slave!\n")); Serial.print(log_1);

    //Serial.println("FALHOU Modified Peer slave!");
  }
  esp_now_register_recv_cb(OnDataRecv_Master);
}

int32_t getWiFiChannel(const char *ssid) {
  char log_1[50];

  if (int32_t n = WiFi.scanNetworks()) {
    //Serial.print("Ha n redes:"); Serial.println(n);
    for (uint8_t i = 0; i < n; i++) {
      //snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer slave to channel: %s .\n"), WiFi.SSID(i).c_str()); Serial.print(log_1);
      //Serial.println(WiFi.SSID(i).c_str());
      if ((String(WiFi.SSID(i)) == String(ssid))) {
        return WiFi.channel(i);
      }
    }
  }
  snprintf_P(log_1, sizeof(log_1), PSTR("Wifi not found in getWiFiChannel.\n")); Serial.print(log_1);
  //Serial.println("Wifi not found.");
  return 0;
}

void InitESPNow() {
  char log_1[25];

  //Se a inicialização foi bem sucedida
  if (esp_now_init() == ESP_OK) {
    //Serial.println("ESPNow Init Success.");
  }
  //Se houve erro na inicialização
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESPNow Init Failed.\n")); Serial.print(log_1);
    //Serial.println("ESPNow Init Failed.");
    ESP.restart();
  }
}

void Send_to_master() {
  char log_1[50];

  if ((Master_received > 0) && (Master_struct.Canal != primaryChan))Ajustar_canal_Master(Master_struct.Canal);
  sendMaster();
  Master_received --;
  snprintf_P(log_1, sizeof(log_1), PSTR("Master_received reduced by 1, is now:%i .\n"), Master_received); Serial.print(log_1);

  if (Master_received < -3) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Master_received menor que -3?: %i.\n"), Master_received); Serial.print(log_1);
    snprintf_P(log_1, sizeof(log_1), PSTR("Restarting ESP32 in 2 seconds.\n")); Serial.print(log_1);


    delay(2000);
    ESP.restart();
  }
  snprintf_P(log_1, sizeof(log_1), PSTR("Master_received:%i\n"), Master_received); Serial.print(log_1);

}

void sendMaster() {
  char log_1[50];


  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar no canal:%i\n"), WiFi.channel()); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar Role:%c\n"), Myself.Role); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar ID:%i\n"), Myself.ID); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar Temp_0:%f\n"), Myself.Temp_0); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar Temp_1:%f\n"), Myself.Temp_1); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar Temp_2:%f\n"), Myself.Temp_2); Serial.print(log_1);
  snprintf_P(log_1, sizeof(log_1), PSTR("Vou enviar VP:%i\n"), Myself.VP); Serial.print(log_1);

  esp_err_t result = esp_now_send(masterDeviceMac, (uint8_t*) &Myself, sizeof(Myself));
  if (!result == ESP_OK) {
    char log_1[80];
    snprintf_P(log_1, sizeof(log_1), PSTR("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].\n")); Serial.print(log_1);
    //Serial.println("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].");
  }
}

void Send_to_slaves() {
  char log_1[80];
  Master_struct.epoch_esp = Inrtc.getEpoch();

  Calculo_da_temp_a_enviar();

  Master_struct.Canal = WiFi.channel();
  snprintf_P(log_1, sizeof(log_1), PSTR("Tou a enviar o canal:%i\n"), Master_struct.Canal); Serial.print(log_1);

  uint8_t broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  esp_err_t result = esp_now_send(broadcast, (uint8_t*) &Master_struct, sizeof(Master_struct));



  if (result == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Enviado ESPNOW Broadcast para slaves.\n")); Serial.print(log_1);
    //Serial.println("Enviado."); //So dá erro se houver problemas na construção dos dados slaves.Não confirma entrega.
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].\n")); Serial.print(log_1);

    //Serial.println("Erro nos dados dos slaves alvo, verificar esp_now_send() e macSlaves[].");
  }
  //} else Serial.println("Nada para enviar porque não recebi nada dos Viewers desde o ultimo envio.");
}

void OnDataSent_Master(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char log_1[25];

  //Caso não receba confirmação da entrega, imprime o channel para confirmar que estão no memso canal:
  if (status) {//status = 0 quer dizer que foi entregue, se for 1 é porque não foi entregue.

    snprintf_P(log_1, sizeof(log_1), PSTR("Não foi entregue.\n")); Serial.print(log_1);
    //Serial.println("Não foi entregue.");
  }

  //Enviamos novamente os dados
  //send();
  //Serial.println("");
  snprintf_P(log_1, sizeof(log_1), PSTR("\n")); Serial.print(log_1);

}

void OnDataRecv_Master(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {
  rtc_wdt_feed();
  char log_1[50];

  Estrutura Chegada;
  memcpy(&Chegada, incomingData, sizeof(Chegada));
  if (Chegada.Role == 'S') {

    Serial.printf("Recebi o S%d com Temp_0: %.2f ºC,Temp_1: %.2f ºC, Temp_2: %.2f ºC, .\n", Chegada.ID, Chegada.Temp_0, Chegada.Temp_1, Chegada.Temp_2);
    Heaters[Chegada.ID].Temp_0 = Chegada.Temp_0;
    Heaters[Chegada.ID].Temp_1 = Chegada.Temp_1;
    Heaters[Chegada.ID].Temp_2 = Chegada.Temp_2;

    Heaters[Chegada.ID].Times++;

    if (Heaters[Chegada.ID].Temp_1 > -126 && Heaters[Chegada.ID].Temp_1 < 84)Heaters[Chegada.ID].Good_Temp_1++;
    if (Heaters[Chegada.ID].Temp_2 > -126 && Heaters[Chegada.ID].Temp_2 < 84)Heaters[Chegada.ID].Good_Temp_2++;

    if (Heaters[Chegada.ID].Received_state == 0)Heaters[Chegada.ID].Received_state = 1; //Received_state=1 é atualizado.
    Heaters[Chegada.ID].VP = Chegada.VP;

  }
  if (Chegada.Role == 'V') {
    Serial.printf("Recebi o V%d com Temp_0: %.2f ºC,Temp_1: %.2f ºC,Temp_2: %.2f ºC.\n", Chegada.ID, Chegada.Temp_0, Chegada.Temp_1, Chegada.Temp_2);
    Viewers[Chegada.ID].Temp_0 = Chegada.Temp_0;
    Viewers[Chegada.ID].Temp_1 = Chegada.Temp_1;
    Viewers[Chegada.ID].Temp_2 = Chegada.Temp_2;

    Viewers[Chegada.ID].Times++;

    if (Viewers[Chegada.ID].Temp_1 > -126 && Viewers[Chegada.ID].Temp_1 < 84)Viewers[Chegada.ID].Good_Temp_1++;
    if (Viewers[Chegada.ID].Temp_2 > -126 && Viewers[Chegada.ID].Temp_2 < 84)Viewers[Chegada.ID].Good_Temp_2++;

    if (Viewers[Chegada.ID].Received_state == 0)Viewers[Chegada.ID].Received_state = 1; //Received_state=1 é atualizado.
    Viewers[Chegada.ID].VP = Chegada.VP;

    //Serial.printf("V%d com Times:%.0f,Good_Temp_1:%.0f ºC,Good_Temp_2:%.0f .\n", Chegada.ID, Viewers[Chegada.ID].Times, Viewers[Chegada.ID].Good_Temp_1, Viewers[Chegada.ID].Good_Temp_2);

  }
}

void OnDataRecv_Slave(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {
  char log_1[100];
  snprintf_P(log_1, sizeof(log_1), PSTR("Recebi informação.\n")); Serial.print(log_1);

  M_struct Chegada_Master;
  memcpy(&Chegada_Master, incomingData, sizeof(Chegada_Master));
  if (Chegada_Master.Role == 'M' && Chegada_Master.ID == Targeted_Master) {
    Found_channel_but_not_master = 0;

    //Se as horas recebidas pelo master são pos 1 January 2022 00:00:01:
    if (Chegada_Master.epoch_esp > 1640995201) { //1640995201=1 January 2022 00:00:01
      if (Outrtc_working) {
        NTP_epochTime = Chegada_Master.epoch_esp;
        if (!Outrtc_adjusted) { //Se o meu relogio estava desatualizado: !Outrtc_working = Falho no begin ou esta atras de 01/01/2022.

          Inrtc.setTime(NTP_epochTime);
          Outrtc.setEpoch(NTP_epochTime);

          if (Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201 ))Outrtc_adjusted = true;//1640995201=1 January 2022 00:00:01
          Serial.printf("Set Inrtc and Outrtc by esp-retrieved NTP_epochTime due to Outrtc_working being false, NTP_epochTime:%i.\n", NTP_epochTime);
        } else if (((Outrtc.getEpoch() > NTP_epochTime) && ((Outrtc.getEpoch() - NTP_epochTime) > 60)) ||
                   ((Outrtc.getEpoch() < NTP_epochTime) && ((NTP_epochTime - Outrtc.getEpoch()) > 60))) {
          Serial.print("(((Outrtc.getEpoch() - Chegada_Master.epoch_esp) > 60) || ((Chegada_Master.epoch_esp - Outrtc.getEpoch() ) > 60)): ");
          Serial.println(Outrtc.getEpoch() - NTP_epochTime);
          Serial.println(Outrtc.getEpoch());
          Serial.println(NTP_epochTime);
          NTP_epochTime = Chegada_Master.epoch_esp;
          Inrtc.setTime(NTP_epochTime);
          Outrtc.setEpoch(NTP_epochTime);
          if (Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201 ))Outrtc_adjusted = true;//1640995201=1 January 2022 00:00:01
          Serial.printf("Set Inrtc and Outrtc by esp - retrieved NTP_epochTime due to my UNIX differing over 60seconds, NTP_epochTime:: % i.\n", NTP_epochTime);
        }
      } else if (Outrtc.begin())Outrtc_working = true;
    }

    Master_struct.No_Viewers = Chegada_Master.No_Viewers;
    Serial.printf("Recebi do master Chegada_Master.No_Viewers: % i\n", Chegada_Master.No_Viewers);
    if (Chegada_Master.Target_updated) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Recebi informação com target updated de % f no canal % i.\n"), Chegada_Master.Target, Chegada_Master.Canal); Serial.print(log_1);
      //Serial.printf("Recebi informação com target updated de % f no canal % i.\n", Chegada_Master.Target, Chegada_Master.Canal);
      Master_struct.Target = Chegada_Master.Target;
    } else {
      snprintf_P(log_1, sizeof(log_1), PSTR("Recebi informação sem target updated por isso mantive o meu target de % f no canal % i.\n"), Master_struct.Target, Chegada_Master.Canal); Serial.print(log_1);
      //Serial.printf("Recebi informação sem target updated por isso mantive o meu target de % f no canal % i.\n", Master_struct.Target, Chegada_Master.Canal);
    }

    Master_struct.Canal = Chegada_Master.Canal;
    Master_received = Master_received + 3; //Sobe 1 se receber, desce 1 se enviar, faz cap aos 2,-3 reset.
    if (Master_received >= 2)Master_received = 2;
    snprintf_P(log_1, sizeof(log_1), PSTR("Recebi Target: % f e Canal: % i.\n"), Master_struct.Target,  Master_struct.Canal); Serial.print(log_1);
    //Serial.printf("Recebi Target: % f e Canal: % i.\n", Master_struct.Target,  Master_struct.Canal);
  } else Serial.printf("Recebi informacao de outro master: meu: % i em vez de % i.\n", Chegada_Master.ID , Targeted_Master);
}

void OnDataSent_Slave(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char log_1[25];
  Deliver_confirmation = (status == ESP_NOW_SEND_SUCCESS ? 1 : -1);//status = 0 quer dizer que foi entregue, se for 1 é porque não foi entregue.
  //Serial.print("Deliver_confirmation: "); Serial.println(Deliver_confirmation);
  if (Deliver_confirmation == -1) { //Deliver_confirmation = 1 quer dizer que foi entregue, se for -1 é porque não foi entregue.
    //Serial.println("Não foi entregue.");
    Master_received = 0;
    if ((Connected_to_Master) && (cnt_missed_packets < 10)) {
      cnt_missed_packets++;
      snprintf_P(log_1, sizeof(log_1), PSTR("Falhou % d packet.\n"), cnt_missed_packets); Serial.print(log_1);
      //Serial.printf("Falhou % d packet.\r\n", cnt_missed_packets);
      sendMaster();

      //delay(2000);//Passei de 250ms para 2000ms para ver se da tempo de o master lhe dizer o canal novo.
      for (int l = 0; l < 2000; l++) {
        delay(1);
        yield();
        if (Master_received > 0 )break;
      }
      return;
    } else if ((Connected_to_Master) && (cnt_missed_packets >= 10)) {

      Connected_to_Master = false;
      snprintf_P(log_1, sizeof(log_1), PSTR("Falhou % i packets, vou procurar o master noutro canal.\n"), cnt_missed_packets); Serial.print(log_1);
      cnt_missed_packets = 0;
      //Serial.println("Falhou 5 packets, vou procurar o master noutro canal.");
      return;
    }
  } else if (Deliver_confirmation == 1) {
    Found_channel_but_not_master = 0;
    snprintf_P(log_1, sizeof(log_1), PSTR("Foi entregue.\n")); Serial.print(log_1);
    //Serial.println("Foi entregue.");
    cnt_missed_packets = 0;
  }


}
