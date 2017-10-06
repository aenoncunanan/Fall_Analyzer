// Import the Libraries needed
#include "CurieIMU.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

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
  
// Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  
// Prompt a welcome message  
  Serial.println("Device is ready!"); 
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

void loop() {
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
 
    } else {
      orientationString = "down";
      orientation = 1;
    }
  } else if ( (absY > absX) && (absY > absZ)) {
    // base orientation on Y
    if (y > 0) {
      orientationString = "digital pins up";
      orientation = 2;      
    } else {
      orientationString = "analog pins up";
      orientation = 3;  
    }
  } else {
    // base orientation on X
    if (x < 0) {
      orientationString = "connector up";
      orientation = 4;
    } else {
      orientationString = "connector down";
      orientation = 5;              
    }
  }

  // if the orientation has changed, print out a description:
  if (orientation != lastOrientation) {
    Serial.println(orientationString);
    lastOrientation = orientation;
  }
}
