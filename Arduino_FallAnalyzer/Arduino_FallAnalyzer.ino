// Import the Libraries needed
//Libraries for Genuino101
#include "CurieIMU.h"

//Libraries for External Accelerometer
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int ax1,ay1,az1;
int gx1,gy1,gz1;

unsigned long loopTime = 0;          // get the time since program started
unsigned long interruptsTime = 0;    // get the time when free fall event is detected

void setup() {
  // Initialize the serial communication
  Serial.begin(38400);
  delay(1000);
  while(!Serial) ;    // wait for serial port to connect.
  
  initMainBoard();
  initExtAccel();
  
  // Prompt a welcome message  
  Serial.println("Device is ready!"); 
  delay(3000);
}

void initMainBoard(){
  // Initialize the devices
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  delay(1000);
  
  CurieIMU.attachInterrupt(eventCallback);

  // Enable Free Fall Detection
  CurieIMU.setDetectionThreshold(CURIE_IMU_FREEFALL, 1000); // 1g=1000mg
  CurieIMU.setDetectionDuration(CURIE_IMU_FREEFALL, 50);  // 50ms
  CurieIMU.interrupts(CURIE_IMU_FREEFALL);   
  
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);  
}

void initExtAccel(){
  // Join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  //Initialize I2C device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  delay(1000);

  //Verify Connection with the external accelerometer
  Serial.println("Testing external accelerometer connection...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  accelgyro.setSleepEnabled(false);
  
/*  while(accelgyro.testConnection() == 0){
    accelgyro.initialize();
    delay(1000);
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    accelgyro.setSleepEnabled(false);
  } */
  
  if(accelgyro.testConnection() == 0){
    Serial.println("Restarting the device!");
    delay(500);
    Serial.write(12);
    setup();
  }

  // Change the accel/gyro offset values
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
}

String onBoardFall = ""; 
int lastOrient = -1; //Previous orientation of the user (for comparison)

void loop() { 
 
  String userOrientation = "";
  String onBoardAccel = "";
  String extAccel = ""; 

  int orientation = -1;
  
  onBoardAccel = onBoardAccelerometer();
  extAccel = externalAccelerometer();  
  onBoardFallSense();
  
  if (onBoardAccel == "X_UP" && extAccel == "X_UP"){
    userOrientation = "Sitting Position";
    orientation = 0;
  } else if (onBoardAccel == "X_UP" && extAccel == "X_DOWN"){
    userOrientation = "Sitting Position";
    orientation = 1;
  } else if (onBoardAccel == "X_UP" && extAccel == "Y_UP"){
    userOrientation = "Standing Position";
    orientation = 2;
  }
  
  if (onBoardFall == "Falling!"){
    userOrientation = "Falling!";
    orientation = 3;
  }

  // if the orientation has changed, print out a description:
  if (orientation != lastOrient) {
    Serial.println("User Orientation: " + userOrientation);
    lastOrient = orientation;
    //logData here!
    //get Date through RTC module
    //get Time through RTC module
    //get Activity through userOrientation
//    if (userOrientation == "Falling!"){
//      //get Location using GSM module
//    }
    //add "end_of_activity" string every after activity
  } 

}

String onBoardAccelerometer() {
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
      mainOrientationString = "Z_UP";
    } else {
      mainOrientationString = "Z_DOWN";
    }
  } else if ( (absY > absX) && (absY > absZ)) {
    // base orientation on Y
    if (y > 0) {
      mainOrientationString = "Y_UP";     
    } else {
      mainOrientationString = "Y_DOWN"; 
    }
  } else {
    // base orientation on X
    if (x < 0) {
      mainOrientationString = "X_UP";
    } else {
      mainOrientationString = "X_DOWN";            
    }
  } 

  return mainOrientationString;
}

String externalAccelerometer(){
  String extOrientationString; //string for printing the accelerometer sensor's orientation
  
  // Read raw external accel/gyro:
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax1=ax/81;
  ay1=ay/81;
  az1=az/81;

  if (ax1 <= 20 && ay1 <= 20 && az1 >= 89) { 
    extOrientationString = "Z_UP";
  } else if (ax1 <= 20 && ay1 <= 20 && az1 <= -89) { 
    extOrientationString = "Z_DOWN";
  } else if (ax1 <= 20 && ay1 >= 89 && az1 <= 20) { 
    extOrientationString = "Y_UP";
  } else if (ax1 <= 20 && ay1 <= -89 && az1 <= 20) { 
    extOrientationString = "Y_DOWN";
  } else if (ax1 >= 89 && ay1 <= 20 && az1 <= 20) { 
    extOrientationString = "X_UP";
  } else if (ax1 <= -89 && ay1 <= 20 && az1 <= 20) {
    extOrientationString = "X_DOWN";
  } 

   return extOrientationString;
}

static void onBoardFallSense(){
    //Check if falling
  loopTime = millis();
  if(abs(loopTime -interruptsTime) < 1000 ){    
    onBoardFall = "Falling!";
  } else {
    onBoardFall = "";
  }
}

static void eventCallback(){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_FREEFALL)) {
    interruptsTime = millis(); 
  }
}
