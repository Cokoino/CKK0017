/*****************************************************
 * This code applies to Metal robotic arm Kit 
 * Through this link you can download the source code:
 * https://github.com/Cokoino/CKK0017
 
*****************************************************/
#include <SCoop.h> //Import multithread library
#include <PS2X_lib.h> //Import PS2 library

//Define the connection between PS2 and Arduino
#define PS2_DAT     13    
#define PS2_CMD     12  
#define PS2_SEL     11  
#define PS2_CLK     10  

#define pressures   false //Do not enable pressure sensitivity
#define rumble      false //Do not activate the motor
PS2X ps2x;  //create PS2 Controller Class
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte vibrate = 0;
#include "src/CokoinoArm.h"
#define buzzerPin A0

CokoinoArm arm;
int xL,yL,xR,yR;
const int act_max=10;//The default setting records 10 actions, which you can change according to your own needs, up to a maximum of 100
int act[act_max][6];//Support action memory for 6 servos
int num=0,num_do=0;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN A1
//LED Number
#define NUMPIXELS  4
#define BRIGHTNESS 5  // NeoPixel brightness, 0 (min) to 255 (max)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);// Required for 16 MHz Adafruit Trinket
#endif

defineTask(TaskOne);     // Create subthread 1
defineTask(TaskTwo);    // Create subthread 2

void TaskOne::setup() //Thread 1 setup
{
  pinMode(buzzerPin,OUTPUT);
  Serial.begin(57600);
  arm.ServoAttach(2,3,4,5,6,7);// 将转向舵机连接到数字234567脚
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  //手柄的针设置:clock, command, attention, data, Pressures, Rumble
  //禁用了压力灵敏度和震动反馈.
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
  }
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
 ps2x.enableRumble();              //enable rumble vibration motors
 ps2x.enablePressures();           //enable reading the pressure values from the buttons.  
 
}

void TaskTwo::setup(){    //Thread 2 setup
  //Serial.begin(57600);
  pixels.begin(); //初始化灯带
  pixels.show(); // Turn OFF all pixels ASAP
  pixels.setBrightness(BRIGHTNESS);
}

void setup(){
  mySCoop.start(); //Start multithreading
}

