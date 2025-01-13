void Ajustar_canal_Master(int canal) {
  char log_1[100];
  snprintf_P(log_1, sizeof(log_1), PSTR("Eu estava no canal %i e o master estava no %i, vou ajustar.\n"), WiFi.channel(), canal); Serial.print(log_1);
  //Serial.print("Eu estava no canal: "); Serial.print( WiFi.channel()); Serial.print(" mas o master envio o canal "); Serial.print(canal); Serial.print(" e Master_struct.Canal "); Serial.println(Master_struct.Canal);
  //Serial.printf("Eu estava no canal %i e o master estava no %i, vou ajustar.\n", primaryChan,canal);
  Connected_to_Master = true;

  primaryChan = canal;//Sera que vai funcionar quando estiver em modo espera e receber dp master?
  //primaryChan = Master_struct.Canal;

  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  ESP_ERROR_CHECK(esp_wifi_set_channel(primaryChan, secondChan));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

  WiFi.disconnect();//Mudar isto de sitio

  snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel in Ajustar_canal_Master:%i.\n"), WiFi.channel()); Serial.print(log_1);
  //Serial.printf("New Wifi channel in Ajustar_canal_Master:%i.\n", WiFi.channel());

  if (esp_now_init() == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);
    // Serial.println("ESP NOW INIT!");
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
    //Serial.println("ESP NOW INIT FAILED....");
  }


  memcpy( &master.peer_addr, masterDeviceMac, 6 );
  master.channel = primaryChan;
  master.encrypt = 0;
  master.ifidx = WIFI_IF_STA;

  //if (esp_now_mod_peer(masterNode) == ESP_OK) Serial.println("Modified Peer!");
  //Add node first time, else  replace.
  if (!Primeiro_add_peer) {
    if ( esp_now_add_peer(masterNode) == ESP_OK) {//Caso ja tenha este endereço adicionado, não adiciona de novo?
      snprintf_P(log_1, sizeof(log_1), PSTR("Added Peer!\n")); Serial.print(log_1);
      //Serial.println("Added Peer!");
      Primeiro_add_peer = true;
    }
  }  else {
    if (esp_now_mod_peer(masterNode) == ESP_OK) {//Se ja exixtia, apenas modifica?
      snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
      //Serial.println("Modified Peer!");
    }
  }

  esp_now_register_send_cb(OnDataSent_Slave);
  esp_now_register_recv_cb(OnDataRecv_Slave);
  snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel in Ajustar_canal_Master:%i.\n"), WiFi.channel()); Serial.print(log_1);
  //Serial.printf("New Wifi channel in Ajustar_canal_Master:%i.\n", WiFi.channel());

}

int TestSend() {
  char log_1[25];

  strcpy((char*)dataToSend, "Calling Master");

  esp_err_t sendResult = esp_now_send(master.peer_addr, dataToSend, sizeof(dataToSend));

  if (sendResult == ESP_OK) {
    //    Serial.println("Send success");
    return 1;
  } else if (sendResult == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    snprintf_P(log_1, sizeof(log_1), PSTR("ESPNOW not Init.\n")); Serial.print(log_1);
    //Serial.println("ESPNOW not Init.");
  } else if (sendResult == ESP_ERR_ESPNOW_ARG) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Invalid Argument\n")); Serial.print(log_1);
    //Serial.println("Invalid Argument");
  } else if (sendResult == ESP_ERR_ESPNOW_INTERNAL) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Internal Error\n")); Serial.print(log_1);
    //Serial.println("Internal Error");
  } else if (sendResult == ESP_ERR_ESPNOW_NO_MEM) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP_ERR_ESPNOW_NO_MEM.\n")); Serial.print(log_1);
    //Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (sendResult == ESP_ERR_ESPNOW_NOT_FOUND) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Peer not found.\n")); Serial.print(log_1);
    //Serial.println("Peer not found.");
  }
  else if (sendResult == ESP_ERR_ESPNOW_IF) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Interface Error.\n")); Serial.print(log_1);
    //Serial.println("Interface Error.");
  }   else {
    snprintf_P(log_1, sizeof(log_1), PSTR("\nNot sure what happened\t%i\n"), sendResult); Serial.print(log_1);
    //Serial.printf("\nNot sure what happened\t%d", sendResult);
  }
  return 0;
}

