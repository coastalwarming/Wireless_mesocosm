/*Defines:*/
float Version = 2.04;

#include <TimeLib.h>//Convercao de epoch em horas e minutos.
bool NTP_updated = false;
volatile unsigned long Falhei_Blynk_login_time = 0;
volatile bool Falhei_Blynk_login_flag = false;
unsigned long NTP_request_instant = 0;
//Blynk IOT:
char BLYNK_TEMPLATE_ID_[13];
char BLYNK_DEVICE_NAME_[35];
char auth[35] = "";
//Wifi:
bool Wifi_present = false;
const char* ssid;
const char* password;
bool Online_mode = false;
byte Tentativas_wifi = 0;
bool Tentar_wifi = false;
unsigned long millis_wifi_tentativa = 0;
void ajustar_canal_no_master(int canal);
int32_t getWiFiChannel(const char *ssid) ;
void Enviar_canal_aos_slaves();
void InitESPNow();
bool ajusta = false;
int Canal_wifi = 0;
bool Reset = false;
typedef struct M_struct {
  bool No_Viewers = true;
  unsigned long int epoch_esp;
  volatile float Target;
  byte ID;
  char Role;
  volatile byte Canal;
  bool Target_updated;
} M_struct;
M_struct Master_struct;
unsigned long int NTP_epochTime = 0;
bool Temp_offset_updated = false;
//#define BLYNK_TEMPLATE_ID "TMPLaCJ5Y85Y"
#define BLYNK_TEMPLATE_ID "Mesocosm"
//Se nao definir BLYNK_TEMPLATE_ID, mesmo que em branco, surge este erro:error: 'BLYNK_FIRMWARE_TYPE' was not declared in this scope BLYNK_FIRMWARE_TYPE,
//Apenas o BLYNK_TEMPLATE_ID aqui definido permite fazer OTA. Posso sempre fazer OTA onde mudo este BLYNK_TEMPLATE_ID .

//#define BLYNK_DEVICE_NAME "MesocosmDebug5"//Nao preciso definir BLYNK_DEVICE_NAME, basta a variavel BLYNK_DEVICE_NAME_ que vem no cartao e esta na EEProm.
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define BLYNK_FIRMWARE_VERSION "0.01."////Precisa de ter isto definido para compilar.
/**/

/*Libraries*/
#include <Preferences.h>//Eeprom access.
Preferences preferences;
#include "soc/rtc_wdt.h"
#include <WiFi.h>//V2.1
#include <esp_wifi.h>//V2.0 //Para mudar o MAC do Maste, e para usar o   esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
#include <esp_now.h>//V2.0 

#include <ESP32Time.h>//Internal clock;
#include <SparkFun_RV3032.h>//External clock.
#include "time.h"//NTP Time

#include <Update.h>
#include <SPI.h>//SPI access to SD card.
#include <SD.h>//Communication to SD card.
#include <FS.h>//Filesystem access to SD card.

#include <OneWire.h>//OneWire protocol for temperature sensors.
#include <DallasTemperature.h>//Temperature sensores library (DS18B20).

#include "BlynkEdgent.h"//Online IOT cloud.
/**/


//Watchdog
hw_timer_t *timer = NULL;

/*Global variables:*/
//RTC:
unsigned long current_epoch = 0;
bool Outrtc_working = false;
bool Outrtc_adjusted = false;

int Ano = 0;
int mes = 0;
int dia = 0;
int hora = 0;
int Prev_hora = 0;
int minuto = 0;
int segundo = 0;


const char* ntpServer = "pool.ntp.org";

bool ntp_rtc = false;
//

//MAC:
uint8_t masterDeviceMac[6] = {0, 0, 0, 0, 0, 0};
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//Data manegement:

//Temporaries for SD data extract:
char ssid_[15];
char password_[30];
String myString_ssid, myString_password;

