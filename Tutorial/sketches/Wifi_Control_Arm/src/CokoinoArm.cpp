#include"CokoinoArm.h"
CokoinoArm::CokoinoArm(){}
void CokoinoArm::ServoAttach(uint8_t servo1_pin,uint8_t servo2_pin,uint8_t servo3_pin,uint8_t servo4_pin,uint8_t servo5_pin,uint8_t servo6_pin)
{
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  servo3.attach(servo3_pin);
  servo4.attach(servo4_pin);
  servo5.attach(servo5_pin);
  servo6.attach(servo6_pin);
  angle[0]=servo1.read();
  angle[1]=servo2.read();
  angle[2]=servo3.read();
  angle[3]=servo4.read();
  angle[4]=servo5.read();
  angle[5]=servo6.read();
}

/////////////////////////////////////////////////////////
void CokoinoArm::left(int speed)
{
  angle[0]=servo1.read();
  angle[0]=angle[0]+1;
  if(angle[0]>=180)
    {angle[0] = 180;}
  servo1.write(angle[0]);
  delay(speed);       
}
/////////////////////////////////////////////////////////
void CokoinoArm::right(int speed)
{
  angle[0]=servo1.read();
  angle[0]=angle[0]-1;
  if(angle[0]<=0)
    {angle[0] =0;}
  servo1.write(angle[0]);
  delay(speed);    
}
/////////////////////////////////////////////////////////
void CokoinoArm::up1(int speed)
{
  angle[1]=servo2.read();
  angle[1]=angle[1]+1;
  if(angle[1]>=180)
    {angle[1]= 180;}
  servo2.write(angle[1]);
  delay(speed);      
}
/////////////////////////////////////////////////////////
void CokoinoArm::down1(int speed)
{
  angle[1]=servo2.read();
  angle[1]=angle[1]-1;
  if(angle[1]<=0)
    {angle[1] = 0;}
  servo2.write(angle[1]);
  delay(speed);    
}
/////////////////////////////////////////////////////////
void CokoinoArm::up2(int speed)
{
  angle[2]=servo3.read();
  angle[2]=angle[2]+1;
  if(angle[2]>=180)
    {angle[2]= 180;}
  servo3.write(angle[2]);
  delay(speed);      
}
/////////////////////////////////////////////////////////
void CokoinoArm::down2(int speed)
{
  angle[2]=servo3.read();
  angle[2]=angle[2]-1;
  if(angle[2]<=0)
    {angle[2] = 0;}
  servo3.write(angle[2]);
  delay(speed);    
}
/////////////////////////////////////////////////////////
void CokoinoArm::up3(int speed)
{
  angle[3]=servo4.read();
  angle[3]=angle[3]+1;
  if(angle[3]>=180)
    {angle[3]= 180;}
  servo4.write(angle[3]);
  delay(speed);      
}
/////////////////////////////////////////////////////////
void CokoinoArm::down3(int speed)
{
  angle[3]=servo4.read();
  angle[3]=angle[3]-1;
  if(angle[3]<=0)
    {angle[3] = 0;}
  servo4.write(angle[3]);
  delay(speed);    
}
/////////////////////////////////////////////////////////
void CokoinoArm::forwardrotation(int speed)
{
  angle[4]=servo5.read();
  angle[4]=angle[4]+1;
  if(angle[4]>=180)
    {angle[4]= 180;}
  servo5.write(angle[4]);
  delay(speed);      
}
/////////////////////////////////////////////////////////
void CokoinoArm::reverserotation(int speed)
{
  angle[4]=servo5.read();
  angle[4]=angle[4]-1;
  if(angle[4]<=0)
    {angle[4] = 0;}
  servo5.write(angle[4]);
  delay(speed);    
}
/////////////////////////////////////////////////////////
void CokoinoArm::open(int speed)
{
  angle[5]=servo6.read();
  angle[5]=angle[5]+1;
  if(angle[5]>=180)
    {angle[5] =180;}
  servo6.write(angle[5]);
  delay(speed);     
}
/////////////////////////////////////////////////////////
void CokoinoArm::close(int speed)
{
  angle[5]=servo6.read();
  angle[5]=angle[5]-1;
  if(angle[5]<=0)
    {angle[5] =0;}
  servo6.write(angle[5]);
  delay(speed);     
}
/////////////////////////////////////////////////////////
int *CokoinoArm::captureAction(void)
{
  return angle;	
}
/////////////////////////////////////////////////////////
void CokoinoArm::do_action(int *angle,int speed)
{
  int count=0;
  int T_angle[6];
  int S_angle[6];
  for(int i=0;i<6;i++){
	T_angle[i]= *angle;
	angle++;
    }
  S_angle[0]=servo1.read();
  S_angle[1]=servo2.read();
  S_angle[2]=servo3.read();
  S_angle[3]=servo4.read();
  S_angle[4]=servo5.read();
  S_angle[5]=servo6.read();
  do{
	S_angle[0] = (T_angle[0]>S_angle[0])?S_angle[0]+1:S_angle[0]-1;
	servo1.write(S_angle[0]);
    S_angle[1] = (T_angle[1]>S_angle[1])?S_angle[1]+1:S_angle[1]-1;
    servo2.write(S_angle[1]);
	S_angle[2] = (T_angle[2]>S_angle[2])?S_angle[2]+1:S_angle[2]-1;
	servo3.write(S_angle[2]);
	S_angle[3] = (T_angle[3]>S_angle[3])?S_angle[3]+1:S_angle[3]-1;
	servo4.write(S_angle[3]);
	S_angle[4] = (T_angle[4]>S_angle[4])?S_angle[4]+1:S_angle[4]-1;
	servo5.write(S_angle[4]);
	S_angle[5] = (T_angle[5]>S_angle[5])?S_angle[5]+1:S_angle[5]-1;
	servo6.write(S_angle[5]);
	count++;
	delay(speed);
  }while(count<180);
  delay(speed*20);
}




