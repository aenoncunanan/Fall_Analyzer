//A: IMPORT THE LIBRARIES AND DEFINITIONS
  //A1: DECLARATIONS FOR CURIEIMU
    #include "CurieIMU.h"

    unsigned long loopTime = 0;          // get the time since program started
    unsigned long interruptsTime = 0;    // get the time when free fall event is detected
  //A1: END

  //A2: DECLARATIONS FOR EXTERNAL ACCELEROMETER
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
  //A2: END

  //A3: DECLARATIONS FOR SDCARD MODULE
    #include <SPI.h>
    #include "SdFat.h"

    #define USE_SDIO 0 // Set USE_SDIO to zero for SPI card access

    const uint8_t SD_CHIP_SELECT = SS; // Default SD Chip select is the SPI SS pin
    
    #if USE_SDIO // Use faster SdioCardEX
      SdFatSdioEX sd;
    #else // USE_SDIO
      SdFat sd;
    #endif  // USE_SDIO

    float cardSize; // global for card size
    File myFile; //will be used for file creation
    SdFile file; //will be used for getting filenames
  //A3: END

  //A4: DECLARATIONS FOR GPS GSM RTC SHIELD
    #include <SoftwareSerial.h>

    #define DEBUG true
    #define GPSready A2

    SoftwareSerial mySerial(7, 8);
  //A4: END

  //A5: OTHER DECLARATIONS
    #define DeviceReady A1
    #define FallMemory A0

    #define falseAlarmButton A3

    String lastKnownTimeLoc = "";
    String userName = "";

    String onBoardFall = "";
    int lastOrient = -1; //Previous orientation of the user (for comparison)
  //A5: END

  //A6: DECLARATIONS FOR KEEPING TRACK OF ACTIVITIES
    #define maxAct 1000 //Maximum number of activities per text file
    int actFileCountR; //activity file counter
    int actCounterR;   //activity counter per file
  //A6: END

  //A7: DECLARATIONS FOR TIMER
    #include "CurieTimerOne.h"
    const int gpsTimer = 5000000; //5 seconds
    const int memoryTimer = 9000000; //9 seconds
  //A7: END
 
//A: END

void parseAndsave(char *buff) {
  char *name = strtok(buff, " =");
  if (name) {
    char *junk = strtok(NULL, " ");
    if (junk) {
      char *valu = strtok(NULL, " ");
      if (valu) {
        int val = atoi(valu);
        if (strcmp(name, "actFileCount") == 0) {
          actFileCountR = val;
        }
        if (strcmp(name, "actCounter") == 0) {
          actCounterR = val;
        }
      }
    }
  }
}

void readFileLog(){
  myFile = sd.open("filelog.txt");
  if (myFile) {
    char buffer[5];
    byte index = 0;

    while (myFile.available()) {
      //Serial.write(myFile.read());
      char c = myFile.read();
      if (c == '\n' || c == '\r') { //Check for carriage return or line feed
        parseAndsave(buffer);
        index = 0;
        buffer[index] = '\0'; //Keep buffer NULL terminated
      } else {
        buffer[index++] = c;
        buffer[index] = '\0'; //Keep buffer NULL terminated
      }
    }
    myFile.close();
  } else {
    Serial.println("ERROR READING FILE LOG!");
    readFileLog();
  }

  Serial.print("actFileCount: ");
  Serial.println(actFileCountR);
  Serial.print("actCounter: ");
  Serial.println(actCounterR);  
}

void logData(String userOrientation){
  if(!sd.exists("Activities")){
    sd.mkdir("Activities");
    sd.chdir("Activities");
  } else {
    sd.chdir("Activities");
  }

  String fileName = "activity";
  fileName.concat(actFileCountR);
  fileName.concat(".txt");

  if (actCounterR < maxAct){
    myFile = sd.open(fileName, FILE_WRITE);

    if (myFile){
      myFile.println(lastKnownTimeLoc);
      myFile.println(userOrientation);
      myFile.println("-end_of_activity-");
      myFile.close();
      actCounterR++;

      Serial.println(actCounterR);
//      Serial.println(sendData("AT+CGNSINF", 1000, DEBUG));
      Serial.println(lastKnownTimeLoc);
      Serial.println("User Orientation: " + userOrientation);
      Serial.println("-end_of_activity-");
    } else{
      Serial.println("Error Writing Activity File");  
    }
  } else{
    actFileCountR++;
    actCounterR = 0;
  }

  if(!sd.chdir()){
    Serial.println("Error Going back to the root folder");
  }

  updateFileLog();
  
}