//Temperature:
volatile float Temp_offset = 0;
float P_Temp_ripple = 0, N_Temp_ripple = 0.4;
int  resolution = 11;
bool Temp_requested = false, Myself_Temp_1_ok = false, Myself_Temp_2_ok = false, Functional = false;
int Heaters_received = 0, Viewers_received = 0;
//Slaves and targets manegement:
byte Slave_Heaters_size = 0, Viewers_size = 0, Targeted_Master = 0;

//Timing:
byte Log_period_minutes = 1;
unsigned long Temp_period_seconds = 10;
unsigned long previousMillis = 0, previousMillis_1 = 0, previousMillis_2 = 0;
unsigned previousMillis_watchdog = 0;
unsigned currentMillis_watchdog = 0;
//SD:
char *p;
char File_name[35];
byte Relay_state = 0;
bool SdExist_ = false;
int caracteres = 0;
int local_offset = 0;
//GPIO:
const byte Blue_LED = 13;
const byte Botao_GPIO34 = 34;
const byte Sensor_1_Pin = 14;
const byte Sensor_2_Pin = 15;
const byte Rele_1 = 16;
const byte Rele_2 = 17;

//ESPNOW:
bool Resumo_sensores_Flag = false;
esp_now_peer_info_t master;
const esp_now_peer_info_t *masterNode = &master;
esp_err_t sendResult;
esp_now_peer_info_t slave;
const esp_now_peer_info_t *slaveNode = &slave;
bool Connected_to_Master = false;
bool Primeiro_add_peer = false;
const uint8_t maxDataFrameSize = 200;
uint8_t dataToSend[maxDataFrameSize];
byte cnt = 0;
int Deliver_confirmation = 0;
uint8_t missed = 0;
byte cnt_missed_packets = 0;
int ms_sleep = 0;
bool Canais_concorrentes_flag = false;
int Canais_concorrentes_index = 0;
int Melhor_canal[13];
volatile int Master_received = 0;
int entregas[14];
int primaryChan = 0;
byte n_canais_iguais = 0;
int canais_iguais[13];
bool Modo_esperar_flag = false;
byte Found_channel_but_not_master = 0;
//Structs para gestao de dados:
typedef struct Estrutura {
  char MAC[18];
  //byte Role;
  char Role;
  byte ID;
  float Temp_0;
  float Temp_1;
  float Temp_2;
  volatile bool Received;
  byte VP;
  float Good_Temp_1 = 0;
  float Good_Temp_2 = 0;
  float Times = 0;
} Estrutura;
Estrutura Myself;

typedef struct Slaves {
  float Temp_0;
  float Temp_1;
  float Temp_2;
  byte Received_state;//Received_state=0 ainda nao foi atualizado; =1 Atualizado mas nao gravado no SD; =2 Ja foi gravado no cartao, por isso pode ser limpo assim que for postado(limpo=0);
  byte VP;
  float Good_Temp_1 = 0;
  float Good_Temp_2 = 0;
  float Times = 0;
} Slaves;
Slaves Heaters[125];
Slaves Viewers[125];


/**/


/*Classes declaration:*/
//RTC:
ESP32Time Inrtc;
RV3032 Outrtc;
//Temperature sensors:
OneWire oneWire_Sensor_1(Sensor_1_Pin);
OneWire oneWire_Sensor_2(Sensor_2_Pin);
DallasTemperature Sensor_1(&oneWire_Sensor_1);
DallasTemperature Sensor_2 (&oneWire_Sensor_2);
DeviceAddress Sensor_1_Address;
DeviceAddress Sensor_2_Address;
/**/

