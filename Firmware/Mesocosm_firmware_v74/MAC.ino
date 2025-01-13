
void Adjust_MAC_target() {
  char log_1[80];
  char macAddr[18];
  switch (Targeted_Master) {
    case 1: {
        uint8_t MASTER_NEW_MAC_1[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_1, sizeof(MASTER_NEW_MAC_1));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);
        break;
      }
    case 2: {
        uint8_t MASTER_NEW_MAC_2[] = {0x44, 0x44, 0x44, 0x44, 0x44, 0x44};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_2, sizeof(MASTER_NEW_MAC_2));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);
        break;
      }
    case 3: {
        uint8_t MASTER_NEW_MAC_3[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_3, sizeof(MASTER_NEW_MAC_3));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);
        break;
      }
    case 4: {
        uint8_t MASTER_NEW_MAC_4[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_4, sizeof(MASTER_NEW_MAC_4));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);
        break;
      }
    case 5: {
        uint8_t MASTER_NEW_MAC_5[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_5, sizeof(MASTER_NEW_MAC_5));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);
        break;
      }
    default: {
        uint8_t MASTER_NEW_MAC_6[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
        memcpy(masterDeviceMac, MASTER_NEW_MAC_6, sizeof(MASTER_NEW_MAC_6));
        sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", masterDeviceMac[0], masterDeviceMac[1], masterDeviceMac[2], masterDeviceMac[3], masterDeviceMac[4], masterDeviceMac[5]);
        snprintf_P(log_1, sizeof(log_1), PSTR("Targeted macAddr: %s\n"), macAddr); Serial.print(log_1);
        //Serial.println(macAddr);   /break;
      }
  }

}
void Adjust_MAC() {
  char log_1[80];
  Master_struct.ID = Myself.ID;//A struct Master_struct é a que o master envia e os slaves esperam receber.
  Master_struct.Role = Myself.Role;

  //Serial.print("Myself.Role:"); Serial.println(Myself.Role);
  //Serial.print("Master_struct.Role:"); Serial.println(Master_struct.Role);
  switch (Myself.ID) {
    case 1: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_1[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_1[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac1[6];
        WiFi.macAddress(mac1);
        snprintf_P(log_1, sizeof(log_1), PSTR("Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac1[0], mac1[1], mac1[2], mac1[3], mac1[4], mac1[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address5:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
    case 2: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_2[] = {0x44, 0x44, 0x44, 0x44, 0x44, 0x44};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_2[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac2[6];
        WiFi.macAddress(mac2);
        snprintf_P(log_1, sizeof(log_1), PSTR("Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address5:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
    case 3: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_3[] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_3[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac3[6];
        WiFi.macAddress(mac3);
        snprintf_P(log_1, sizeof(log_1), PSTR("Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address5:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
    case 4: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_4[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_4[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac4[6];
        WiFi.macAddress(mac4);
        snprintf_P(log_1, sizeof(log_1), PSTR("Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac4[0], mac4[1], mac4[2], mac4[3], mac4[4], mac4[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address5:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
    case 5: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_5[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_5[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac5[6];
        WiFi.macAddress(mac5);
        snprintf_P(log_1, sizeof(log_1), PSTR("Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address5:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
    default: {
        //Change MAC dps do mode e antes dol begin para n mandar coneçao abaixo.:
        uint8_t MASTER_NEW_MAC_6[] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
        esp_wifi_set_mac(WIFI_IF_STA, &MASTER_NEW_MAC_6[0]);//Altera o MAC. WIFI_IF_STA ou ESP_IF_WIFI_AP .
        byte mac0[6];
        WiFi.macAddress(mac0);
        snprintf_P(log_1, sizeof(log_1), PSTR("DEFAULT - Master ID: %i - Current MAC STA Address1:%02x:%02x:%02x:%02x:%02x:%02x\n"), Myself.ID, mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5] ); Serial.print(log_1);
        //Serial.print("Current MAC STA Address0:  "); Serial.println(WiFi.macAddress()); //MAC visto quando como STA.
        //
        break;
      }
  }
}