void updateFileLog(){
  String a = "actFileCount = ";
  String b = "actCounter = ";

  a.concat(actFileCountR);
  b.concat(actCounterR);

  removeFile("filelog.txt");
  myFile = sd.open("filelog.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(a);
    myFile.println(b);
    Serial.println(a);
    Serial.println(b);

    myFile.close();
  } else {
    Serial.println("ERROR UPDATING FILE LOG!");
  }  
}

void removeFile(String toRemove) {
  myFile = sd.open(toRemove, FILE_WRITE);
  if (myFile) {
    if (!myFile.remove()) {
      Serial.println("ERROR REMOVING OLD FILE LOG!");
    }
    myFile.close();
  } else {
    Serial.println("ERROR OPENING FILE TO BE REMOVED!");
  }
}

void deleteOldFiles() {
  Serial.println("Deleting old files");
  sd.chdir("Activities");
  //sd.ls(LS_R);

  int counter = 0;
  int itemsToDelete = 5;

  char name[20];
  while (file.openNext(sd.vwd(), O_READ) && counter < itemsToDelete) {
//    file.printName();
    file.getName(name, 20); //20bytes
    
    removeFile(String(name));
    
    file.close();
    counter++;
  }

  if (!sd.chdir()) {
    Serial.println("ERROR RETURNING BACK TO THE ROOT");
  }
  delay(2000);
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
  if(abs(loopTime - interruptsTime) < 1000 ){    
    onBoardFall = "Falling!";
  } else {
    onBoardFall = "";
  }
}

void initMainBoard(){
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
  Serial.println("IMU initialization successful!"); 
}

void initExtAccel(){
  Serial.println("Initializing External Acceleromter...");
  
  // Join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  accelgyro.initialize();
  delay(1000);

  //Verify Connection with the external accelerometer
  Serial.println("Testing external accelerometer connection...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  accelgyro.setSleepEnabled(false);
  
  if(accelgyro.testConnection() == 0){
    Serial.println("Reinitialization started!");
    delay(500);
    Serial.write(12);
    setup();
  }

  // Change the accel/gyro offset values
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);

  Serial.println("External Accelerometer initialization successful!");
}

void initSDCard(){
  Serial.println("Initializing Sd Card...");
  #if USE_SDIO
    if (!sd.cardBegin()) {
      Serial.println("cardBegin failed");
      Serial.println("Re-initializing Sd Card...");
      initSDCard();
    }
    if (!sd.begin(SD_CHIP_SELECT)){
      Serial.println("SD Chip Select initialization failed");
      Serial.println("Re-initializing Sd Card...");
      initSDCard();      
    }
  #else  // USE_SDIO
    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.
    if (!sd.cardBegin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
      Serial.println("cardBegin failed");
      Serial.println("Re-initializing Sd Card...");
      initSDCard();      
    }
    if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))){
      Serial.println("SD Chip Select initialization failed");
      Serial.println("Re-initializing Sd Card...");
      initSDCard();      
    }
  #endif  // USE_SDIO 

  cardSize = sd.card()->cardSize();
  
  if (cardSize == 0) {
    Serial.println("cardSize failed");
    Serial.println("Re-initializing Sd Card...");
    initSDCard();    
  }
  
  if (!sd.fsBegin()) {
    Serial.println("\nFile System initialization failed.\n");
    Serial.println("Re-initializing Sd Card...");
    initSDCard();    
  }

    Serial.println("Sd Card initialization successful!");
}

void checkGPSConnection(){
    String response = sendData("AT+CGNSINF", 1000, DEBUG);

    //Check if GPS is already connected/fixed
    if (response[25] == '1') {
      digitalWrite(GPSready, HIGH);
      lastKnownTimeLoc = response;
      Serial.println("GPS READY!");
    } else {
      digitalWrite(GPSready, !digitalRead(GPSready));
      Serial.println("GPS Connecting...");
    }

    Serial.println(response);
}

void fallBuzz(){
  tone(FallMemory, 400, 200);
  delay(200);
  noTone(FallMemory);
  tone(FallMemory, 500, 200);
  delay(200);
  noTone(FallMemory);
  tone(FallMemory, 600, 200);
  delay(200);
  noTone(FallMemory);
}

void memoryBuzz(){
  tone(FallMemory, 300, 300);
  delay(300);
  noTone(FallMemory);  
}

