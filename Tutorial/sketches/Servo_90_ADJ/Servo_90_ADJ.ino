#include<Servo.h>
Servo myservo;  // Create a servo class

void setup() {  
myservo.attach(2);  //Set the servo control pin as D10
delay(100);          //delay 100ms 
}
/////////////////////////////////////////////////////////
void loop() {
 myservo.write(90);  //The servo is 65 degrees
 delay(1000);
 }
