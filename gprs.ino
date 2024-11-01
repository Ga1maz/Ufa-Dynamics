#include <GPRS_Shield_Arduino.h>

// номер на который будем звонить
#define PHONE_NUMBER  "+79177537900"
#define TEXT "Train!"

// Библиотека для работы с дальномерами
#include "TFLidar.h"
 
// Библиотека для работы программного Serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
// Serial-порт к которому подключён дальномер
#define LIDAR_SERIAL    mySerial
TFLidar lidar;
int dist;
// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);

// флаг для предотвращения повторного выполнения
bool actionExecuted = false;

void setup() {
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.print("Serial init OK\r\n");

  // инициализация GPRS модуля
  Serial1.begin(9600);
  gprs.powerOn();
  while (!gprs.init()) {
    delay(1000);
    Serial.print("GPRS Init error\r\n");
  }
  Serial.println("GPRS init success");

  // настройка пина 7 как входа с подтягивающим резистором
  pinMode(7, INPUT_PULLUP);
  
  // настройка пина 8 как выхода
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);  // по умолчанию установим LOW на пине 8
    // Открываем последовательный порт для мониторинга действий в программе
  // и передаём скорость 9600 бод
  Serial.begin(9600);
  // Ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial);
 
  Serial.print("Serial init OK\r\n");
  // Открываем Serial-соединение с дальномером на скорости 115200
  LIDAR_SERIAL.begin(115200);
  lidar.begin(&LIDAR_SERIAL);  
}

void loop() {
  // Проверяем, если на 7 пине GND и действие ещё не выполнено
  if (lidar.getData(dist) == LOW && !actionExecuted) {
    // Устанавливаем высокий уровень (5V) на пине 8 навсегда
    digitalWrite(8, HIGH);
    Serial.print("Start to sms");
    gprs.sendSMS(PHONE_NUMBER, TEXT);
    delay(5000);

    Serial.print("Start to call ");
    Serial.print(PHONE_NUMBER);
    gprs.callUp(PHONE_NUMBER);

    // Устанавливаем флаг, чтобы не выполнять действие повторно
    actionExecuted = true;
  }
}
