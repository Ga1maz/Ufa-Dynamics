#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266HTTPClient.h>  // Добавляем библиотеку для HTTP-запросов

// Настройки Wi-Fi
const char* ssid = "****";
const char* password = "*********";

// Настройки MQTT
const char* mqtt_server = "mqtt.cloa.space";
const int mqtt_port = 1883;
const char* mqtt_user = "******";
const char* mqtt_pass = "********";
const char* mqtt_topic = "home/temperature";  // Тема для получения данных

WiFiClient espClient;
PubSubClient client(espClient);

// Настройки OLED экрана (при необходимости изменить параметры)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Если дисплей не использует отдельный пин для сброса

// Адрес I2C для дисплея (проверьте свой дисплей, обычно это 0x3C или 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// HTTP URL для запроса данных
const char* api_url = "https://ufa-d.cloa.space/";  // Укажите URL сайта или API, откуда нужно получать данные

void setup() {
  // Инициализация серийного порта
  Serial.begin(115200);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Настройка MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  // Инициализация OLED дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C - стандартный I2C адрес для большинства дисплеев
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);  // Пауза перед началом

  // Очистка экрана
  display.clearDisplay();
}

void loop() {
  // Подключение к MQTT серверу, если не подключены
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Обновление данных на экране (каждые 1000ms)
  displayData();

  // Пример получения данных с сайта/сервера API
  String apiData = getApiData();
  if (apiData.length() > 0) {
    displayApiData(apiData);  // Отображаем полученные данные
  }
  delay(10000);  // Пауза перед следующим запросом к API
}

void reconnect() {
  // Пытаемся подключиться к MQTT серверу
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Подписываемся на топик
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Функция обратного вызова для получения сообщений от MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Преобразуем полученные данные в строку
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Выводим данные в серийный порт для отладки
  Serial.print("Received message: ");
  Serial.println(message);

  // Отображаем данные на экране
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Received: ");
  display.println(message);
  display.display();
}

void displayData() {
  // Просто обновляем данные на экране, если нужно.
  // В этом примере мы выводим строку о состоянии соединения с MQTT.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("MQTT Status: ");
  if (client.connected()) {
    display.println("Connected");
  } else {
    display.println("Disconnected");
  }
  display.display();
}

// Функция для получения данных с сайта/API
String getApiData() {
  WiFiClient client;  // Создаём клиент для HTTP-запроса
  HTTPClient http;
  String payload = "";
  
  // Выполняем HTTP-запрос
  http.begin(client, api_url);  // Новый способ вызова begin() с WiFiClient
  int httpCode = http.GET();  // Выполняем GET-запрос

  if (httpCode > 0) {
    // Если запрос успешен, получаем ответ
    payload = http.getString();
    Serial.println("API Data: " + payload);
  } else {
    Serial.println("Error on HTTP request");
  }

  // Закрываем соединение
  http.end();
  return payload;
}

// Функция для отображения данных с сайта/сервера API на экране
void displayApiData(String data) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Разбиваем данные на несколько строк, если они длинные
  int lineHeight = 10; // Высота строки
  int yPos = 0;

  for (int i = 0; i < data.length(); i++) {
    display.print(data[i]);
    yPos = (i / 16) * lineHeight + 10; // Перенос строки каждые 16 символов
    if (yPos < SCREEN_HEIGHT - lineHeight) {
      display.setCursor(0, yPos);
    }
  }

  display.display();
}
