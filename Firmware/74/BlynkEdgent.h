/* Extern meaning:
   Since the extern keyword extends the function’s visibility to the whole program,
   the function can be used (called) anywhere in any of the files of the whole program,
   provided those files contain a declaration of the function. (With the declaration of the function in place,
   the compiler knows the definition of the function exists somewhere else and it goes ahead and compiles the file).
   So that’s all about extern and functions.
*/
extern "C" {
  void app_loop();
  void eraseMcuConfig();
  void restartMCU();
}

#include "Settings.h"
#include <BlynkSimpleEsp32_SSL.h>

#ifndef BLYNK_NEW_LIBRARY
#error "Old version of Blynk library is in use. Please replace it with the new one."
#endif

//Fui eu a testra o blynk id por cartao sd.
/*
#if !defined(BLYNK_TEMPLATE_ID) || !defined(BLYNK_DEVICE_NAME)
#error "Please specify your BLYNK_TEMPLATE_ID and BLYNK_DEVICE_NAME"
#endif
*/
#include "BlynkState.h"
#include "ConfigStore.h"
#include "ResetButton.h"
#include "ConfigMode.h"

#include "OTA.h"
#include "Console.h"

inline
void BlynkState::set(State m) {
  if (state != m && m < MODE_MAX_VALUE) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;

    // You can put your state handling here,
    // i.e. implement custom indication
  }
}


void runBlynkWithChecks() {
  char log_1[25];
  Blynk.run();
  if (BlynkState::get() == MODE_RUNNING) {

    if (!Blynk.connected()) {

      //Serial.println("MEU:!Blynk.connected()");
      //Serial.printf_P(PSTR("MEU:!Blynk.connected()\n"));
      snprintf_P(log_1, sizeof(log_1), PSTR("MEU:!Blynk.connected()\n")); Serial.print(log_1);

      if (WiFi.status() == WL_CONNECTED) {
        BlynkState::set(MODE_CONNECTING_CLOUD);
        //Serial.println("MEU: WiFi.status() == WL_CONNECTED");
        //Serial.printf_P(PSTR("MEU: WiFi.status() == WL_CONNECTED\n"));
        snprintf_P(log_1, sizeof(log_1), PSTR("MEU: WiFi.status() == WL_CONNECTED\n")); Serial.print(log_1);

      } else {
        BlynkState::set(MODE_CONNECTING_NET);
        //Serial.println("MEU: MODE_CONNECTING_NET");
        //Serial.printf_P(PSTR("MEU: MODE_CONNECTING_NET\n"));
        snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_CONNECTING_NET\n")); Serial.print(log_1);

      }
    }
  }


}

class Edgent {

  public:
    void begin()
    {
      //   WiFi.persistent(false);//Por defeito WiFi.persistent esta falso e implica que os dados wifi n sao apagados no reset ou no WiFi.disconnect() .
      //  WiFi.enableSTA(true); // Needed to get MAC

      //indicator_init();
      button_init();//Deteta botão de rest pressionado.
      config_init();
      console_init();

      //  printDeviceBanner();

      if (configStore.getFlag(CONFIG_FLAG_VALID)) {
        BlynkState::set(MODE_CONNECTING_NET);
      } else if (config_load_blnkopt()) {
        DEBUG_PRINT("Firmware is preprovisioned");
        BlynkState::set(MODE_CONNECTING_NET);
      } else {
        BlynkState::set(MODE_WAIT_CONFIG);
      }
    }

    void run() {
      char log_1[25];
      app_loop();
      switch (BlynkState::get()) {
        case MODE_WAIT_CONFIG:  {
            snprintf_P(log_1, sizeof(log_1), PSTR("MODE_WAIT_CONFIG\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MODE_WAIT_CONFIG\n"));
          }
        case MODE_CONFIGURING: {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_CONFIGURING\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: MODE_CONFIGURING\n"));
            enterConfigMode();
            break;
          }
        case MODE_CONNECTING_NET: {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_CONNECTING_NET\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: MODE_CONNECTING_NET\n"));
            //Serial.println("MEU: MODE_CONNECTING_NET");
            enterConnectNet();
            break;
          }
        case MODE_CONNECTING_CLOUD: {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: enterConnectCloud\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: enterConnectCloud\n"));
            //Serial.println("MEU: enterConnectCloud");

            enterConnectCloud();
            break;
          }
        case MODE_RUNNING: {
            //Serial.println("MEU: MODE_RUNNING");
            runBlynkWithChecks();
            break;
          }
        case MODE_OTA_UPGRADE:  {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_OTA_UPGRADE\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: MODE_OTA_UPGRADE\n"));
            //Serial.println("MEU: MODE_OTA_UPGRADE");
            enterOTA();
            break;
          }
        case MODE_SWITCH_TO_STA:  {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_SWITCH_TO_STA\n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: MODE_SWITCH_TO_STA\n"));
            //Serial.println("MEU: MODE_SWITCH_TO_STA");
            enterSwitchToSTA();
            break;
          }
        case MODE_RESET_CONFIG:  {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: MODE_RESET_CONFIG. \n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: MODE_RESET_CONFIG\n"));
            //Serial.println("MEU: MODE_RESET_CONFIG");
            enterResetConfig();
            break;
          }
        default:            {
            snprintf_P(log_1, sizeof(log_1), PSTR("MEU: default. \n")); Serial.print(log_1);
            //Serial.printf_P(PSTR("MEU: default\n"));
            //Serial.println("MEU: default");
            enterError();
            break;
          }
      }
    }

};

Edgent BlynkEdgent;
BlynkTimer edgentTimer;

void app_loop() {
  edgentTimer.run();
  edgentConsole.run();
}
