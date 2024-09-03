#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define NUMPIXELS  12 //led QTY
#define BRIGHTNESS 5  // NeoPixel brightness, 0 (min) to 255 (max)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);// Required for 16 MHz Adafruit Trinket
#endif

void setup() 
{
  Serial.begin(115200);
  pixels.begin(); //Initialize the light strip
  pixels.show(); // Turn OFF all pixels ASAP
  pixels.setBrightness(BRIGHTNESS);
}

void loop() 
{ 

  for(int i=0;i<=NUMPIXELS;i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,200,200)); //setup color
    pixels.show(); // update display
    delay(500); 
  }


  for(int i=NUMPIXELS;i>=0;i--)
  {
    pixels.setPixelColor(i, pixels.Color(0,0,0)); //setup color
    pixels.show(); // update display
    delay(500);
  }


  for(int i=1;i<=12;i++)
  {
    //red
    pixels.setPixelColor( i , pixels.Color(255,0,0));
    pixels.setPixelColor(i+1, pixels.Color(255,0,0));
    //yellow
    pixels.setPixelColor(i+2, pixels.Color(255,255,0));
    pixels.setPixelColor(i+3, pixels.Color(255,255,0));
    //green
    pixels.setPixelColor(i+4, pixels.Color(0,255,0));
    pixels.setPixelColor(i+5, pixels.Color(0,255,0));
    //blue
    pixels.setPixelColor(i+6, pixels.Color(0,0,255));
    pixels.setPixelColor(i+7, pixels.Color(0,0,255));
    //light blue
    pixels.setPixelColor(i+8, pixels.Color(0,255,255));
    pixels.setPixelColor(i+9, pixels.Color(0,255,255));
    //purple
    pixels.setPixelColor(i+10, pixels.Color(255,0,255));
    pixels.setPixelColor(i+11, pixels.Color(255,0,255));
    //purple
    pixels.setPixelColor(i-1, pixels.Color(255,0,255));
    pixels.setPixelColor(i-2, pixels.Color(255,0,255));
    //light blue
    pixels.setPixelColor(i-3, pixels.Color(0,255,255));
    pixels.setPixelColor(i-4, pixels.Color(0,255,255));
    //blue
    pixels.setPixelColor(i-5, pixels.Color(0,0,255));
    pixels.setPixelColor(i-6, pixels.Color(0,0,255));
    //green
    pixels.setPixelColor(i-7, pixels.Color(0,255,0));
    pixels.setPixelColor(i-8, pixels.Color(0,255,0));
    //yellow
    pixels.setPixelColor(i-9, pixels.Color(255,255,0));
    pixels.setPixelColor(i-10, pixels.Color(255,255,0));
    //red
    pixels.setPixelColor(i-11, pixels.Color(255,0,0));
    //Refresh Color
    pixels.show();
    delay(500);
    }   
}



