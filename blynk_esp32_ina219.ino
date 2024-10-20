#include <Wire.h>
#include <Adafruit_INA219.h>

// Убедитесь, что эти строки находятся перед подключением Blynk
#define BLYNK_TEMPLATE_ID "*************" // Замените на id прибора Blynk
#define BLYNK_TEMPLATE_NAME "********" // Замените на ваше имя прибора Blynk
char auth[] = "*******************************"; // Замените на ваш токен Blynk

// Wi-Fi настройки
const char* ssid = "*****"; // Замените на имя вашей Wi-Fi сети
const char* password = "*******"; // Замените на пароль вашей Wi-Fi сети

#include <BlynkSimpleEsp32.h>

// Инициализация INA219
Adafruit_INA219 ina219;

// Переменные для хранения значений
float voltage = 0;      // Вольты
float current = 0;      // Амперы
float power = 0;        // Ватты

// Переменные для милли-единиц
int millivolts = 0;     // Милливольты
int milliamps = 0;      // Миллиамперы
int milliwatts = 0;     // Милливатты

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password);
  ina219.begin();
}

void loop() {
  Blynk.run();

  // Чтение данных с INA219
  voltage = ina219.getBusVoltage_V();
  current = ina219.getCurrent_mA() / 1000.0;  // Амперы
  power = voltage * current;  // Ватты

  // Вычисление милли-единиц
  millivolts = voltage * 1000;    // Вольты в милливольты
  milliamps = ina219.getCurrent_mA(); // Прямое чтение в миллиамперах
  milliwatts = power * 1000;      // Ватты в милливатты

  // Отправка данных в Blynk
  Blynk.virtualWrite(V0, voltage);      // Вольты (V)
  Blynk.virtualWrite(V1, current);      // Амперы (A)
  Blynk.virtualWrite(V2, power);        // Ватты (W)

  Blynk.virtualWrite(V3, millivolts);   // Милливольты (mV)
  Blynk.virtualWrite(V4, milliamps);    // Миллиамперы (mA)
  Blynk.virtualWrite(V5, milliwatts);   // Милливатты (mW)

  // Отладочная информация
  Serial.print("Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V, Current: ");
  Serial.print(current, 2);
  Serial.print(" A, Power: ");
  Serial.print(power, 2);
  Serial.println(" W");
  Serial.print("Millivolts: ");
  Serial.print(millivolts);
  Serial.print(" mV, Milliamps: ");
  Serial.print(milliamps);
  Serial.print(" mA, Milliwatts: ");
  Serial.print(milliwatts);
  Serial.println(" mW");

  // Задержка между обновлениями
  delay(2000);  // Обновление каждую 1 секунду
}
