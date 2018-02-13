#include <SPI.h>
#include "SdFat.h"

// Set USE_SDIO to zero for SPI card access.
#define USE_SDIO 0
/*
   SD chip select pin.  Common values are:

   Arduino Ethernet shield, pin 4.
   SparkFun SD shield, pin 8.
   Adafruit SD shields and modules, pin 10.
   Default SD chip select is the SPI SS pin.
*/
const uint8_t SD_CHIP_SELECT = SS;

#if USE_SDIO
// Use faster SdioCardEX
SdFatSdioEX sd;
#else // USE_SDIO
SdFat sd;
#endif  // USE_SDIO

// global for card size
float cardSize;

File myFile;

SdFile file;

void readFileLog() {
  myFile = sd.open("respondents.txt");
  if (myFile) {
    char buffer[15];
    byte index = 0;

    while (myFile.available()) {
      char c = myFile.read();
        if (c == '\n' || c == '\r') { //Check for carriage return or line feed
             if(buffer[0] == '+'){
//                String toContact(buffer); 
//                Serial.println(toContact);                 

                String toContact(buffer);
                String a = "\"AT+CMGS=\"";
                a.concat(toContact);
                a.concat("\"\\r\""); 
                Serial.println(a);
             }
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
  }

}

void initSD() {
#if USE_SDIO
  if (!sd.cardBegin()) {
    Serial.println("\ncardBegin failed");
    return;
  }
  if (!sd.begin(SD_CHIP_SELECT)) {
    Serial.println("initialization failed");
    return;
  }
#else  // USE_SDIO
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.cardBegin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
    Serial.println("cardBegin failed");
    return;
  }
  if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))) {
    Serial.println("initialization failed");
    return;
  }
#endif  // USE_SDIO 

  cardSize = sd.card()->cardSize();

  if (cardSize == 0) {
    Serial.println("cardSize failed");
    return;
  }

  if (!sd.fsBegin()) {
    Serial.println("\nFile System initialization failed.\n");
    return;
  }

  readFileLog();
}

void setup() {
  Serial.begin(9600);

  // Wait for USB Serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  initSD();
}

void loop() {
}
