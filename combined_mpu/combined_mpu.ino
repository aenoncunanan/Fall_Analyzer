
#include "CurieIMU.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;

int16_t axEx, ayEx, azEx, gxEx, gyEx, gzEx; 
int axIn, ayIn, azIn;
double byEx, bxEx, bzEx;
 
double vsumcurrentEx=0;
double vsumcheckEx=0; 
double vsumnewEx=0;

double vsumcurrentIn=0;
double vsumcheckIn=0;  
double vsumnewIn=0;

double degreesdiff=0;



void setup() {
  Serial.begin(38400); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
  
  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

   Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
   Serial.println("Testing device connections...");
   Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
String dynamicm;
String staticm;
byEx=ayEx;

  accelgyro.getMotion6(&axEx, &ayEx, &azEx, &gxEx, &gyEx, &gzEx); //mpu6050
  CurieIMU.readAccelerometer(axIn, ayIn, azIn); // curie

  vsumcurrentEx = sqrt(pow(axEx,2)+pow(ayEx,2)+pow(azEx,2));
  vsumcheckEx = abs((abs(vsumnewEx)-abs(vsumcurrentEx))/(abs(vsumnewEx)))*100;
  
  vsumcurrentIn = sqrt(pow(axIn,2)+pow(ayIn,2)+pow(azIn,2));
  vsumcheckIn = abs((abs(vsumnewIn)-abs(vsumcurrentIn))/(abs(vsumnewIn)))*100;
  
  // display tab-separated accelerometer x/y/z values
  Serial.print("Internal accelerations:\t");
  Serial.print(axIn);
  Serial.print("\t");
  Serial.print(ayIn);
  Serial.print("\t");
  Serial.print(azIn);
  Serial.println();

  Serial.print("External accelerations:\t");
  Serial.print(axEx);
  Serial.print("\t");
  Serial.print(ayEx);
  Serial.print("\t");
  Serial.print(azEx);
  Serial.println();

  if (abs(vsumcheckEx) >= 1.5 && abs(vsumcheckIn) >= 1.5) 
  {
  vsumnewEx=vsumcurrentEx;
  vsumnewIn=vsumcurrentIn;
  Serial.println(dynamicmode());
  }
  else {
    
  vsumnewEx = (vsumnewEx + vsumcurrentEx)/2; //average vector sum
  vsumnewIn = (vsumnewIn + vsumcurrentIn)/2; //average vector sum
  Serial.println(staticmode());
  }
 
delay(750);
}

static String staticmode(){

 String staticstr="Identifying....";

 if (axEx <= -13900 && axIn <= -16000 && axIn >= -17500){
  staticstr= "Standing Position";
 }
 else if (ayEx >= 14000 && axIn <= -12500){
 staticstr= "Sitting Position";
 }
 else if ((abs(azIn) || abs(ayIn)) >=  13500 && (abs(azEx) || abs(ayEx)) >= 14500){
    staticstr ="Lying Position";
  }

 
return staticstr;
}


static String dynamicmode(){

  String dynastr = "detecting motions";
  
  degreesdiff = abs(((180/3.14)*(acos(ayEx/vsumnewEx)))-((180/3.14)*(acos(byEx/vsumnewEx))));
  
//  Serial.print((abs((vsumnew-abs(ayEx))/vsumnew))*9.8);
//  Serial.println(" m/s^2");
// 

 if (abs(vsumcheckEx) >= 25 || abs(vsumcheckIn) >= 25 ) {
  dynastr = "High Acceleration detected!";
  
  if (ayIn >= 13500 && ayEx >= 15900 && (abs(vsumcheckEx) >= 25 || abs(vsumcheckIn) >= 25 )){
    dynastr ="Falling! : Backwards";
  }
  else if (ayIn <= -14000 && ayEx <= -15400 && (abs(vsumcheckEx) >= 25 || abs(vsumcheckIn) >= 25 )){
    dynastr ="Falling! : Forward";
  }
  else if (abs(azIn) >=  15900 && abs(azEx) >= 15400 && (abs(vsumcheckEx) >= 25 || abs(vsumcheckIn) >= 25 )){
    dynastr ="Falling! : Sideways";
  }

  
//  Serial.print((abs(vsumcheck/100))*9.8); 
//  Serial.println(" m/s^2");
 }
 else if (degreesdiff <= 70 && degreesdiff >= 10 && (abs(vsumcheckEx) >= 8 || abs(vsumcheckIn) >= 8 ) ){
  dynastr = "Walking";
 }
 return dynastr;
}



