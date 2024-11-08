#define green1 2
#define yellow1 3
#define red1 4
#define green2 5
#define yellow2 6
#define red2 7
#define green3 8
#define yellow3 9
#define red3 10
#define BUTTON1 11
#define BUTTON2 12
#define BUTTON3 13
int a;
void setup()
{
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  {
    for(a=2; a<11; a++)
  {
   pinMode(a, OUTPUT); 
}
    digitalWrite(green1, HIGH); 
    digitalWrite(green2, HIGH); 
    digitalWrite(green3, HIGH); 
  }}
void loop()
 {
  if ( digitalRead(BUTTON1) == HIGH && digitalRead(BUTTON2) == LOW && digitalRead(BUTTON3) == LOW)
  {
    digitalWrite(red1, HIGH); 
    digitalWrite(green1, LOW);
    digitalWrite(yellow1, LOW); 
    digitalWrite(red2, LOW); 
    digitalWrite(green2, HIGH);
    digitalWrite(yellow2, LOW); 
    digitalWrite(red3, LOW); 
    digitalWrite(green3, HIGH);
    digitalWrite(yellow3, LOW); 
  }
  if (digitalRead(BUTTON1) == LOW && digitalRead(BUTTON2) == HIGH && digitalRead(BUTTON3) == LOW)
  {
    digitalWrite(red1, LOW); 
    digitalWrite(green1, LOW);
    digitalWrite(yellow1, HIGH); 
    digitalWrite(red2, HIGH); 
    digitalWrite(green2, LOW);
    digitalWrite(yellow2, LOW); 
    digitalWrite(red3, LOW); 
    digitalWrite(green3, HIGH);
    digitalWrite(yellow3, LOW); 
  }
  if (digitalRead(BUTTON1) == LOW && digitalRead(BUTTON2) == LOW && digitalRead(BUTTON3) == HIGH)
  {
    digitalWrite(red1, LOW); 
    digitalWrite(green1, HIGH);
    digitalWrite(yellow1, LOW); 
    digitalWrite(red2, LOW); 
    digitalWrite(green2, LOW);
    digitalWrite(yellow2, HIGH); 
    digitalWrite(red3, HIGH); 
    digitalWrite(green3, LOW);
    digitalWrite(yellow3, LOW); 
  }
 }

 
