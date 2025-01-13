BLYNK_WRITE(V1)
{
  char log_1[25];
  //digitalWrite(Blue_LED, param.asInt());
  //snprintf_P(log_1, sizeof(log_1), PSTR("Recebi v1:%f\n"), param.asFloat()); Serial.print(log_1);
  if (param.asFloat() != Temp_offset) {
    Temp_offset = param.asFloat();
    Blynk.virtualWrite(2, Temp_offset);
    Temp_offset_updated = true;
    Serial.print("Temp_offset em BLYNK_WRITE(V1):"); Serial.println(Temp_offset);
    if (Temp_offset_updated)Update_Temp_offset();
  }


  /*Opcoes;*/
  //  int virtual_pin_value = param.asInt();
  //  String virtual_pin_value = param.asStrng()();
  //  float virtual_pin_value = param.asFloat();

  //Blynk.logEvent("Ola", "Mensagem_Ola");
  //Blynk.virtualWrite(V1,0);//Envia 0 pelo V1
  /**/

}

void Send_Reset_n_Version()
{
  char log_[20];
  //snprintf_P(log_1, sizeof(log_1), PSTR("Send Reset online.\n")); Serial.print(log_1);
  Serial.println("Send Reset online.");
  Reset = true;
  //Blynk.virtualWrite(12, 1);
  sprintf(log_, "Reset\nFW:V%.2f\n",  Version);
  Blynk.virtualWrite(14, log_);
  //Serial.print("log_: "); Serial.println(log_);
  //Blynk.logEvent("reset", "Reseted");
}
