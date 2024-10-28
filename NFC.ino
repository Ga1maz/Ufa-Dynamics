#include <Wire.h>
#include <SPI.h>
// библиотека для работы с RFID/NFC
#include <Adafruit_PN532.h>
// библиотека для работы с GPRS устройством
#include <GPRS_Shield_Arduino.h>
 
// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
// GPRS gprs(Serial1, 2, 3); 
// пин прерывания
#define PN532_IRQ   9
// создаём объект для работы со сканером и передаём ему два параметра
// первый — номер пина прерывания
// вторым — число 100
// от Adafruit был программный сброс шилда 
// в cканере RFID/NFC 13,56 МГц (Troyka-модуль) этот пин не используется
// поэтому передаём цифру, большая чем любой пин Arduino
Adafruit_PN532 nfc(PN532_IRQ, 100);
 
void setup(void)
{
  Serial.begin(9600);
  // инициализация RFID/NFC сканера
  nfc.begin();
  int versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find RFID/NFC reader");
    while(1) {
    }
  }
 
  Serial.println("Found RFID/NFC reader");
  // настраиваем модуль
  nfc.SAMConfig();
  Serial.println("Waiting for a card ...");

}
 
void loop(void)
{
  uint8_t success;
  // буфер для хранения ID карты
  uint8_t uid[8];
  // размер буфера карты
  uint8_t uidLength;
  // слушаем новые метки
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  // если найдена карта
  if (success) {
    // выводим в консоль полученные данные
    Serial.println("Found a card");
    Serial.print("ID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("ID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    delay(1000);
    // открываем последовательный порт для мониторинга действий в программе
    Serial.begin(9600);
  // открываем Serial-соединение с GPRS Shield
    Serial1.begin(9600);
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
    while (!Serial) {
    }
    Serial.print("Serial init OK\r\n");
    // включаем GPRS шилд
    gprs.powerOn();
    // проверяем есть ли связь с GPRS устройством
    while (!gprs.init()) {
      // если связи нет, ждём 1 секунду
      // и выводим сообщение об ошибке
      // процесс повторяется в цикле
      // пока не появится ответ от GPRS устройства
      delay(1000);
      Serial.print("Init error\r\n");
    }
    // выводим сообщение об удачной инициализации GPRS Shield
    Serial.println("GPRS init success");
    // отправляем сообщение по указанному номеру с заданным текстом
    gprs.sendSMS("+79872594344", "Train Ufa - Samara");
    }
}