void initGPSModule(){
  Serial.println("Initializing GPS,GSM,RTC Shield...");
  mySerial.begin(38400);
  
  pinMode(GPSready, OUTPUT);

  onGPS();

  while (lastKnownTimeLoc == ""){
    checkGPSConnection();
  }
}

void onGPS() {
  sendData("AT+CGNSPWR=1", 1000, DEBUG);
  Serial.println("GPS Turned ON!");
}

void offGPS() {
  sendData("AT+CGNSPWR=0", 1000, DEBUG);
  Serial.println("GPS Turned OFF!");
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  mySerial.println(command);

  delay(5);

  if (debug) {
    long int time = millis();
    while ( (time + timeout) > millis()) {
      while (mySerial.available()) {
        response += char(mySerial.read());
      }
    }
  }

  return response;
}

void checkSpace(){
  float totalSize = 0.000512 * cardSize;
  float freeSize = 0.000512*sd.vol()->freeClusterCount()*sd.vol()->blocksPerCluster();
  
  float lowLevel = 0.1 * totalSize; //10% of total size
  if (freeSize <= lowLevel){
    memoryBuzz();
    deleteOldFiles();
    Serial.println("LOW MEMORY SPACE!");
    Serial.print("Remaining Space: ");
    Serial.print(freeSize);
    Serial.println(" MB (MB = 1,000,000 bytes)");    
  } else {
    Serial.print("Remaining Space: ");
    Serial.println(freeSize);
  }
}

String setMessage(){
  String message = userName;
  message.concat(" has been fallen! at:  ");
  String date = "";
  String longitude = "";
  String latitude = "";
  int comma = 0;
  int i = 0;
  boolean gpsReady = 0;
  
  while (comma <= 5){
    if (lastKnownTimeLoc[i] == ',' ){
      comma++;
      i++;
    }  

    if (comma == 2){
      date = date + lastKnownTimeLoc[i];
    }
    if (comma == 3){
      longitude = longitude + lastKnownTimeLoc[i];
    }
    if (comma == 4){
      latitude = latitude + lastKnownTimeLoc[i];
    }
    i++;                    
 
  }

  message.concat("LONGITUDE: ");
  message.concat(longitude);
  message.concat("\n");
  message.concat("LATITUDE: ");
  message.concat(latitude);
  message.concat("\n");
  message.concat("TIME: ");
  message.concat(date);                                      
  message.concat(" \r");

  return message;
}

void initUsername(){
  myFile = sd.open("profile.txt");
  if (myFile){
    char buffer[15];
    byte index = 0;
    int lineCount = 0;
    while(myFile.available() && lineCount < 3){
      char c = myFile.read();
      if (c == '\n' || c == '\r') { //Check for carriage return or line feed
        userName.concat(buffer);
        lineCount++;
        index = 0;
        buffer[index] = '\0'; //Keep buffer NULL terminated        
      } else{
          buffer[index++] = c;
          buffer[index] = '\0'; //Keep buffer NULL terminated        
      }
    }
    
    myFile.close();
  }else{
    Serial.println("ERROR READING PROFILE!");
  }
}

