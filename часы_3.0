#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <cmath>

// Настройки Wi-Fi
const char* ssid = "ffff";
const char* password = "123456780";

// Настройки MQTT
const char* mqtt_server = "mqtt.cloa.su";
const int mqtt_port = 1883;
const char* mqtt_user = "ga1maz";
const char* mqtt_pass = "almazg1234";
const char* mqtt_topic = "watch/1";  // Тема для получения данных

WiFiClient espClient;
PubSubClient client(espClient);

////////////////////////////////////////
float x2 = 54.748327;
float y2 = 55.948332;
////////////////////////////////////////

// Настройки OLED экрана 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Если дисплей не использует отдельный пин для сброса
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Переменная для отслеживания текущего сообщения
String lastMessage = "";

// Массив для хранения элементов сообщения
const int MAX_ARRAY_SIZE = 10;  // Максимальное количество элементов в массиве
String messageArray[MAX_ARRAY_SIZE];
int arraySize = 0;  // Фактическое количество элементов в массиве

void setup() {
  Serial.begin(115200);
  pinMode(7, OUTPUT);

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
  Wire.begin();  // Инициализация I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);  // Пауза перед началом
  display.clearDisplay();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("XIAO_ESP32S3_Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to Wi-Fi...");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Wi-Fi Reconnected");
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Получено сообщение: ");
  Serial.println(message);

  // Разбиваем сообщение на массив по запятым
  arraySize = 0;  // Сбрасываем счетчик элементов массива
  int startIndex = 0;
  int commaIndex;

  while ((commaIndex = message.indexOf(',', startIndex)) != -1 && arraySize < MAX_ARRAY_SIZE) {
    messageArray[arraySize] = message.substring(startIndex, commaIndex);
    messageArray[arraySize].trim();  // Убираем лишние пробелы
    arraySize++;
    startIndex = commaIndex + 1;
  }

  // Добавляем последний элемент (после последней запятой)
  if (arraySize < MAX_ARRAY_SIZE) {
    messageArray[arraySize] = message.substring(startIndex);
    messageArray[arraySize].trim();  // Убираем лишние пробелы
    arraySize++;
  }

  // Выводим массив в Serial для отладки
  Serial.println("Массив сообщения:");
  for (int i = 0; i < arraySize; i++) {
    Serial.print("Элемент ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(messageArray[i]);
  }

  float x1 = atof(messageArray[0].c_str());
  float y1 = atof(messageArray[1].c_str());
 // double x2 = atof(messageArray[2].c_str());
//double y2 = atof(messageArray[3].c_str()); // Создаём переменные параллельно переводя с стр в дабл

  float res1 = x2 - x1;
  float res2 = y2 - y1;
  float k1 = pow(res1, 2);
  float k2 = pow(res2, 2);
  float res = k1 + k2;
  float dlinna = sqrt(res);
  Serial.print(dlinna);



  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (dlinna > 2) {
    display.print("The train far away  ");
    Serial.println("The train far away");
    display.print("Do Poezda - ");
    display.print(dlinna);
    display.print(" KM");

  } else {
    display.println("The train is close");
    digitalWrite(7, HIGH);
    Serial.println("The train is close");
  }

  // Обновляем дисплей
  display.display();
}
