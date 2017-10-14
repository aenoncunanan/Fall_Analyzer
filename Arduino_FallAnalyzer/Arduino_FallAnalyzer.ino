// Import the Libraries needed
#include "CurieIMU.h"
#include "SPI.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int ax1,ay1,az1;
int gx1,gy1,gz1;

int mainLastOrient = - 1;  // Previous orientation (for comparison)
int extLastOrient = -1; //Previous orientation of external Accel

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  
// Initialize the serial communication
  Serial.begin(38400);
  delay(2000);
  
// Initialize the devices
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

// Verify Connection with the external accelerometer
  Serial.println("Testing external accelerometer connection...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
// Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

// Change the accel/gyro offset values
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
  
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

  /*
  The orientations of the accel sensor:
  0: flat, processor facing up
  1: flat, processor facing down
  2: Y axis pointing upwards
  3: Y axis pointing downwards
  4: X axis pointing upwards
  5: X axis pointing downwards
  */

void loop() {
  int mainOrientation = - 1;    // the board's orientation
  int extOrientation = -1; // the accel sensor's orientation
  
  String mainOrientationString; // string for printing description of orientation
  String extOrientationString; //string for printing the accelerometer sensor's orientation

  // Read accelerometer:
  int x = CurieIMU.readAccelerometer(X_AXIS);
  int y = CurieIMU.readAccelerometer(Y_AXIS);
  int z = CurieIMU.readAccelerometer(Z_AXIS);

  // Calculate the absolute values, to determine the largest
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);

  // Read raw external accel/gyro:
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax1=ax/81;
  ay1=ay/81;
  az1=az/81;

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


  if (ax1 <= 20 && ay1 <= 20 && az1 >= 89) { 
    extOrientationString = "ext UP";
    extOrientation = 0;
  } else if (ax1 <= 20 && ay1 <= 20 && az1 <= -89) { 
    extOrientationString = "ext DOWN";
    extOrientation = 1;
  } else if (ax1 <= 20 && ay1 >= 89 && az1 <= 20) { 
    extOrientationString = "ext Y_UP";
    extOrientation = 2;
  } else if (ax1 <= 20 && ay1 <= -89 && az1 <= 20) { 
    extOrientationString = "ext Y_DOWN";
    extOrientation = 3;
  } else if (ax1 >= 89 && ay1 <= 20 && az1 <= 20) { 
    extOrientationString = "ext X_UP";
    extOrientation = 4;
  } else if (ax1 <= -89 && ay1 <= 20 && az1 <= 20) {
    extOrientationString = "ext X_DOWN";
    extOrientation = 5;
  }


  // if the orientation has changed, print out a description:
  if (mainOrientation != mainLastOrient) {
    Serial.println(mainOrientationString);
    mainLastOrient = mainOrientation;
  }

  if (extOrientation != extLastOrient) {
    Serial.println(extOrientationString);
  extLastOrient = extOrientation;
  }
}
