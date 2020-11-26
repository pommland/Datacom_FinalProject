#include <Servo.h> 
#define L_ang 130
#define M_ang 88
#define R_ang 49

Servo myservo;
String state = "M";
int ang = M_ang;

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(ang);  // set servo to mid-point
} 

void loop() {
  if(Serial.available() > 0) {
    String _state = Serial.readStringUntil('\n');
    if (_state == "L") {
      while (ang < L_ang) {
        myservo.write(++ang);
        delay(10);
      }
      while (ang > L_ang) {
        myservo.write(--ang);  
        delay(10);
      }
    }
    if (_state == "M") {
      while (ang < M_ang) {
        myservo.write(++ang);  
        delay(10);
      }
      while (ang > M_ang) {
        myservo.write(--ang);  
        delay(10);
      }
    }  
    if (_state == "R") {
      while (ang < R_ang) {
        myservo.write(++ang);  
        delay(10);
      }
      while (ang > R_ang) {
        myservo.write(--ang);  
        delay(10);
      }
    }
    Serial.println(_state); 
  }
} 
