/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  2 pins are required to interface (no pin for reset)

  Distance measurement with five Parallax #28015 sensor
  PING))) Ultrasonic Range Finder Rev D (3 pins)

  save measurement date in cm array[0 .. 4]


  (c) Kapeller Alexander, 06.02.2023
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// index for sensors
const int maxindex = 5;

// this constant won't change. It's the pin number of the sensor's output:
const int pingPin[] = {14, 14, 14, 14, 14};

// x, y positions for display
const int xpos[] = {5, 30, 58, 86, 110};
const int ypos[] = {40, 30, 20, 30, 40};

void setup()   {

  Serial.begin(9600);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  delay(2000);

  // clear the buffer.
  display.clearDisplay();

  // set display options
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();

}


void loop() {
  // establish variables for duration of the ping, and the distance result
  long duration[maxindex];
  long cm[maxindex];
  

  // sensor readings
    for(int index=0; index<maxindex;index++){
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(pingPin[index], OUTPUT);
    digitalWrite(pingPin[index], LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin[index], HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin[index], LOW);

   // The same pin is used to read the signal from the PING))): a HIGH pulse
   // whose duration is the time (in microseconds) from the sending of the ping
   // to the reception of its echo off of an object.
    pinMode(pingPin[index], INPUT);
    duration[index] = pulseIn(pingPin[index], HIGH);
    cm[index] = microsecondsToCentimeters(duration[index]);
    delay(20);
  }
 
  //serial monitor sensorvalue in cm  
  for(int index=0; index<maxindex;index++){
    Serial.print(index);
  Serial.print(" ");
  Serial.print(cm[index]);
  Serial.print("  ");
  }
  Serial.println();

  //display Headline
  display.clearDisplay();
  display.setCursor(5,0);
  display.print("alle Messwerte in cm");
  
  // display sensorvalue in cm
  for(int index=0; index<maxindex;index++){
    display.setCursor(xpos[index],ypos[index]);
    if(cm[index]<100) display.print(" ");
    if(cm[index]<10) display.print(" ");
    display.print(cm[index]);
  }

  /*
  // sensor C
  display.setCursor(58,20);
  if(cm[2]<100) display.print(" ");
  if(cm[2]<10) display.print(" ");
  display.print(cm[2]);
  
  // sensor B
  display.setCursor(30,30);
  if(cm[1]<100) display.print(" ");
  if(cm[1]<10) display.print(" ");
  display.print(cm[1]);

   // sensor D
  display.setCursor(86,30);
  if(cm[3]<100) display.print(" ");
  if(cm[3]<10) display.print(" ");
  display.print(cm[3]);

  // sensor A
  display.setCursor(5,40);
  if(cm[0]<100) display.print(" ");
  if(cm[0]<10) display.print(" ");
  display.print(cm[0]);

   // sensor E
  display.setCursor(110,40);
  if(cm[4]<100) display.print(" ");
  if(cm[4]<10) display.print(" ");
  display.print(cm[4]);
  */

  display.display();
  //delay(50);

}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