int Find_Master_and_connect_ESPNow() {
  Serial.println("Entered Find_Master_and_connect_ESPNow()");

  char log_1[80];

  byte maxIndex = 0;
  byte maxValue = 0;

  for (primaryChan = 1; primaryChan <= 14; primaryChan++) {
    rtc_wdt_feed();

    if (primaryChan == 14)break;

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    ESP_ERROR_CHECK(esp_wifi_set_channel(primaryChan, secondChan));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

    // WiFi.printDiag(Serial);

    WiFi.disconnect();//Mudar isto de sitio

    snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel:%i.\n"), WiFi.channel()); Serial.print(log_1);
    //Serial.printf("New Wifi channel:%i.\n", WiFi.channel());

    if (esp_now_init() == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);
      //Serial.println("ESP NOW INIT!");
    }
    else {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
      //Serial.println("ESP NOW INIT FAILED....");
    }

    memcpy( &master.peer_addr, masterDeviceMac, 6 );
    master.channel = primaryChan;
    master.encrypt = 0;
    master.ifidx = WIFI_IF_STA;

    //Add node first time, else  replace.
    if (primaryChan == 1 && !Primeiro_add_peer) {
      if ( esp_now_add_peer(masterNode) == ESP_OK) {//Caso ja tenha este endereço adicionado, não adiciona de novo?
        snprintf_P(log_1, sizeof(log_1), PSTR("Added Peer.\n")); Serial.print(log_1);
        //Serial.println("Added Peer!");
        Primeiro_add_peer = true;
      }
    }  else {
      if (esp_now_mod_peer(masterNode) == ESP_OK) {//Se ja exixtia, apenas modifica?
        snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
        //Serial.println("Modified Peer!");
      }
    }

    esp_now_register_send_cb(OnDataSent_Slave);
    esp_now_register_recv_cb(OnDataRecv_Slave);


    missed = 0;
    for (int i = 0; i < 3; i++) {

      Deliver_confirmation = 0;


      if (TestSend()) {

        for (int i = 0; i < 1000; i++, ms_sleep--) {
          delay(1);
          yield();
          if (Deliver_confirmation != 0 )break;
        }


      }

      if (Deliver_confirmation == 1 ) {
        //Serial.println("missed");
        //missed--;
        entregas[primaryChan]++;
      } else missed++;
      //Serial.printf("missed: % i.\n ", missed);
      //if (missed >5)break;
      delay(100);
    }
    delay(100);
  }

  for (int i = 1; i < 14; i++) {
    snprintf_P(log_1, sizeof(log_1), PSTR("C % i( % i)\n"), i, entregas[i]); Serial.print(log_1);
    //Serial.printf("C % i( % i), \n", i, entregas[i]);
    if (entregas[i] > maxValue) {
      maxValue = entregas[i];
      maxIndex = i;
    }
  }
  // int Melhor_canal[3]
  Melhor_canal[0] = maxIndex;

  Canais_concorrentes_index = 0;
  Canais_concorrentes_flag = false;

  //Serial.printf("O melhor canal pode ter sido o: % i.\n",  Melhor_canal[0]);
  for (int i = 1; i < 14; i++) {
    if ((entregas[i] == maxValue) && (i != Melhor_canal[0])) {
      Canais_concorrentes_index++;
      Melhor_canal[Canais_concorrentes_index] = i;
      //Serial.printf("Ha mais um concorrente: % i.\n", Canais_concorrentes_index);
      //Serial.printf("O: % i.\n", i);
      Canais_concorrentes_flag = true;
    }
  }

  if (maxValue == 0) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Restarting ESP32 in 2 seconds.\n")); Serial.print(log_1);
    //Serial.println("Restarting ESP32 in 2 seconds");
    delay(2000);
    ESP.restart();
  }

  if (!Canais_concorrentes_flag) {
    Connected_to_Master = true;
    primaryChan = maxIndex;

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    ESP_ERROR_CHECK(esp_wifi_set_channel(primaryChan, secondChan));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

    // WiFi.printDiag(Serial);

    WiFi.disconnect();//Mudar isto de sitio

    snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel:%i.\n"), WiFi.channel()); Serial.print(log_1);
    //Serial.printf("New Wifi channel:%i.\n", WiFi.channel());

    if (esp_now_init() == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);

    }
    else {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
      //Serial.println("ESP NOW INIT FAILED....");
    }

    memcpy( &master.peer_addr, masterDeviceMac, 6 );
    master.channel = primaryChan;
    master.encrypt = 0;
    master.ifidx = WIFI_IF_STA;

    //Add node first time, else  replace.
    if (primaryChan == 1 && !Primeiro_add_peer) {
      if ( esp_now_add_peer(masterNode) == ESP_OK) {//Caso ja tenha este endereço adicionado, não adiciona de novo?
        snprintf_P(log_1, sizeof(log_1), PSTR("Added Peer!\n")); Serial.print(log_1);
        //Serial.println("Added Peer!");
        Primeiro_add_peer = true;
      }
    }  else {
      if (esp_now_mod_peer(masterNode) == ESP_OK) {//Se ja exixtia, apenas modifica?
        snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
        //Serial.println("Modified Peer!");
      }
    }

    esp_now_register_send_cb(OnDataSent_Slave);
    esp_now_register_recv_cb(OnDataRecv_Slave);

    return 1;
  }
  if (Canais_concorrentes_flag) {
    snprintf_P(log_1, sizeof(log_1), PSTR("Ha % i canais concorrentes: % i, % i, % i.\n"), Canais_concorrentes_index + 1, Melhor_canal[0], Melhor_canal[1], Melhor_canal[2]); Serial.print(log_1);
    //Serial.printf("Ha % i canais concorrentes: % i, % i, % i.\n", Canais_concorrentes_index + 1, Melhor_canal[0], Melhor_canal[1], Melhor_canal[2]);
    return 0;
  }

  //Serial.printf("O maior numero de entregas foi % i no canal % i.\n", maxValue , maxIndex);

}

