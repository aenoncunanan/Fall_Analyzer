// Import the Libraries needed
#include "CurieIMU.h"
#include "SPI.h"
#include "Wire.h"

int mainLastOrient = - 1;  // Previous orientation (for comparison)

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
  int mainOrientation = - 1;    // the board's orientation
  String mainOrientationString; // string for printing description of orientation

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
      mainOrientationString = "up";
      mainOrientation = 0;
 
    } else {
      mainOrientationString = "down";
      mainOrientation = 1;
    }
  } else if ( (absY > absX) && (absY > absZ)) {
    // base orientation on Y
    if (y > 0) {
      mainOrientationString = "digital pins up";
      mainOrientation = 2;      
    } else {
      mainOrientationString = "analog pins up";
      mainOrientation = 3;  
    }
  } else {
    // base orientation on X
    if (x < 0) {
      mainOrientationString = "connector up";
      mainOrientation = 4;
    } else {
      mainOrientationString = "connector down";
      mainOrientation = 5;              
    }
  }

  // if the orientation has changed, print out a description:
  if (mainOrientation != mainLastOrient) {
    Serial.println(mainOrientationString);
    mainLastOrient = mainOrientation;
  }
}
