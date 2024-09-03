/*****************************************************
 
 * This code applies to Mechenical Robotic Arm Kit
 * Through this link you can download the source code:
 * https://github.com/Cokoino/CKK0017
 * Company web site:
 * http://cokoino.com/
 
*****************************************************/


#include <Regexp.h>
#include "src/CokoinoArm.h"

CokoinoArm arm;
#define buzzerPin A0

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// regular
MatchState ms;
/// Car driving direction control button on the app interface, a total of 8 direction buttons.
const String phone1 = "fS";  // forwardStart:                    
const String phone1_5 = "lfS";  // forward_left_Start            
const String phone2 = "lS";    // leftStart                     
const String phone2_5 = "lbS";  // left_backward_Start        
const String phone3 = "bS";    // backwardStart                    
const String phone3_5 = "rbS";  // backward_right_Start           
const String phone4 = "rS";    // rightStart                     
const String phone4_5 = "rfS";  // right_forward_Start         
/// The other function buttons on the app interface
const String phone5 = "OK";//stop
const String phone6 = "rtl";//rotation left
const String phone7 = "rtr";//rotation right
const String phone8 = "trk";//track line running
const String phone9 = "aod";//Avoid obstacles
const String phone10 = "lgt";//light show
const String phone11 = "muc";//buzzer
const String phone12 = "flt";//fllow light
const String phone13 = "bt1";//button1
const String phone14 = "bt2";//button1

String comdata = "";//import the comdata string
char judge = 0;//init the judge