void TaskOne::loop()   //loop subthread 1
{
  if(error != 0)
  return; 
  ps2x.read_gamepad(); //至少每秒调用一次，以便从控制器获取数据.
   //Analogue Stick readings
  yR = ps2x.Analog(PSS_RY); //右侧摇杆上/下
  xR = ps2x.Analog(PSS_RX); //右侧摇杆左/右
  yL = ps2x.Analog(PSS_LY); //左侧摇杆上/下
  xL = ps2x.Analog(PSS_LX); //左侧摇杆左/右
  date_processing(&xL,&yL);
  date_processing(&xR,&yR);
  turnUD1();
  turnUD2();
  turnLR();
  turnCO();
  turnUD3();
  turnRotation();
  buzzer();
  C_action();
  Do_action();
}
///Servo1///
void turnLR(void){
  if(xL!=123){
    if(0<=xL && xL<=25){arm.right(15);return;}
    if(225<xL && xL<=256){arm.left(15);return;}  
    if(25<xL && xL<=50){arm.right(15);return;}
    if(200<xL && xL<=225){arm.left(15);return;}
    if(50<xL && xL<=75){arm.right(15);return;}
    if(175<xL && xL<=200){arm.left(15);return;}
    if(75<xL && xL<=100){arm.right(15);return;}
    if(150<xL && xL<=175){arm.left(15);return;}
    if(100<xL && xL<=120){arm.right(15);return;}
    if(125<xL && xL<=150){arm.left(15);return;}
  }
}
///////////////////////////////////////////////////////////////
///Servo2///
void turnUD1(void){
  if(yL!=123){
    if(0<=yL && yL<=25){arm.down1(15);delay(20);return;}
    if(225<yL && yL<=256){arm.up1(15);delay(20);return;} 
    if(25<yL && yL<=50){arm.down1(15);delay(20);return;}
    if(200<yL && yL<=225){arm.up1(15);delay(20);return;}
    if(50<yL && yL<=75){arm.down1(15);delay(20);return;}
    if(175<yL && yL<=200){arm.up1(15);delay(20);return;}
    if(75<yL && yL<=100){arm.down1(15);delay(20);return;}
    if(150<yL && yL<=175){arm.up1(15);delay(20);return;}
    if(100<yL && yL<=122){arm.down1(15);delay(20);return;}
    if(123<yL && yL<=150){arm.up1(15);delay(20);return;} 
  }
}
///////////////////////////////////////////////////////////////
///Servo3///
void turnUD2(void){
  if(yR!=123){
    if(0<=yR && yR<=25){arm.up2(10);delay(15);return;}
    if(225<yR && yR<=256){arm.down2(10);delay(15);return;} 
    if(25<yR && yR<=50){arm.up2(10);delay(15);return;}
    if(200<yR && yR<=225){arm.down2(10);delay(15);return;}
    if(50<yR && yR<=75){arm.up2(10);delay(15);return;}
    if(175<yR && yR<=200){arm.down2(10);delay(15);return;}
    if(75<yR && yR<=100){arm.up2(10);delay(15);return;}
    if(150<yR && yR<=175){arm.down2(10);delay(15);return;}
    if(100<yR && yR<=122){arm.up2(10);delay(15);return;}
    if(123<yR && yR<=150){arm.down2(10);delay(15);return;} 
  }
}
///////////////////////////////////////////////////////////////
///Servo4///
void turnUD3(void){
 if(ps2x.Button(PSB_PAD_UP)) {
    arm.up3(10);
    return;
  }else if(ps2x.ButtonReleased(PSB_PAD_UP))  {
    arm.up3(0);
    return;
  }
  if(ps2x.Button(PSB_PAD_DOWN)) {
    arm.down3(10);
    return;
  }else if(ps2x.ButtonReleased(PSB_PAD_DOWN))  {
    arm.down3(0);
    return;
  }
}
///////////////////////////////////////////////////////////////
///Servo5///
void turnRotation(void){
 if(ps2x.Button(PSB_PAD_RIGHT)) {
    arm.forwardrotation(10);
    return;
  }else if(ps2x.ButtonReleased(PSB_PAD_RIGHT))  {
    arm.forwardrotation(0);
    return;
  }
  if(ps2x.Button(PSB_PAD_LEFT)) {
    arm.reverserotation(10);
    return;
  }else if(ps2x.ButtonReleased(PSB_PAD_LEFT))  {
    arm.reverserotation(0);
    return;
  }
}
///////////////////////////////////////////////////////////////
///Servo6///
void turnCO(void){
  if(xR!=123){
    if(0<=xR && xR<=25){arm.close(0);delay(20);return;}
    if(225<xR && xR<=256){arm.open(0);delay(20);return;} 
    if(25<xR && xR<=50){arm.close(5);delay(20);return;}
    if(200<xR && xR<=225){arm.open(5);delay(20);return;}
    if(50<xR && xR<=75){arm.close(5);delay(20);return;}
    if(175<xR && xR<=200){arm.open(5);delay(20);return;}
    if(75<xR && xR<=100){arm.close(5);delay(20);return;}
    if(150<xR && xR<=175){arm.open(5);delay(20);return;}
    if(100<xR && xR<=120){arm.close(5);delay(20);return;}
    if(125<xR && xR<=150){arm.open(5);delay(20);return;} 
  }
}
///////////////////////////////////////////////////////////////
void date_processing(int *x,int *y){
  if(abs(123-*x)>abs(123-*y))
    {*y = 123;}
  else
    {*x = 123;}
}
///////////////////////////////////////////////////////////////
void buzzer(void)
{
  if(ps2x.Button(PSB_SQUARE) || ps2x.Button(PSB_CIRCLE)) {
  digitalWrite(buzzerPin,HIGH);
  return;
  }else if(ps2x.ButtonReleased(PSB_SQUARE) || ps2x.ButtonReleased(PSB_CIRCLE)) {
     digitalWrite(buzzerPin,LOW);
    return;
  }
}

