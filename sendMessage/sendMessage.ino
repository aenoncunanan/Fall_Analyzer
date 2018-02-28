#define DEBUG true
#define GPSready A2
#define DeviceReady A1
#define FallMemory A0

String lastKnownTimeLoc = "";
String userName = "";

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

String setMessage(){
  String message = "aenon";
  message.concat(" has been fallen! \nDetails: \n");
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

void SendTextMessage(){
  offGPS(); //turn off GPS to prevent interruption

  String response = "";
  String message = setMessage();

  myFile = sd.open("respondents.txt");
  
  String toContact = "AT+CMGS=\"";
  toContact.concat("+639165200536");
  toContact.concat("\"\r");
  
  delay(1000);
  
  Serial.println("===Sending message===");

  mySerial.print("\r");
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
//  mySerial.print("AT+CMGS=\"+639165200536\"\r");    //Start accepting the text for the message
  mySerial.print(toContact);
  //to be sent to the number specified.
  //Replace this number with the target mobile number.
  delay(1000);
  mySerial.print(message);   //The text for the message
  mySerial.write(0x1A);  //Equivalent to sending Ctrl+Z

  long int time = millis();
  while ( (time + 1000) > millis()) {
    while (mySerial.available()) {
      response += char(mySerial.read());
    }
  }
  
  Serial.println("RESPONSE: ");
  Serial.println(response);

  int bracketCount = 0;
  boolean sent = false;
//  unsigned int responseLength = response.length();
  for(int i = 0; i < response.length(); i++){
    if (response[i] == '>'){
      bracketCount++;
    }
  }
  if (bracketCount >= 2){
    sent = true;
    Serial.println("===SENT!===");
  } else{
    Serial.println("===MESSAGE WAS NOT SENT! RESENDING...===");
    SendTextMessage();
  }
  
  delay(5000);
  onGPS(); //turon on GPS again  
}

//void SendTextMessage() {
//  offGPS();
//
//  String response = "";
//  String message = setMessage();
//  
//  myFile = sd.open("respondents.txt");
//  if (myFile) {
//    char buffer[15];
//    byte index = 0;
//
//    while (myFile.available()) {
//      char c = myFile.read();
//        if (c == '\n' || c == '\r') { //Check for carriage return or line feed
//             if(buffer[0] == '+'){
//                String toContact(buffer);
//                String receiver = "AT+CMGS=\"";
//                receiver.concat(toContact);
//                receiver.concat("\"\r"); 
//                String single = "AT+CMGS=\"09165200536\"\r";
//                Serial.println("Mulitiple receiver: ");
//                Serial.println(receiver);
//
//                Serial.println("Single receiver: ");
//                Serial.println(single);
//                
//                mySerial.print("\r");
//                mySerial.print("AT+CMGF=1 \r");    //Because we want to send the SMS in text mode
//                mySerial.print(single);
//                delay(1000);
//                mySerial.print(message);
//                mySerial.write(0x1A);
//                delay(1000);
//             }
//               index = 0;
//               buffer[index] = '\0'; //Keep buffer NULL terminated
//        } else {
//                buffer[index++] = c;
//                buffer[index] = '\0'; //Keep buffer NULL terminated
//        }
//
//    }
//    myFile.close();
//  } else {
//    Serial.println("ERROR READING RESPONDENTS FILE!");
//  }
//
//  delay(2000);
//  onGPS(); //turon on GPS again
//}

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

  Serial.print("User: ");
  Serial.println(userName);
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

void setup() {
  Serial.begin(38400);
  while(!Serial){
  }
  pinMode(FallMemory, OUTPUT);
  pinMode(DeviceReady, OUTPUT);
  
//  initSDCard();
//  initUsername();
  initGPSModule();

  delay(3000);
  
  Serial.println("Device is ready!");
  digitalWrite(DeviceReady, HIGH);
}

void loop() {
  checkGPSConnection();
  Serial.println("==================");
  SendTextMessage();
  Serial.println("==================");
  Serial.println("5 seconds delay.");
  delay(5000);
}
