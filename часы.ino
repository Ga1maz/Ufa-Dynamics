#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define vibro 4

// Настройки Wi-Fi
const char* ssid = "****";
const char* password = "*******";

// Настройки MQTT
const char* mqtt_server = "************";
const int mqtt_port = ****;
const char* mqtt_user = "********";
const char* mqtt_pass = "**********";
const char* mqtt_topic = "*********";  // Тема для получения данных

WiFiClient espClient;
PubSubClient client(espClient);

// Настройки OLED экрана (при необходимости изменить параметры)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Если дисплей не использует отдельный пин для сброса

// Адрес I2C для дисплея (проверьте свой дисплей, обычно это 0x3C или 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Инициализация серийного порта
  Serial.begin(115200);
  pinMode(vibro, OUTPUT);

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
  if (message == "trane is close" ) {
    digitalWrite(vibro, HIGH);
    delay(1000);

  }

  // Отображаем данные на экране
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("MQTT Message: ");
  display.println(message);
  display.display();
}

