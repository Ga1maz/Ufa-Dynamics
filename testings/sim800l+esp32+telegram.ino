#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGsmClient.h>

#define SIM800L_IP5306_VERSION_20190610

#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG          SerialMon

#include "utilities.h"

#define SerialMon Serial
#define SerialAT  Serial1

#define TINY_GSM_MODEM_SIM800          
#define TINY_GSM_RX_BUFFER      1024  

const char* ssid = "dom100a";
const char* password = "almaz@7777";

String token = "7354786369:AAHegRH7-K2UHW9BzypglLbxtaG5nao2Evw";
String chatID = "-4555464569";
String message = "৹❗Поезд был замечен❗৹";

String telegramAPI = "https://api.telegram.org/bot" + token + "/sendMessage?chat_id=" + chatID + "&text=" + message;

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define SMS_TARGET  "+79631413596"
#define CALL_TARGET "+79631413596"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  if (setupPMU() == false) {
      Serial.println("Setting power error");
  }

  setupModem();

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Соединямся с WiFi...");
  }
  Serial.println("Соединились с WiFi!");

  HTTPClient http;
  delay(10000);
  http.begin(telegramAPI);
  int httpCode = http.GET(); 

  if (httpCode > 0) {
    Serial.println("Сообщение в Telegram было отправлено успешно!");
  } else {
    Serial.println("Сообщение в Telegram было отправлено с ошибкой!");
  }
  http.end();


  SerialMon.println("Initializing modem...");
    modem.restart();

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  bool  res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
  DBG("SMS:", res ? "OK" : "fail");

    // Do nothing forevermore
  while (true) {
      modem.maintain();
  }

  delay(5000);

  SerialMon.println("Initializing modem...");
    modem.restart();

  SerialAT.print("AT+CHFA=1\r\n");
    delay(2);

  SerialAT.print("AT+CRSL=100\r\n");
    delay(2);

  SerialAT.print("AT+CLVL=100\r\n");
    delay(2);

  SerialAT.print("AT+CLIP=1\r\n");
    delay(10000);

  Serial.print("Calling:");
  Serial.println(CALL_TARGET);

  bool res = modem.callNumber(CALL_TARGET);

  Serial.print("Call:");
  Serial.println(res ? "OK" : "fail");

  res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");

    // Do nothing forevermore
    while (true) {
        modem.maintain();
    }
}

void loop() {}
