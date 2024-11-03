#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi and MQTT server credentials
const char* ssid = "dom100a";
const char* password = "almaz@7777";
const char* mqtt_server = "mqtt.cloa.space";
const char* mqtt_user = "ga1maz";
const char* mqtt_password = "almazg1234";

WiFiClient espClient;
PubSubClient client(espClient);

// Configure hardware serial for GPS
HardwareSerial gpsSerial(1); // Use hardware serial 1
TinyGPSPlus gps;

// Variables to store GPS data
String poeGpsData = "";
String myGpsData = "";

void setup() {
  Serial.begin(115200);

  // Configure GPS Serial (TX pin to GPIO 16, RX pin to GPIO 17)
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // Adjust pins according to your setup

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Initialize display with the correct address
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Display", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("gps/coordinates");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages
  poeGpsData = ""; // Clear previous data
  for (int i = 0; i < length; i++) {
    poeGpsData += (char)payload[i];
  }
}

// Read GPS data and display on OLED
void displayGPSData() {
    myGpsData = ""; // Clear previous data

    // Read GPS data
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gps.encode(c);
    }

  // Debug output
  Serial.print("GPS Status: ");
  if (gps.location.isValid()) {
    myGpsData = "Lat: " + String(gps.location.lat(), 6) + " Lon: " + String(gps.location.lng(), 6);
    Serial.print("Valid Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", ");
    Serial.println(gps.location.lng(), 6);
  } else {
    myGpsData = "Waiting for GPS fix...";
    Serial.println("Waiting for GPS fix...");
  }

  // Display both GPS data on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Poezd_GPS:");
  display.setCursor(0, 10);
  display.print(poeGpsData);
  display.setCursor(0, 30);
  display.print("My_GPS:");
  display.setCursor(0, 40);
  display.print(myGpsData);
  display.display();

  Serial.println("Poezd_GPS Data: " + poeGpsData);
  Serial.println("My_GPS Data: " + myGpsData);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  displayGPSData();
  delay(1000); // Delay for readability
}
