void NTP_adjust() {
  static unsigned long NTP_epochTime_ = 0;
  static bool NTP_updated = false;
  time_t now;
  struct tm timeinfo = {};

  //Serial.println("Entrei em NTP_adjust(). ");

  if (!NTP_updated && NTP_request_instant == 0) {
    Serial.println("Fiz pedido getLocalTime().");
    getLocalTime( &timeinfo);
    NTP_request_instant = millis();
  }

  if (NTP_request_instant != 0 && (millis() > (NTP_request_instant + 60000))) {
    NTP_updated = true;
    NTP_request_instant = 0;

    Serial.println("Passaram 60 segundos, vou ler time(&now).");

    time(&now);//O que que isto faz?!

    NTP_epochTime_ = now;

    Serial.printf("Novo NTP_epochTime_:%i %02i/%02i/%04i %02ih:%02imin\n",
                  NTP_epochTime_,
                  day(NTP_epochTime_),
                  month(NTP_epochTime_),
                  year(NTP_epochTime_),
                  hour(NTP_epochTime_),
                  minute(NTP_epochTime_));


 if (NTP_epochTime_ > 1640995201) {//1640995201=1 January 2022 00:00:01
   Serial.println("Inrtc e Outrtc ajustados pelo novo NTP.");
    Inrtc.setTime(NTP_epochTime_);
    if (Outrtc.begin())Outrtc.setEpoch(NTP_epochTime_);
}
    
  }
}
