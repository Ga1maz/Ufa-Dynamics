#include <GPRS_Shield_Arduino.h>

#define PHONE_NUMBER  "+79631413596"
#define MESSAGE  "Warning train!"
#define TRIGGER_PIN 7 
#define CONFIRMATION_DELAY 500 

GPRS gprs(Serial1);
bool actionPerformed = false; 

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  pinMode(TRIGGER_PIN, INPUT); 
  Serial.println("Serial init OK");
  Serial1.begin(9600);
  gprs.powerOn();
  while (!gprs.init()) {
    delay(1000);
    Serial.println("GPRS Init error");
  }
  Serial.println("GPRS init success");
}

void loop() {
  if (!actionPerformed && digitalRead(TRIGGER_PIN) == HIGH) {
    delay(CONFIRMATION_DELAY); 
    if (digitalRead(TRIGGER_PIN) == HIGH) {
      Serial.println("Start to send message ...");
      gprs.sendSMS(PHONE_NUMBER, MESSAGE);
      Serial.println("Success: SMS sent.");
      delay(2000);  
      Serial.print("Start to call ");
      Serial.println(PHONE_NUMBER);
      gprs.callUp(PHONE_NUMBER);
      Serial.println("Success: Call made.");
      actionPerformed = true;  
    }
  }
  delay(100); 
}