void setup() {
  char log_1[80];

  delay(2000);
  //Serial:
  Serial.begin(115200);
  Serial.printf("\nSetup initiated.\n");
  Serial.printf("FIRMWARE_VERSION:%.2f\n", Version);

  //


  //Led:
  pinMode(Blue_LED, OUTPUT);
  //

  //Reles:
  pinMode(Rele_1, OUTPUT);
  pinMode(Rele_2, OUTPUT);
  //


  //Sensores Temperatura:
  Sensors_begin();

  //

  //RTC para logar:
  Wire.begin();
  if (!Outrtc.begin()) {
    Outrtc_working = false;
    snprintf_P(log_1, sizeof(log_1), PSTR("Something went wrong on Outrtc.begin(), check wiring.\n")); Serial.print(log_1);
  } else {
    Outrtc_working = true;
    configTime(0, 0, ntpServer);
    //Desligar refresh
    //Serial.print("Registo 10h antes:"); printFullBin(Outrtc.readRegister_(0x10));
    Outrtc.writeBit_(0x10, 2, 1);
    delay(50);

    //Serial.print("Registo 10h apos:  "); printFullBin(Outrtc.readRegister_(0x10));
    //Mudar Registos na RAM
    Outrtc.writeRegister_(0xC0, 0b00100000);//Modo backup pilha com 3v<VDD(3.3v).
    delay(50);
    Outrtc.set24Hour(); //Uncomment this line if you'd like to set the RTC to 24 hour mode.
    delay(50);

    //Update eeprom
    //Serial.print("Registo 3Fh antes:"); printFullBin(Outrtc.readRegister_(0x3F));
    Outrtc.writeRegister_(0x3F, 0b00010001);
    delay(50);
    //Serial.print("Registo 3Fh apos:  "); printFullBin(Outrtc.readRegister_(0x3F));

    //Voltar a ligar refresh
    //Serial.print("Registo 10h antes:"); printFullBin(Outrtc.readRegister_(0x10));
    Outrtc.writeBit_(0x10, 2, 0);
    delay(50);

    //Serial.print("Registo 10h apos:  "); printFullBin(Outrtc.readRegister_(0x10));
    Outrtc.writeRegister_(0xC0, 0b00100000);//Modo backup pilha com 3v<VDD(3.3v).//Teste para ver se rtc nao perde as horas (nem sempre acontece).
    Outrtc.set24Hour();
    delay(50);

    if (Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201)) {//1640995201=1 January 2022 00:00:01
      Outrtc_adjusted = true;
      current_epoch = Outrtc.getEpoch();
      Inrtc.setTime(current_epoch);

      Serial.printf("Inrtc set as Outrtc:\nCurrent_epoch:%i %02i/%02i/%04i %02ih:%02imin\n",
                    current_epoch,
                    day(current_epoch),
                    month(current_epoch),
                    year(current_epoch),
                    hour(current_epoch),
                    minute(current_epoch));
    } else {

      Outrtc_adjusted = false;
      //Inrtc.setTime(946684801);//946684801=1 January 2000 00:00:01 //CRASHA se meter esta hora!
      Inrtc.setTime(1577836801);//1577836801=1 January 2020 00:00:01
      delay(500);
      snprintf_P(log_1, sizeof(log_1), PSTR("Outrtc had time before 2022, had to be set to 1/1/2020 00:00:01,In_RTC:%i Out_RTC:%i\n"), Inrtc.getYear(), Outrtc.getYear()); Serial.println(log_1);


    }
  }


  //

  //Modo wifi antes do set_mac para n mandar coneçao abaixo.
  WiFi.mode(WIFI_STA);
  //

  //SD:
  if (InitSDCard()) {
    updateFromFS(SD);
    snprintf_P(log_1, sizeof(log_1), PSTR("Settings retrieved from SD card.\n")); Serial.print(log_1);
    readFile(SD, "/settings.txt", true);
    createDir(SD, "/Data");
    Update_eeprom();
    SdExist_ = true;
  } else {
    snprintf_P(log_1, sizeof(log_1), PSTR("Settings not retrieved from SD,retrieved from EEPROM.\n")); Serial.print(log_1);
    Read_from_eeprom();
    SdExist_ = false;
  }
  if (Myself.Role == 'M') {
    snprintf_P(log_1, sizeof(log_1), PSTR("I'm a Master! :D\n")); Serial.print(log_1);
    Adjust_MAC();
  }
  if (Myself.Role != 'M') {
    snprintf_P(log_1, sizeof(log_1), PSTR("I'm a %c . (\n"), Myself.Role); Serial.print(log_1);
    Adjust_MAC_target();
  }

  //Blynk
  if (Myself.Role == 'M' && Wifi_present)BlynkEdgent.begin();
  //

  //If WiFi flag is 1, get channel:
  if (Wifi_present && (Myself.Role == 'M')) {
    if (getWiFiChannel(ssid)) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Found my wifi on channel %i!\n"), getWiFiChannel(ssid)); Serial.print(log_1);
      enterConnectNet();
      InitESPNow();
      esp_now_register_recv_cb(OnDataRecv_Master);
      slave.channel = 0;
      slave.encrypt = 0;
      memcpy(slave.peer_addr, broadcastAddress, 6);
      esp_now_add_peer(slaveNode);//esp_now_add_peer(&slave);
    } else {
      Online_mode = false;
      millis_wifi_tentativa = millis();
      InitESPNow();
      esp_now_register_recv_cb(OnDataRecv_Master);
      slave.channel = 0;
      slave.encrypt = 0;
      memcpy(slave.peer_addr, broadcastAddress, 6);
      esp_now_add_peer(slaveNode);
      snprintf_P(log_1, sizeof(log_1), PSTR("Initiated espnow without wifi present, on channel %i .\n"), WiFi.channel()); Serial.print(log_1);
    }
  } else if (!Wifi_present && (Myself.Role == 'M')) {
    InitESPNow();
    esp_now_register_recv_cb(OnDataRecv_Master);
    slave.channel = 0;
    slave.encrypt = 0;
    memcpy(slave.peer_addr, broadcastAddress, 6);
    esp_now_add_peer(slaveNode);
    snprintf_P(log_1, sizeof(log_1), PSTR("WiFi flag is 0 in SD card/eeprom settings.\n")); Serial.print(log_1);
  }
  //

  //Ver estado de wifi power saving mode e mudar:
  esp_wifi_set_ps(WIFI_PS_NONE);//Turn modem sleep off.


  if (Myself.Role != 'M') {
    if (getWiFiChannel(ssid)) {
      Canal_wifi = getWiFiChannel(ssid);
      snprintf_P(log_1, sizeof(log_1), PSTR("O wifi existe por isso vou mudar para o canal do wifi:%i.\n"), Canal_wifi); Serial.print(log_1);
      Ajustar_canal_Master(Canal_wifi);
    } else {
      if (!Find_Master_and_connect_ESPNow()) {
        snprintf_P(log_1, sizeof(log_1), PSTR("Ha canais concorrente no setup!\n")); Serial.print(log_1);
        Find_Master_and_connect_ESPNow_detailed();
      }
      //if (Modo_esperar_flag)Modo_esperar();
    }
  }




  delay(5000);


  //Watchdog
  rtc_wdt_protect_off();      //Disable RTC WDT write protection
  rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_RTC);
  rtc_wdt_set_time(RTC_WDT_STAGE0, 15000);
  rtc_wdt_enable();           //Start the RTC WDT timer
  rtc_wdt_protect_on();       //Enable RTC WDT write protection

  snprintf_P(log_1, sizeof(log_1), PSTR("Finished setup.\n")); Serial.print(log_1);

}