///////////////////////////////////////////////////////////////
void C_action(void){
  if(ps2x.ButtonPressed(PSB_TRIANGLE)){
    int *p;
    p=arm.captureAction();
    for(char i=0;i<6;i++){
    act[num][i]=*p;
    p=p+1;     
    }
    num++;
    num_do=num;
    if(num>=act_max){
      num=0;
      digitalWrite(buzzerPin,HIGH);
      delay(300);
      digitalWrite(buzzerPin,LOW);
      delay(300);        
      }
    //Serial.println(act[0][0]);
  }
}
///////////////////////////////////////////////////////////////
void Do_action(void){
  if(ps2x.ButtonPressed(PSB_CROSS)){
      digitalWrite(buzzerPin,HIGH);
      delay(300);
      digitalWrite(buzzerPin,LOW);
      delay(300);   
      digitalWrite(buzzerPin,HIGH);
      delay(300);
      digitalWrite(buzzerPin,LOW);
      delay(300);             
    for(int i=0;i<num_do;i++){
      arm.do_action(act[i],15);
      }
    num=0;
      digitalWrite(buzzerPin,HIGH);
      delay(3000);
      digitalWrite(buzzerPin,LOW);
      delay(50);        
  }
}

 void TaskTwo::loop()  //loop subthread 2
{
  for(int i=0;i<=NUMPIXELS;i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,200,200)); //颜色设置
    pixels.show(); // 更新灯带显示
    delay(500); 
  }


  for(int i=NUMPIXELS;i>=0;i--)
  {
    pixels.setPixelColor(i, pixels.Color(0,0,0)); //颜色设置
    pixels.show(); // 更新灯带显示
    delay(500);
  }


  for(int i=1;i<=12;i++)
  {
    //红
    pixels.setPixelColor( i , pixels.Color(255,0,0));
    pixels.setPixelColor(i+1, pixels.Color(255,0,0));
    //黄
    pixels.setPixelColor(i+2, pixels.Color(255,255,0));
    pixels.setPixelColor(i+3, pixels.Color(255,255,0));
    //绿
    pixels.setPixelColor(i+4, pixels.Color(0,255,0));
    pixels.setPixelColor(i+5, pixels.Color(0,255,0));
    //蓝
    pixels.setPixelColor(i+6, pixels.Color(0,0,255));
    pixels.setPixelColor(i+7, pixels.Color(0,0,255));
    //青
    pixels.setPixelColor(i+8, pixels.Color(0,255,255));
    pixels.setPixelColor(i+9, pixels.Color(0,255,255));
    //紫
    pixels.setPixelColor(i+10, pixels.Color(255,0,255));
    pixels.setPixelColor(i+11, pixels.Color(255,0,255));
    //紫
    pixels.setPixelColor(i-1, pixels.Color(255,0,255));
    pixels.setPixelColor(i-2, pixels.Color(255,0,255));
    //青
    pixels.setPixelColor(i-3, pixels.Color(0,255,255));
    pixels.setPixelColor(i-4, pixels.Color(0,255,255));
    //蓝
    pixels.setPixelColor(i-5, pixels.Color(0,0,255));
    pixels.setPixelColor(i-6, pixels.Color(0,0,255));
    //绿
    pixels.setPixelColor(i-7, pixels.Color(0,255,0));
    pixels.setPixelColor(i-8, pixels.Color(0,255,0));
    //黄
    pixels.setPixelColor(i-9, pixels.Color(255,255,0));
    pixels.setPixelColor(i-10, pixels.Color(255,255,0));
    //红
    pixels.setPixelColor(i-11, pixels.Color(255,0,0));
    //刷新颜色
    pixels.show();
    delay(500);
    }
    
}

void loop(){
  yield();  //loop Multithreaded task
}
