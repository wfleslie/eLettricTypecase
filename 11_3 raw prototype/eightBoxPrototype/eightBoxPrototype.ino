/* 
10/28/16
Eight box prototype, one multiplexer

*/

//NeoPixel info:
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 3 //pin for DIN on NeoPixel
#define NUMPIXELS 8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//End Neopixel info

//LCD info:
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int x = 0;
int y = 0;
//End LCD info

int threshold = 200; //Light level to trigger event
int resetVal = 220; //Light level to return to before triggering next event
                    //Currently broken cuz e light is blue only, should be a higher value
boolean fingerIN = false; //prevent multiple iterations without removing finger

int muxValues[] = {0,0,0,0,0,0,0,0,}; //set 8 values for multiplexer

void setup()
{
    lcd.begin(16, 2); //set up the LCD's number of columns and rows
    pixels.begin(); // Initialize NeoPixel library
    Serial.begin(9600);  //Begin serial communcation

    //4051 digital control pins
    pinMode(4, OUTPUT);    // s0
    pinMode(5, OUTPUT);    // s1
    pinMode(6, OUTPUT);    // s2
    
}

void readData() //separate function to print to LCD
  {
    if (fingerIN == false) {
      if (muxValues[0] < threshold) {
         lcd.print("H");
         x = x + 1;
         fingerIN = true;
      } else if (muxValues[1] < threshold) {
          lcd.print("!");
          x = x + 1;
          fingerIN = true;
      } else if (muxValues[2] < threshold) {
          lcd.print("l");
          x = x + 1;
          fingerIN = true; 
      } else if (muxValues[3] < threshold) {
          lcd.print("b");
          x = x + 1;
          fingerIN = true;
      } else if (muxValues[4] < threshold) {
          lcd.print("e");
          x = x + 1;
          fingerIN = true;
      } else if (muxValues[5] < threshold) {
          lcd.print("a");
          x = x + 1;
          fingerIN = true;
      } else if (muxValues[6] < threshold) {
          lcd.print("r");
          x = x + 1;
          fingerIN = true;
      }else if (muxValues[7] < threshold) {
          lcd.print("n");
          x = x + 1;
          fingerIN = true;
      }
       } else if (fingerIN == true) {
        if (muxValues[0] > resetVal
            && muxValues[1] > resetVal
            && muxValues[2] > resetVal
            && muxValues[3] > resetVal
            && muxValues[4] > resetVal
            && muxValues[5] > resetVal
            && muxValues[6] > resetVal
            && muxValues[7] > resetVal
           ) {
          fingerIN = false;
        } else  {
          fingerIN = true;
        } 
      }
  }
  
void displayData() //separate function to print to serial monitor
  {  //Serial.println(fingerIN);
    for(int i = 0; i < 8; i++) {
      Serial.print("Pin");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(muxValues[i]);   
   }
    Serial.println("====================");
  }


void loop()
{
   //LCD specific coding. Can remove with different output.
   if (x > 15) {
     if (y == 0) {
       x = 0;
       y = 1;
     } else if (y == 1) {
       lcd.clear();
       x = 0;
       y = 0;
     }
   }
   lcd.setCursor(x,y);
   //End of LCD specific.

    for(int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(255,255,255)); //to make all white
    }
 /* //To make individual colors
    pixels.setPixelColor(0, pixels.Color(255,255,255));
    pixels.setPixelColor(1, pixels.Color(255,255,0));
    pixels.setPixelColor(2, pixels.Color(0,255,255));
    pixels.setPixelColor(3, pixels.Color(255,0,255));
    pixels.setPixelColor(4, pixels.Color(255,255,255));
    pixels.setPixelColor(5, pixels.Color(50,50,255));
    pixels.setPixelColor(6, pixels.Color(255,0,0));
    pixels.setPixelColor(7, pixels.Color(0,255,0)); */
    pixels.show();

     //Read Value of 4051 analog-in 0 by setting the values of s0,s1 and s2
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    muxValues[0] = analogRead(0); // read input pin, store in muxValues

    //repeat to read other pins (in this case analog in 1)
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    muxValues[1] = analogRead(0); // read input pin, store in muxValues

    //Analog 2
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    muxValues[2] = analogRead(0); // read input pin, store in muxValues

    //Analog 3
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    muxValues[3] = analogRead(0); // read input pin, store in muxValues
 
    //Analog 4
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    muxValues[4] = analogRead(0); // read input pin, store in muxValues

    //Analog 5
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    muxValues[5] = analogRead(0); // read input pin, store in muxValues

    //Analog 6
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    muxValues[6] = analogRead(0); // read input pin, store in muxValues

    //Analog 7
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    muxValues[7] = analogRead(0); // read input pin, store in muxValues

   readData(); 
   displayData();
 
   //delay(1000); //slow down serial monitor
}
