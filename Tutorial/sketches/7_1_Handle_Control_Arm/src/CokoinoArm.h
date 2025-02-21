/*
 * This code applies to cokoino mechanical arm
 * Through this link you can download the source code:
 * https://github.com/Cokoino/CKK0006
 * Company web site:
 * http://cokoino.com/
 */
#ifndef COKOINOARM_H_
#define COKOINOARM_H_ 
#include"CokoinoServo.h"
#include <Arduino.h>

class CokoinoArm
{
 public:
     CokoinoArm();
     void ServoAttach(uint8_t servo1_pin,uint8_t servo2_pin,uint8_t servo3_pin,uint8_t servo4_pin,uint8_t servo5_pin,uint8_t servo6_pin);
     void up1(int speed);
     void down1(int speed);
     void up2(int speed);
     void down2(int speed);
     void up3(int speed);
     void down3(int speed);
     void forwardrotation(int speed);
     void reverserotation(int speed);
     void left(int speed);
     void right(int speed);
     void open(int speed);
     void close(int speed);
	 int *captureAction(void);
	 void do_action(int *angle,int speed);
 //private:
     int angle[6];
     CokoinoServo servo1;
     CokoinoServo servo2;
     CokoinoServo servo3;
     CokoinoServo servo4; 
     CokoinoServo servo5;
     CokoinoServo servo6; 
     
};
#endif