void loop() {

  unsigned long currentMillis = millis();
  unsigned long currentMillis_watchdog = millis();

  if (Online_mode && (Myself.Role == 'M') && Wifi_present && Blynk.connected() && !Reset)Send_Reset_n_Version();
  if (Online_mode && (Myself.Role == 'M') && Wifi_present)NTP_adjust();

  if (currentMillis_watchdog - previousMillis_watchdog >= 1000) {
    rtc_wdt_feed();
    char log_1[25];
    //Serial.printf_P(PSTR("Dog fed. Heap: %u.\n"), ESP.getFreeHeap());
    //snprintf_P(log_1, sizeof(log_1), PSTR("Dog fed. Heap: %u.\n"), ESP.getFreeHeap()); Serial.print(log_1);
    previousMillis_watchdog = currentMillis_watchdog;
  }

  if (Online_mode && (Myself.Role == 'M') && Wifi_present && !Falhei_Blynk_login_flag) BlynkEdgent.run();

  if (Online_mode && (Myself.Role == 'M') && Wifi_present && Falhei_Blynk_login_flag && Falhei_Blynk_login_time  < millis()) {
    Falhei_Blynk_login_flag = false;
    Serial.println("Voltar a fazer blynk connect.");
    BlynkState::set(MODE_CONNECTING_CLOUD);
  }

  if (!Online_mode && (Myself.Role == 'M') && (Wifi_present) && (!Tentar_wifi) && ((currentMillis - millis_wifi_tentativa) > (30 * 1000)))Tentar_wifi = true;

  if (!Online_mode && (Myself.Role == 'M') && Wifi_present && Tentar_wifi) {
    char log_1[80];
    snprintf_P(log_1, sizeof(log_1), PSTR("Tentar_wifi:%i\n"), Tentar_wifi); Serial.print(log_1);
    millis_wifi_tentativa = millis();
    Tentar_wifi = false;
    if (getWiFiChannel(ssid)) {
      Canal_wifi = getWiFiChannel(ssid);
      snprintf_P(log_1, sizeof(log_1), PSTR("Encontrei o meu wifi no canal:%i\n"), Canal_wifi); Serial.print(log_1);


      if (Canal_wifi < 1)Canal_wifi = 1;
      if (Canal_wifi != Master_struct.Canal)for (int i = 0; i < 3; i++) {

          Enviar_canal_aos_slaves();
          delay(100);
        }

      ajustar_canal_no_master(Canal_wifi);
      //BlynkEdgent.run();//Precisa de estar aqui? Arrisco a a cloud estar em baixo.
    }
    else {
      snprintf_P(log_1, sizeof(log_1), PSTR("Nao encontro o meu wifi por isso nem vou tentar.\n")); Serial.print(log_1);
    }
  }



  if (((currentMillis - previousMillis_1) > ((Temp_period_seconds * 1000) - 1000)) && (!Temp_requested))Request_Temperatures(); //Peço temperaturas a cada 9 segundos.

  if (((currentMillis - previousMillis_1) > (Temp_period_seconds * 1000)) && (Temp_requested)) {
    previousMillis_1 = currentMillis;  //Recolho temperaturas a cada 10 segundos.
    Get_Temperatures();

    if (Myself.Role != 'V')Apply_relays();
  }

  if (((currentMillis - previousMillis_2) > (Log_period_minutes * 1000 * 60))) {
    previousMillis_2 = currentMillis;
    if (Temp_offset_updated) {
      Update_Temp_offset();
    } //else Serial.println("Temp_offset_updated is false.");
    Log_send_post();
  }

  if (!Connected_to_Master && (Myself.Role != 'M')) {
    Serial.println("No_Viewers became true_1.");
    Master_struct.No_Viewers = true;
    digitalWrite(Rele_1, LOW);
    digitalWrite(Rele_2, LOW);

    char log_1[25];
    snprintf_P(log_1, sizeof(log_1), PSTR("Vou tentar procurar master.\n")); Serial.print(log_1);

    if (!Find_Master_and_connect_ESPNow()) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Ha canais concorrente no setup!\n")); Serial.print(log_1);

      Find_Master_and_connect_ESPNow_detailed();
    }
    //if (Modo_esperar_flag)Modo_esperar();
  }

}
