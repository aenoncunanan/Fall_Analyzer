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

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax1=ax/81;
    ay1=ay/81;
    az1=az/81;

    // display tab-separated accel/gyro x/y/z values
   if (ax1 <= 20 && ay1 <= 20 && az1 >= 89)
   { 
    Serial.print("z top"); Serial.print("\n");
   }
   if (ax1 <= 20 && ay1 <= 20 && az1 <= -89)
   { 
    Serial.print("z down"); Serial.print("\n");
   }
   
    if (ax1 <= 20 && ay1 >= 89 && az1 <= 20)
   { 
    Serial.print("y top"); Serial.print("\n");
   }
    if (ax1 <= 20 && ay1 <= -89 && az1 <= 20)
   { 
    Serial.print("y down"); Serial.print("\n");
   }

    if (ax1 >= 89 && ay1 <= 20 && az1 <= 20)
   { 
    Serial.print("x top"); Serial.print("\n");
   }
    if (ax1 <= -89 && ay1 <= 20 && az1 <= 20)
   { 
    Serial.print("x down"); Serial.print("\n");
   }

}
