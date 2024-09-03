/*****************************************************
 
 * This code applies to Metal robotic arm Kit 
 * Through this link you can download the source code:
 * https://github.com/Cokoino/CKK0017

*****************************************************/

#define buzzer_pin A0    //buzzer PIN is A0 on the control board

void setup()
{
  Serial.begin(9600);//Set baud rate
  pinMode(buzzer_pin, OUTPUT);//Set pin A0 to output mode and input signals to the buzzer
}

void loop() 
{
  digitalWrite(buzzer_pin,HIGH);
  delay(50);
  digitalWrite(buzzer_pin,LOW);
  delay(1000);
}