void SendTextMessage(){
  offGPS(); //turn off GPS to prevent interruption

  Serial.println("======================");
  Serial.println("===SENDING MESSAGE!===");

  String message = setMessage();

  myFile = sd.open("respondents.txt");
  if(myFile){
    char buffer[15];
    byte index = 0;

    while(myFile.available()){
      char c = myFile.read();
      boolean sent = false;
      if(c == '\n' || c == '\r') { //Check for carriage return or line feed
        if(buffer[0] == '+'){
          String toContact(buffer);
          
          Serial.print("CONTACT NUMBER: ");
          Serial.println(toContact);
          
          while(sent == false){
            String response = "";
            String receiver = "AT+CMGS=\"";
            receiver.concat(toContact);
            receiver.concat("\"\r"); 
            Serial.println(receiver);
            
            mySerial.print("\r");
            mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
            mySerial.print(receiver);
            delay(1000);
            mySerial.print(message);
            mySerial.write(0x1A);

            long int time = millis();
            while((time + 1000) > millis()){
              while(mySerial.available()){
                response += char(mySerial.read());
              }
            }
  
            Serial.println("RESPONSE: ");
            Serial.println(response);
  
            int bracketCount = 0;
            for(int i = 0; i < response.length(); i++){
              if(response[i] == '>'){
                bracketCount++;
              }
            }
            if (bracketCount >= 2){
              sent = true;
              Serial.println("===MESSAGE SENT!===");
            }else{
              Serial.println("===MESSAGE WAS NOT SENT!===");
              Serial.println("Resending Message...");
              sent = false;
            }            
          }
          
        }
        index = 0;
        buffer[index] = '\0'; //Keep buffer NULL terminated        
      }else{
        buffer[index++] = c;
        buffer[index] = '\0'; //Keep buffer NULL terminated 
      }
    }
    myFile.close();
  } else{
    Serial.println("ERROR READING RESPONDENTS FILE!");
  }


//  String toContact = "AT+CMGS=\"";
//  toContact.concat("+639165200536");
//  toContact.concat("\"\r");
//  
//  delay(1000);
//  
//  Serial.println("===Sending message===");
//
//  mySerial.print("\r");
//  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode    //Start accepting the text for the message
//  mySerial.print(toContact);
//  //to be sent to the number specified.
//  //Replace this number with the target mobile number.
//  delay(1000);
//  mySerial.print(message);   //The text for the message
//  mySerial.write(0x1A);  //Equivalent to sending Ctrl+Z
//
//  long int time = millis();
//  while ( (time + 1000) > millis()) {
//    while (mySerial.available()) {
//      response += char(mySerial.read());
//    }
//  }
//  
//  Serial.println("RESPONSE: ");
//  Serial.println(response);
//
//  int bracketCount = 0;
//  boolean sent = false;
////  unsigned int responseLength = response.length();
//  for(int i = 0; i < response.length(); i++){
//    if (response[i] == '>'){
//      bracketCount++;
//    }
//  }
//  if (bracketCount >= 2){
//    sent = true;
//    Serial.println("===SENT!===");
//  } else{
//    Serial.println("===MESSAGE WAS NOT SENT!===");
//    SendTextMessage();
//  }

  Serial.println("===DONE!===");
  Serial.println("======================");
  delay(5000);
  onGPS(); //turon on GPS again  
}

static void eventCallback(){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_FREEFALL)) {
    interruptsTime = millis(); 
  }
}

void setup() {
  Serial.begin(38400);
  delay(1000);
  while(!Serial){} //wait for serial port to connect

  //Initialize the devices
  initMainBoard();
  initExtAccel();
  initSDCard();
  readFileLog();
  initUsername();
  initGPSModule();  

  pinMode(FallMemory, OUTPUT);
  pinMode(DeviceReady, OUTPUT);
  pinMode(falseAlarmButton, INPUT_PULLUP);

  CurieTimerOne.start(memoryTimer, &checkSpace);
  
  //Prompt a welcome message
  Serial.println("Device is ready!");
  digitalWrite(DeviceReady, HIGH);

  delay(3000);
}

void loop() {
  checkGPSConnection();

  String userOrientation = "";
  String onBoardAccel = "";
  String extAccel = "";

  int orientation = -1;

  onBoardAccel = onBoardAccelerometer();
  extAccel = externalAccelerometer();
  onBoardFallSense();

  if (onBoardAccel == "X_UP" && extAccel == "Y_UP"){
    userOrientation = "Sitting Position";
    orientation = 0;
  } else if (onBoardAccel == "X_UP" && extAccel == "Y_DOWN"){
    userOrientation = "Sitting Position";
    orientation = 1;
  } else if (onBoardAccel == "X_UP" && extAccel == "X_DOWN"){
    userOrientation = "Standing Position";
    orientation = 2;
  } else {
    userOrientation = "UNKNOWN";
    orientation = 3;
  }
  
  if (onBoardFall == "Falling!"){
    unsigned long int fallStart = millis();
    
    if (onBoardAccel == "Z_UP"){
      userOrientation = "Falling! : Sideways";
    } else if (onBoardAccel == "Z_DOWN"){
      userOrientation = "Falling! : Sideways";
    } else if (onBoardAccel == "Y_DOWN"){
      userOrientation = "Falling! : Forward";
    } else if (onBoardAccel == "Y_UP"){
      userOrientation = "Falling! : Backwards";
    }       

    orientation = 4;

    //check wether to send an alarm in 10 seconds
    boolean flag = true;
    while(flag == true){
      if(millis() - fallStart <= 10000){
        if (digitalRead(falseAlarmButton) == LOW){
          Serial.println("False Alarm!");
          flag = false;
        } else{
          Serial.println(millis() - fallStart);
          fallBuzz();
        }
      } else{
        SendTextMessage();
        flag = false;
      }      
    }
     
  }

  // if the orientation has changed, print out a description:
  if (orientation != lastOrient) {
    lastOrient = orientation;
    if(userOrientation != "UNKNOWN"){
      logData(userOrientation); 
    }
  } 
}
