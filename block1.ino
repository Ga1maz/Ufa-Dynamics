//ARDUINO MEGA - CMC, TELEFON, SEMAFOR 
//БИБЛИОТЕКИ 
#include <GPRS_Shield_Arduino.h> 
//ПАРАМЕТРЫ 
#define PHONE_NUMBER  "+79631413596"  
 
GPRS gprs(Serial1); 
 
const int inputPin = 5;  
const int outputPin1 = 6; 
const int outputPin2 = 7;  
 
bool lastState = LOW;  
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;  
 
void setup() 
{ 
  Serial.begin(9600); 
  while (!Serial) { 
  } 
  Serial.print("Serial init OK\r\n"); 
   
  Serial1.begin(9600); 
   
  pinMode(inputPin, INPUT); 
  pinMode(outputPin1, OUTPUT); 
  pinMode(outputPin2, OUTPUT); 
   
  gprs.powerOn(); 
   
  while (!gprs.init()) { 
    delay(1000); 
    Serial.print("GPRS Init error\r\n"); 
  } 
   
  Serial.println("GPRS init success"); 
} 
 
void loop() 
{ 
  int reading = digitalRead(inputPin);  
   
  if (reading != lastState) { 
    lastDebounceTime = millis(); 
  } 
   
  if ((millis() - lastDebounceTime) > debounceDelay) { 
    if (reading == HIGH) { 
      digitalWrite(outputPin1, HIGH); 
      digitalWrite(outputPin2, HIGH); 
       
      gprs.sendSMS(PHONE_NUMBER, "!Train!"); 
      Serial.println("SMS sent!"); 
      delay(2000); 
      gprs.callUp(PHONE_NUMBER); 
      Serial.println("Calling..."); 
       
      delay(10000);  
    } else { 
      digitalWrite(outputPin1, LOW); 
      digitalWrite(outputPin2, LOW); 
    } 
  } 
 
  lastState = reading; 
}
