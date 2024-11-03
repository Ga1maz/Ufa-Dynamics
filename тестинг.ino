#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <FastLED.h> // Include FastLED library

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

// FastLED configuration
#define LED_PIN 18       // Pin connected to the data line of the LED strip
#define LED_COUNT 120    // Number of LEDs in the strip
CRGB leds[LED_COUNT];

// Button configuration
#define BUTTON_PIN 15     // Pin connected to the button (ground)

// GPS setup
WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// Variables to store GPS data
String poeGpsData = "";
String myGpsData = "";
bool yellowBlinking = false; // Для состояния желтого мигания
bool buttonPressed = false;   // Состояние кнопки
bool ledState = false; // Переменная для хранения состояния LED

// Distance threshold in meters
const double DISTANCE_THRESHOLD = 1000.0;

// Установите пин для управления 19
#define CONTROL_PIN 19

void setup() {
    Serial.begin(115200);
    
    // Configure GPS Serial
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
    
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

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Initialize FastLED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.clear(); // Initialize all LEDs to 'off'

    // Set the button pin as input with pullup
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(CONTROL_PIN, OUTPUT); // Настройка пина 19 как выход
    digitalWrite(CONTROL_PIN, LOW); // Убедитесь, что пин 19 выключен
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
    poeGpsData = "";
    for (int i = 0; i < length; i++) {
        poeGpsData += (char)payload[i];
    }
}

void displayGPSData() {
    myGpsData = ""; 

    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gps.encode(c);
    }

    Serial.print("GPS Status: ");
    if (gps.location.isValid()) {
        myGpsData = String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6);
        Serial.print("Valid Location: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(",");
        Serial.println(gps.location.lng(), 6);
    } else {
        myGpsData = "N/A";
        Serial.println("Waiting for GPS fix...");
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Poezd_GPS:");
    display.setCursor(0, 10);
    display.print(poeGpsData);
    display.setCursor(0, 20);
    display.print("My_GPS:");
    display.setCursor(0, 30);
    display.print(myGpsData);
    display.setCursor(0, 40);

    // Display status based on LED state
    if (yellowBlinking) {
        display.print("Status: Understand");
    } else {
        double distance = -1;
        if (gps.location.isValid() && poeGpsData.length() > 0) {
            double poeLat, poeLng;
            sscanf(poeGpsData.c_str(), "%lf,%lf", &poeLat, &poeLng);
            distance = gps.distanceBetween(gps.location.lat(), gps.location.lng(), poeLat, poeLng);
        }

        if (distance < DISTANCE_THRESHOLD && distance >= 0) {
            display.print("Status: Train");
        } else if (gps.location.isValid()) {
            display.print("Status: Ok");
        } else {
            display.print("Status: Setting up");
        }
    }

    display.display();
    delay(50);  // Add a small delay to give the display time to refresh
}

void controlLEDs() {
    static unsigned long lastBlinkTime = 0;

    // Calculate distance if both GPS are valid
    double distance = -1;
    if (gps.location.isValid() && poeGpsData.length() > 0) {
        double poeLat, poeLng;
        sscanf(poeGpsData.c_str(), "%lf,%lf", &poeLat, &poeLng);
        distance = gps.distanceBetween(gps.location.lat(), gps.location.lng(), poeLat, poeLng);
    }

    if (yellowBlinking) {
        // Включить желтые светодиоды
        if (millis() - lastBlinkTime > 500) {
            ledState = !ledState; // Переключить состояние LED
            if (ledState) {
                fill_solid(leds, LED_COUNT, CRGB::Yellow); // Установить светодиоды в желтый цвет
            } else {
                fill_solid(leds, LED_COUNT, CRGB::Black); // Выключить светодиоды
            }
            FastLED.show();
            lastBlinkTime = millis();
        }
    } else {
        // Если желтое мигание не активно
        if (distance < DISTANCE_THRESHOLD && distance >= 0) {
            // Включить красные светодиоды
            fill_solid(leds, LED_COUNT, CRGB::Red); // Установить светодиоды в красный цвет
            FastLED.show();
            digitalWrite(CONTROL_PIN, HIGH); // Включить пин 19 для индикации
        } else {
            // Выключить светодиоды, когда не красные
            fill_solid(leds, LED_COUNT, CRGB::Black); // Выключить светодиоды
            FastLED.show();
            digitalWrite(CONTROL_PIN, LOW); // Выключить пин 19
        }
    }

    // Проверка нажатия кнопки
    if (digitalRead(BUTTON_PIN) == LOW) {
        // Кнопка нажата
        if (!buttonPressed) {
            // Если кнопка была не нажата раньше, меняем состояние
            yellowBlinking = true; // Включаем желтое мигание
            buttonPressed = true;   // Запоминаем, что кнопка нажата
        }
    } else {
        // Кнопка отпущена
        buttonPressed = false; // Сбрасываем состояние кнопки
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    displayGPSData();
    controlLEDs();
    delay(100); 
}
