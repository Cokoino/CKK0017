#include <Servo.h>  //引入servo库
#include <PS2X_lib.h> // 引入PS2X库
/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false
PS2X ps2x;  //create PS2 Controller Class
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

Servo SteeringServo;  //创建表示转向舵机的对象
Servo ServoN1;  //创建表示舵机1的对象
Servo ServoN2;  //创建表示舵机2的对象
int PlyStnRStickUpDn = 0;  //读取PS2右侧摇杆向上/向下的值.
int PlyStnRStickLtRt = 0;  //读取PS2右侧摇杆向左/向右的值. 
int PlyStnLStickUpDn = 0; //读取PS2左侧摇杆向上/向下的值.
int PlyStnLStickLtRt = 0; //读取PS2左侧摇杆向左/向右的值.
int StrServoSetting = 90; //转向舵机的角度设置
int ServoN1Setting = 90; //舵机1的角度设置
int ServoN2Setting = 90; //舵机2的角度设置

void setup()
{
  Serial.begin(57600);
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  ps2x.config_gamepad(10,11,12,13, false, false);
  //手柄的针设置:clock, command, attention, data, Pressures, Rumble
  //禁用了压力灵敏度和震动反馈.
  SteeringServo.attach(2);// 将转向舵机连接到数字7脚
  ServoN1.attach(3);// 将舵机1连接到数字8脚
  ServoN2.attach(4);// 将舵机2连接到数字8脚
  //将舵机置中90度.
  SteeringServo.write(90);
  ServoN1.write(90);
  ServoN2.write(90);
 error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}
void loop()
{
  ps2x.read_gamepad(); //至少每秒调用一次，以便从控制器获取数据.
 
//Analogue Stick readings
  PlyStnRStickUpDn = ps2x.Analog(PSS_RY); //右侧摇杆上/下
  PlyStnRStickLtRt = ps2x.Analog(PSS_RX); //右侧摇杆左/右
  PlyStnLStickUpDn = ps2x.Analog(PSS_LY); //左侧摇杆上/下
  PlyStnLStickLtRt = ps2x.Analog(PSS_LX); //左侧摇杆左/右
//PS2控制器的读数从0到255，中性点是128.对于x轴运动，0点在左边，对于y轴运动，0点在上面.
//变量，用于存储ESCs和Servos的值. 
//PS2摇杆的值将映射到0到180度.
 if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) {
  StrServoSetting = map(PlyStnRStickLtRt,0,256,0,179);
  ServoN1Setting = map(PlyStnLStickUpDn,0,256,0,179);
  ServoN2Setting = map(PlyStnLStickLtRt,0,256,0,179);
//将值写入到Servos
  SteeringServo.write(StrServoSetting);
  ServoN1.write(ServoN1Setting);
  ServoN2.write(ServoN2Setting);
  delay(100);
 }
}