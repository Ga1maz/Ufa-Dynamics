#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "dom100a";
const char* password = "almaz@7777";

String token = "7354786369:AAHegRH7-K2UHW9BzypglLbxtaG5nao2Evw";
String chatID = "-4555464569";
String message = "৹❗Поезд был замечен❗৹";

String telegramAPI = "https://api.telegram.org/bot" + token + "/sendMessage?chat_id=" + chatID + "&text=" + message;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Соединямся с WiFi...");
  }
  Serial.println("Соединились с WiFi!");

  HTTPClient http;
  http.begin(telegramAPI);
  int httpCode = http.GET(); 

  if (httpCode > 0) {
    Serial.println("Сообщение в Telegram было отправлено успешно!");
  } else {
    Serial.println("Сообщение в Telegram было отправлено с ошибкой!");
  }
  http.end();
}

void loop() {}