void setup() {

  Serial.begin(115200);
  pinMode(buzzerPin,OUTPUT);
  
  arm.ServoAttach(2,3,4,5,6,7);// 将转向舵机连接到数字234567脚
  delay(100);  // If the information printed out of the serial port is garbled, extend the delay time to solve the problem.
  while (Serial.read() >= 0)
    continue;
  Serial.flush();
  ESP8266_ATCOMMAND();//esp-01 module AT instruction function
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {

  while (Serial.available() > 0) {
    comdata += char(Serial.read());
    delay(1);
  }
  judgement();
}

// ESP8266 set the AT instructionS
void ESP8266_ATCOMMAND() {

  Serial.print(F("AT+RST\r\n"));  //F(): Store string constants in Flash flash to avoid memory depletion due to SRAM usage.
  delay(3000);
  Serial.print(F("AT+CWMODE=3\r\n"));  //set to softAP+station mode
  delay(300);
  Serial.print(F("AT+CWSAP=\"Cokoino_ESP8266-01\",\"12345678\",11,2\r\n"));//wifiname:Cokoino_ESP8266-01,wifipassword:12345678
 //channnel:11 Encryption mode:2 ;Encryption mode should not set to 1,otherwise the wifi can't set successed
  delay(200);
  Serial.print(F("AT+CIPMUX=1\r\n"));//Enable multiple connections
  delay(200);
  Serial.print(F("AT+CIPSERVER=1,3001\r\n"));//Create the server. The default port is 333. Change the port to 3001, which is consistent with the APP
  delay(200);
  Serial.print(F("AT+CIPSTO=7000\r\n"));//Example Set the server timeout period to 7000 seconds
  delay(2000);
}
void judgement() {

  if (comdata.length() > 0) {
    comdata += "\n";  //This sentence must be added, otherwise the matched command character is one less, and the newline is used to assist in the complete match.
    char buf[comdata.length()];
    comdata.toCharArray(buf, comdata.length());
    ms.Target(buf);
    char result = ms.Match("%c*%+IPD, ?[0-9]+, ?[0-9]+: ?([^%c]+)%c*$");
    if (result > 0) {
      ms.GetCapture(buf, 0);
      comdata = String(buf);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(comdata);
      delay(100);
    } else {
      result = ms.Match("%c*%s?([0-9]),%s?([^%c]+)%c*$");  // esp8266 Multi-channel supports up to 5 connections (id:0-4)
      if (result > 0) {
        char buf0[1];  // esp8266 In multi-channel mode. id of the connection at this time
        ms.GetCapture(buf0, 0);
        ms.GetCapture(buf, 1);
        comdata = String(buf);
        if (comdata == "CONNECT")//The APP successfully connects to the wifi of ESP-01 module
        {
          String receiveOkMs = "Welcome to connect to ESP8266-01, ID: " + String(buf0) + " .";//A successful connection message is displayed
          Serial.println("AT+CIPSEND=" + String(buf0) + "," + receiveOkMs.length() + "\r\n");
          delay(10);
          Serial.print(receiveOkMs);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(String(buf0) + ",CONNECT ");
          delay(1500);
          lcd.setCursor(0, 1);
          lcd.print("MSG_Len:");
          lcd.setCursor(9, 1);
          lcd.print(String(receiveOkMs.length()) + "Bytes");  // If the combined variable is a non-string, it needs to be converted to a string for normal display.
          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Available Memory");
          lcd.setCursor(0, 1);
          //lcd.print(": " + String(availableMemory()));  // If the combined variable is a non-string, it needs to be converted to a string for normal display.
          delay(2000);
        }
      }

      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("WIFI Prepared!");
    }
    //comdata = "";
    //return; // When debugging communication with the APP, it needs to be commented out when normal use

     if (comdata == phone1) {
       judge = 1;
     }
      else if (comdata == phone1_5) {
       judge = 2;
     } else if (comdata == phone2) {
       judge = 3;
     } else if (comdata == phone2_5) {
       judge = 4;
     } else if (comdata == phone3) {
       judge = 5;
     } else if (comdata == phone3_5) {
       judge = 6;
     } else if (comdata == phone4) {
       judge = 7;
     } else if (comdata == phone4_5) {
       judge = 8;
     } else if (comdata == phone5) {
       judge = 9;
     } else if (comdata == phone6) {
       judge = 10;
     } else if (comdata == phone7) {
       judge = 11;
     } else if (comdata == phone8) {
       judge = 12;
     } else if (comdata == phone9) {
       judge = 13;
     } else if (comdata == phone10) {
       judge = 14;
     } else if (comdata == phone11) {
       judge = 15;
     } else if (comdata == phone12) {
       judge = 16;
     } else if (comdata == phone13) {
       judge = 17;
     } else if (comdata == phone14) {
       judge = 18;
     }
     else {
       judge = 9;
     }
    comdata = "";
  }

  switch (judge) {

    case 1:
      up1();
      delay(20);
      break;
    case 2:
      up2();
      delay(20);
      break;
    case 3:
      left();
      delay(20);
      break;
    case 4:
      up3();
      delay(20);
      break;
    case 5:
      down1();
      delay(20);
      break;
    case 6:
      down3();
      delay(20);
      break;
    case 7:
      right();
      delay(20);
      break;
    case 8:
      down2();
      delay(20);
      break;
    case 9:
      StopArm(); 
      break;
    case 10:
      reverserotation();
      //delay(1000); 
      //judge = 9;
      break; 
    case 11:
      forwardrotation();
      //delay(1000);
      //judge = 9;
      break; 
    //case 12:
      //track_line(); 
     // break; 
    //case 13:
     // obstacle_avoidance();
     // break;  
    //case 14:
      //light_show(); 
      //delay(2000);
      //judge = 9;
      //break;
    case 15:
      buzzer(); 
      delay(1000);
      break;  
    case 17:
      close(); 
      //delay(1000);
      //judge = 9;
      break; 
    case 18:
      open(); 
      //delay(1000);
      //judge = 9;
      break;   
    default: break;
  }
}

//pwm.setPWM(pwmnum,on, off);
     //(pwmnum, on, off) function is mainly to adjust the output PWM duty cycle.
     // Usually, on is set to 0 and off can be changed.
     // Because the PCA9685 is a 12-bit resolution
     // the value of 0 to 4096 off represents a duty cycle of 0 to 100.
void up1(void)
{
arm.up1(5);
}
void up2(void)
{
arm.up2(5);
}
void up3(void)
{
arm.up3(5);
}
void down1(void)
{
arm.down1(5);
}
void down2(void)
{
arm.down2(5);
}
void down3(void)
{
arm.down3(5);
}
void left(void)
{
arm.left(5);
}
void right(void)
{
arm.right(5);
}
void reverserotation(void)
{
arm.reverserotation(9);
}
void forwardrotation(void)
{
arm.forwardrotation(9);
}
void close(void)
{
arm.close(12);
}
void open(void)
{
arm.open(12);
}

void StopArm()
{
 arm.up1(0);
 arm.up2(0);
 arm.up3(0);
 arm.down1(0);
 arm.down2(0);
 arm.down3(0);
 arm.left(0);
 arm.right(0);
 arm.close(0);
 arm.open(0);
 arm.reverserotation(0);
 arm.forwardrotation(0);
}

///////////////////////////////////////////////////////////buzzer sounds
void buzzer(void)
{
 digitalWrite(buzzerPin,HIGH);
 delay(500);
 digitalWrite(buzzerPin,LOW);
 delay(100);
}


int availableMemory() {
  // Use 1024 with ATmega168
  int size = 2048;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL);
  free(buf);
  return size;
}

