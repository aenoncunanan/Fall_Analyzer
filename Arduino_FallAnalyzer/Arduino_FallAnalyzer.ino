// Import the Libraries needed
#include "CurieIMU.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define Pins
#define OLED_CLK  12  //D0
#define OLED_MOSI 11  //D1
#define OLED_RESET  10  //RES
#define OLED_DC   9 //DC
#define OLED_CS   8 //CS

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#if (SSD1306_LCDHEIGHT != 64)

#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int lastOrientation = - 1;  // Previous orientation (for comparison)

void setup() {
// Start Serial Monitor
  Serial.begin(9600);
  delay(2000);
  
// Initialize the screen
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  
// Initialize the display
  Serial.println("Initializing display...");
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
// Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  
// Prompt a welcome message  
  Serial.println("Device is ready!"); 
  display.setCursor(20,30);
  display.print("Device is ready!");
  display.setCursor(0,50);
  display.print("Ambion Cunanan Harris");
  display.display();
  delay(3000);
}

  /*
  The orientations of the board:
  0: flat, processor facing up
  1: flat, processor facing down
  2: landscape, analog pins down
  3: landscape, analog pins up
  4: portrait, USB connector up
  5: portrait, USB connector down
  */
  
  /*
    The orientation of the screen:
    0: default
    1: 90 degrees CCW
    2: 180 CW
    3: 90 degress CW
  */

void loop() {
  display.clearDisplay();   //clear display
  int orientation = - 1;    // the board's orientation
  String orientationString; // string for printing description of orientation

  // Read accelerometer:
  int x = CurieIMU.readAccelerometer(X_AXIS);
  int y = CurieIMU.readAccelerometer(Y_AXIS);
  int z = CurieIMU.readAccelerometer(Z_AXIS);

  // Calculate the absolute values, to determine the largest
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);

  if ( (absZ > absX) && (absZ > absY)) {
    // base orientation on Z
    if (z > 0) {
      orientationString = "up";
      orientation = 0;
      display.clearDisplay();
      display.display();
 
    } else {
      orientationString = "down";
      orientation = 1;
      display.clearDisplay();
      display.display();
    }
  } else if ( (absY > absX) && (absY > absZ)) {
    // base orientation on Y
    if (y > 0) {
      orientationString = "digital pins up / In Sitting Position!";
      orientation = 2;
      display.setRotation(0);
      displaySitting();       
    } else {
      orientationString = "analog pins up / In Sitting Position";
      orientation = 3;  
      display.setRotation(2);
      displaySitting();
    }
  } else {
    // base orientation on X
    if (x < 0) {
      orientationString = "connector up / In Standing Position";
      orientation = 4;
      display.setRotation(3);
      displayStanding(); 
    } else {
      orientationString = "connector down / In Standing Position";
      orientation = 5;
      display.setRotation(1);      
      displayStanding();          
    }
  }

  // if the orientation has changed, print out a description:
  if (orientation != lastOrientation) {
    Serial.println(orientationString);
    lastOrientation = orientation;
  }
}

void displaySitting() {
  display.setCursor(35,20);
  display.println("IN SITTING");
  display.setCursor(40,40);
  display.println("POSITION!");
  display.display();
}

void displayStanding() {
  display.setCursor(20,20);
  display.println("IN");
  display.setCursor(6,40);
  display.println("STANDING");
  display.setCursor(6,60);
  display.println("POSITION!");
  display.display();  
}
