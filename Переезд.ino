#include <Servo.h>
Servo myservo;
#define BUTTON  3
#define RED     11
#define YELLOW  12
#define Relay   7

boolean buttonWasUp = true;  // была ли кнопка отпущена?
boolean ledEnabled = false;  // включен ли свет?
 
void setup()
{
  myservo.attach(5);  // подключаем на пин 5
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(Relay, OUTPUT);
 }
 
void loop()
{
  if (buttonWasUp) {
    digitalWrite (YELLOW, HIGH);
     delay(300);
    digitalWrite (YELLOW, 0);
     delay(300);}
  else {
    digitalWrite(Relay, HIGH);
    delay(5000);
    digitalWrite(YELLOW, LOW);
    delay(100000);
  }
  
  // определить момент «клика» несколько сложнее, чем факт того,
  // что кнопка сейчас просто нажата. Для определения клика мы
  // сначала понимаем, отпущена ли кнопка прямо сейчас...
  boolean buttonIsUp = digitalRead(BUTTON);
 
  // ...если «кнопка была отпущена и (&&) не отпущена сейчас»...
  if (buttonWasUp && !buttonIsUp) {
    // ...может это «клик», а может и ложный сигнал (дребезг),
    // возникающий в момент замыкания/размыкания пластин кнопки,
    // поэтому даём кнопке полностью «успокоиться»...
    delay(10);
    // ...и считываем сигнал снова
    buttonIsUp = digitalRead(BUTTON);
    if (!buttonIsUp) {  // если она всё ещё нажата...
      // ...это клик! Переворачиваем сигнал светодиода
      ledEnabled = !ledEnabled;
      myservo.write(0);   // поворот на 0 градусов
      delay(10);
      digitalWrite(RED, 1);
      //digitalWrite(YELLOW,0);
      digitalWrite(Relay, HIGH);
      delay(5000);
    }
    
  }
 
  // запоминаем последнее состояние кнопки для новой итерации
  buttonWasUp = buttonIsUp;
  
  
}
