//FOR GPS RTC GSM SHIELD
#include <SoftwareSerial.h>

#define DEBUG true
#define powerKey 9
#define GPSready 5

SoftwareSerial mySerial(7, 8);
//END

//FOR SD MODULE
#include <SPI.h>
#include "SdFat.h"

// Set USE_SDIO to zero for SPI card access.
#define USE_SDIO 0

const uint8_t SD_CHIP_SELECT = SS;
const int8_t DISABLE_CHIP_SELECT = -1;

#if USE_SDIO
// Use faster SdioCardEX
SdFatSdioEX sd;
#else // USE_SDIO
SdFat sd;
#endif  // USE_SDIO

// global for card size
float cardSize;

File myFile;
//END


void setup() {
  Serial.begin(38400);
  mySerial.begin(38400);
  pinMode(GPSready, OUTPUT);
  pinMode(powerKey, OUTPUT);

  onGPS();
  initSDCard();

  //  digitalWrite(powerKey, HIGH);
  //  delay(5000);
  //  digitalWrite(powerKey, LOW);

  digitalWrite(GPSready, HIGH);
  delay(1000);
  digitalWrite(GPSready, LOW);
}

void loop() {
  String response = sendData("AT+CGNSINF", 1000, DEBUG);
  logData(response);

  Serial.println(response);
  Serial.println("=======");
}

void onGPS() {
  sendData("AT+CGNSPWR=1", 1000, DEBUG);
  Serial.println("GPS Turned ON!");
}

void offGPS() {
  sendData("AT+CGNSPWR=0", 1000, DEBUG);
  Serial.println("GPS Turned OFF!");
}

void initSDCard() {
  Serial.println("Initializing Sd Card...");
#if USE_SDIO
  if (!sd.cardBegin()) {
    Serial.println("\ncardBegin failed");
    Serial.println("Re-initializing Sd Card...");
    initSDCard();
  }
  if (!sd.begin(SD_CHIP_SELECT)) {
    Serial.println("initialization failed");
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
  if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
    Serial.println("initialization failed");
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
}

void logData(String dataToWrite) {
  myFile = sd.open("testing.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(dataToWrite);
    myFile.println("===========");
    myFile.close();
  } else {
    Serial.println("Error Opening File");
  }
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

    //Check if GPS is already connected
    if (response[25] == '1') {
      digitalWrite(GPSready, HIGH);
      Serial.println("GPS READY!");
    } else {
      digitalWrite(GPSready, LOW);
    }
  }

  return response;
}

void SendTextMessage() {
  offGPS(); //turn off GPS to prevent interruption

  mySerial.print("\r");
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  mySerial.print("AT+CMGS=\"09165200536\"\r");    //Start accepting the text for the message
  //to be sent to the number specified.
  //Replace this number with the target mobile number.
  mySerial.print("it works \r");   //The text for the message
  mySerial.write(0x1A);  //Equivalent to sending Ctrl+Z

  delay(2000);
  onGPS(); //turon on GPS again
}
