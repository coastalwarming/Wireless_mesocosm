
//função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule() {
  //Blynk.virtualWrite(13, 1);
  ets_printf("(watchdog) reiniciar\n"); //imprime no log
  restartMCU();
}

//função que o configura o temporizador
void configureWatchdog()
{
  timer = timerBegin(0, 80, true); //timerID 0, div 80
  //timer, callback, interrupção de borda
  timerAttachInterrupt(timer, &resetModule, true);
  //timer, tempo (us), repetição
  timerAlarmWrite(timer, 60000000, true);//1000000=1segundo
  timerAlarmEnable(timer); //habilita a interrupção //enable interrupt
}
