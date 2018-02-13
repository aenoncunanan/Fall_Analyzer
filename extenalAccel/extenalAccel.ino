#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int ax1,ay1,az1;
int gx1,gy1,gz1;

int extLastOrient = -1; //Previous orientation of external Accel

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);

}

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
  int extOrientation = -1; // the accel sensor's orientation
  String extOrientationString; //string for printing the accelerometer sensor's orientation
  
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax1=ax/81;
  ay1=ay/81;
  az1=az/81;

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

  if (extOrientation != extLastOrient) {
    Serial.println(extOrientationString);
    extLastOrient = extOrientation;
  }

}
