#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <PS2X_lib.h>//for 1.4

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte vibrate = 0;

void setup(){
 Serial.begin(57600);

  
 error = ps2x.config_gamepad(10,12,11,13);   //setup GamePad(clock, command, attention, data) pins, check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons");
   Serial.println("holding L1 or R1 will print out the analog stick values.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands");
   
   //Serial.print(ps2x.Analog(1), HEX);
 
 
 ps2x.enableRumble();              //enable rumble vibration motors
 ps2x.enablePressures();           //enable reading the pressure values from the buttons. 
  
 lcd.init(); // initialize the lcd 
 lcd.backlight();
 lcd.clear();
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
 if(error != 0)
  return; 
  
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
  
  if(ps2x.Button(PSB_START))  {               //will be TRUE as long as button is pressed
       Serial.println("Start is being held");
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("Start");
       }
  if(ps2x.Button(PSB_SELECT)){
       Serial.println("Select is being held");
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("Select");
       }
  if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.println("UP is being held");
       lcd.clear();
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("UP");
       }
  if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.println("RIGHT is being held");
       lcd.clear();
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("RIGHT");
    }
  if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.println("LEFT is being held");
       lcd.clear();
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("LEFT");
    }
  if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.println("DOWN is being held");
       lcd.clear();
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("DOWN");
    }   
  if(ps2x.Button(PSB_TRIANGLE)){
       Serial.println("TRIANGLE is being held");
       lcd.clear();
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("TRIANGLE");
    }   
   
  if(ps2x.Button(PSB_CROSS)){
       Serial.println("CROSS is being held");
       lcd.clear();
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("CROSS");
    }   
  if(ps2x.Button(PSB_SQUARE)){
       Serial.println("SQUARE is being held");
       lcd.clear();
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("SQUARE");
    }   
  if(ps2x.Button(PSB_CIRCLE)){
       Serial.println("CIRCLE is being held");
       lcd.clear();
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("CIRCLE");
    }   
  
  if(ps2x.Button(PSB_L2)){
       Serial.println("L2 is being held");
       lcd.clear();
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("L2");
    }   
  if(ps2x.Button(PSB_R2)){
       Serial.println("R2 is being held");
       lcd.clear();
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("R2");
    }        
  if(ps2x.Button(PSB_L1)){
       Serial.println("L1 is being held");
       lcd.clear();
       Serial.print("Stick Values:");
       Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("LY=");
       lcd.setCursor(6,0);//set the line and the char begaining for the print information
       lcd.print(ps2x.Analog(PSS_LY), DEC);
       Serial.print(",");
       Serial.print(ps2x.Analog(PSS_LX), DEC); 
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("LX=");
       lcd.setCursor(6,1);//set the line and the char begaining for the print information
       lcd.print(ps2x.Analog(PSS_LX), DEC);
    }   
   if(ps2x.Button(PSB_R1)){
       Serial.println("R1 is being held");
       lcd.clear();
       Serial.print("Stick Values:");
       Serial.print(ps2x.Analog(PSS_RY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
       lcd.setCursor(0,0);//set the line and the char begaining for the print information
       lcd.print("RY=");
       lcd.setCursor(6,0);//set the line and the char begaining for the print information
       lcd.print(ps2x.Analog(PSS_RY), DEC);
       Serial.print(",");
       Serial.print(ps2x.Analog(PSS_RX), DEC); 
       lcd.setCursor(0,1);//set the line and the char begaining for the print information
       lcd.print("RX=");
       lcd.setCursor(6,1);//set the line and the char begaining for the print information
       lcd.print(ps2x.Analog(PSS_RX), DEC);
    }   
  
 delay(50);
    
    
     
}