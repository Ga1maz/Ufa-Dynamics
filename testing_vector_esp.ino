#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "dom100a";
const char* password = "almaz@7777";

const char* mqtt_server = "mqtt.cloa.space";
const char* mqtt_user = "ga1maz";
const char* mqtt_password = "almazg1234";
const char* topic = "gps/coordinates";

WiFiClient espClient;
PubSubClient client(espClient);
String gpsCoordinates = "N/A";

void setupWiFi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(9600);
  Wire.begin(D1, D2);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Не удалось инициализировать дисплей!"));
    for (;;);
  }
  display.clearDisplay();
  
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Powered by:");
  display.setCursor(0, 20);
  display.println("Telegram: @al_maz_g");
  display.display();
  delay(2000);
  display.setTextSize(1);

  for (int i = 0; i < 5; i++) {
    display.clearDisplay();
    display.setCursor(0, 50);
    display.println("Loading");
    for (int j = 0; j <= i; j++) {
      display.print(".");
    }
    display.display();
    delay(100);
  }

  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    reconnect();
  }
  client.subscribe(topic);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Poezd_GPS:");
  display.setCursor(0, 10);
  display.println(gpsCoordinates);
  display.setCursor(0, 25);
  display.println("Vektor_GPS:");
  display.setCursor(0, 35);
  display.println(gpsCoordinates);
  display.setCursor(0, 50);
  display.println("STATUS: " + gpsCoordinates);
  display.display();
  delay(500);
}

void setupWiFi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Wi-Fi подключен");
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  gpsCoordinates = message;
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      client.subscribe(topic);
    } else {
      delay(500);
    }
  }
}