int Find_Master_and_connect_ESPNow_detailed() {
  Serial.println("Entered Find_Master_and_connect_ESPNow_detailed()");

  char log_1[80];

  Master_received = 0;


  Canais_concorrentes_flag = false;
  byte maxIndex = 0;
  byte maxValue = 0;
  memset(entregas, 0, sizeof(entregas));
  for (int i = 0; i < Canais_concorrentes_index; i++) {//tinha <= mas crashou no indice 2 pk tinha canal 0.
    rtc_wdt_feed();

    int canal = Melhor_canal[i];

    if (Master_received > 0 ) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Rececao na Find_Master_and_connect_ESPNow_detailed.\n")); Serial.print(log_1);
      //Serial.println("Rececao na Find_Master_and_connect_ESPNow_detailed.");
      snprintf_P(log_1, sizeof(log_1), PSTR("Recebi do master no Canal:%i.\n"), Master_struct.Canal); Serial.print(log_1);
      //Serial.printf("Recebi do master no Canal:%i.\n", Master_struct.Canal);
      Connected_to_Master = true;
      Ajustar_canal_Master(Master_struct.Canal);
      return 1;
    }


    snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel1: %i.\n"), canal); Serial.print(log_1);
    //Serial.printf("New Wifi channel1: %i.\n", canal);
    if (i == Melhor_canal[Canais_concorrentes_index])break;

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    ESP_ERROR_CHECK(esp_wifi_set_channel(canal, secondChan));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

    // WiFi.printDiag(Serial);

    WiFi.disconnect();//Mudar isto de sitio

    snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel:%i.\n"), WiFi.channel()); Serial.print(log_1);
    //Serial.printf("New Wifi channel: %i.\n", WiFi.channel());

    if (esp_now_init() == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);

    }
    else {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
      //Serial.println("ESP NOW INIT FAILED....");
    }


    memcpy( &master.peer_addr, masterDeviceMac, 6 );
    master.channel = canal;
    master.encrypt = 0;
    master.ifidx = WIFI_IF_STA;


    if (esp_now_mod_peer(masterNode) == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
      //Serial.println("Modified Peer!");
    }

    esp_now_register_send_cb(OnDataSent_Slave);
    esp_now_register_recv_cb(OnDataRecv_Slave);



    //
    for (int k = 0; k < 10; k++) {

      //Serial.printf("int k:%i.\n", k);
      Deliver_confirmation = 0;

      if (TestSend()) {
        //Wait for data sent confirmed
        for (int l = 0; l < 1000; l++, ms_sleep--) {
          delay(1);
          yield();
          if (Deliver_confirmation != 0 )break;
        }
      }

      if (Deliver_confirmation == 1 ) {
        entregas[canal]++;
      }
      delay(500);
    }

  }



  for (int i = 0; i <= Canais_concorrentes_index; i++) {
    snprintf_P(log_1, sizeof(log_1), PSTR("C%i(%i)\n"), Melhor_canal[i], entregas[Melhor_canal[i]]); Serial.print(log_1);
    //Serial.printf("C%i(%i), \n", Melhor_canal[i], entregas[Melhor_canal[i]]);
    if (entregas[Melhor_canal[i]] > maxValue) {
      maxValue = entregas[Melhor_canal[i]];
      maxIndex = Melhor_canal[i];
    }
  }
  n_canais_iguais = 0;

  for (int i = 0; i <= Canais_concorrentes_index; i++) {
    //Serial.printf("maxValue:%i\n", maxValue);
    //Serial.printf("Melhor_canal[i]%i\n", Melhor_canal[i]);
    //Serial.printf("entregas[Melhor_canal[i]]\n", entregas[Melhor_canal[i]]);
    if (entregas[Melhor_canal[i]] == maxValue) {
      canais_iguais[n_canais_iguais] = Melhor_canal[i];
      n_canais_iguais++;
    }
  }

  snprintf_P(log_1, sizeof(log_1), PSTR("Ha %i canais iguais.\n"), n_canais_iguais); Serial.print(log_1);
  //Serial.printf("Ha %i canais iguais.\n", n_canais_iguais);
  /*
    //Houve canais iguais?
    if (n_canais_iguais > 1) {
      Modo_esperar_flag = true;
      return 0;
    }
  */
  if (Found_channel_but_not_master > 2) {
    Serial.println("Restarting ESP32 in 2 seconds because FCBNM>2.");
    delay(2000);
    ESP.restart();

  }




  //Ja sei o melhor canal:
  Connected_to_Master = true;
  Found_channel_but_not_master++;

  primaryChan = maxIndex;

  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  ESP_ERROR_CHECK(esp_wifi_set_channel(primaryChan, secondChan));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

  // WiFi.printDiag(Serial);

  WiFi.disconnect();//Mudar isto de sitio

  //Serial.printf("New Wifi channel: % i.\n", WiFi.channel());
  snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel:%i.\n"), WiFi.channel()); Serial.print(log_1);

  if (esp_now_init() == ESP_OK) {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);
    //Serial.println("ESP NOW INIT!");
  }
  else {
    snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
    //Serial.println("ESP NOW INIT FAILED....");
  }

  memcpy( &master.peer_addr, masterDeviceMac, 6 );
  master.channel = primaryChan;
  master.encrypt = 0;
  master.ifidx = WIFI_IF_STA;

  //Add node first time, else  replace.
  if (primaryChan == 1 && !Primeiro_add_peer) {
    if ( esp_now_add_peer(masterNode) == ESP_OK) {//Caso ja tenha este endereço adicionado, não adiciona de novo?
      snprintf_P(log_1, sizeof(log_1), PSTR("Added Peer!\n")); Serial.print(log_1);
      //Serial.println("Added Peer!");
      Primeiro_add_peer = true;
    }
  }  else {
    if (esp_now_mod_peer(masterNode) == ESP_OK) {//Se ja exixtia, apenas modifica?
      snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
      //Serial.println("Modified Peer!");
    }
  }
  esp_now_register_send_cb(OnDataSent_Slave);
  esp_now_register_recv_cb(OnDataRecv_Slave);

  return 1;
}
/*//Modo esperar()
  void Modo_esperar() {
  char log_1[80];

  Master_received = 0;
  Modo_esperar_flag = false;
  snprintf_P(log_1, sizeof(log_1), PSTR("Modo espera!\n")); Serial.print(log_1);
  //Serial.println("Modo espera!");
  for (int i = 0; i <= n_canais_iguais; i++) {
    rtc_wdt_feed();

    int canal = canais_iguais[i];

    snprintf_P(log_1, sizeof(log_1), PSTR("New Wifi channel1:%i.\n"), canal); Serial.print(log_1);
    //Serial.printf("New Wifi channel1: %i.\n", canal);
    if (i == Melhor_canal[Canais_concorrentes_index])break;

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    ESP_ERROR_CHECK(esp_wifi_set_channel(canal, secondChan));
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

    // WiFi.printDiag(Serial);

    WiFi.disconnect();//Mudar isto de sitio

    if (esp_now_init() == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT!\n")); Serial.print(log_1);

    }
    else {
      snprintf_P(log_1, sizeof(log_1), PSTR("ESP NOW INIT FAILED....\n")); Serial.print(log_1);
      //Serial.println("ESP NOW INIT FAILED....");
    }


    memcpy( &master.peer_addr, masterDeviceMac, 6 );
    master.channel = canal;
    master.encrypt = 0;
    master.ifidx = WIFI_IF_STA;


    if (esp_now_mod_peer(masterNode) == ESP_OK) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Modified Peer!\n")); Serial.print(log_1);
      //Serial.println("Modified Peer!");
    }

    esp_now_register_send_cb(OnDataSent_Slave);
    esp_now_register_recv_cb(OnDataRecv_Slave);

    for (int i = 0; i < (3 * Log_period_minutes * 1000 * 60); i++) {
      delay(1);
      yield();
      if (Master_received > 0 )break;
    }
    if (Master_received > 0 ) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Rececao na Modo_esperar.\n")); Serial.print(log_1);
      //Serial.println("Rececao na Modo_esperar.");
      snprintf_P(log_1, sizeof(log_1), PSTR("Recebi do master no Canal:%i.\n"), Master_struct.Canal); Serial.print(log_1);
      //Serial.printf("Recebi do master no Canal:%i.\n", Master_struct.Canal);
      Connected_to_Master = true;
      Ajustar_canal_Master(Master_struct.Canal);
      return;
    }
  }
  snprintf_P(log_1, sizeof(log_1), PSTR("Restarting ESP32 in 2 seconds.\n")); Serial.print(log_1);
  //Serial.println("Restarting ESP32 in 2 seconds");
  delay(2000);
  ESP.restart();

  }
*/
