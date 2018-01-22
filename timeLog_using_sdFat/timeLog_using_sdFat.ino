#include <SPI.h>
#include "SdFat.h"

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

// Set USE_SDIO to zero for SPI card access. 
#define USE_SDIO 0
/*
 * SD chip select pin.  Common values are:
 *
 * Arduino Ethernet shield, pin 4.
 * SparkFun SD shield, pin 8.
 * Adafruit SD shields and modules, pin 10.
 * Default SD chip select is the SPI SS pin.
 */
const uint8_t SD_CHIP_SELECT = SS;
/*
 * Set DISABLE_CHIP_SELECT to disable a second SPI device.
 * For example, with the Ethernet shield, set DISABLE_CHIP_SELECT
 * to 10 to disable the Ethernet controller.
 */
const int8_t DISABLE_CHIP_SELECT = -1;

#if USE_SDIO
  // Use faster SdioCardEX
  SdFatSdioEX sd;
#else // USE_SDIO
  SdFat sd;
#endif  // USE_SDIO

// global for card size
uint32_t cardSize;

File myFile;

void getSpace() {
  Serial.print("Volume is FAT");
  Serial.println(sd.vol()->fatType());

  Serial.print("Total Space: ");
  Serial.print(0.000512*cardSize);
  Serial.println(" MB (MB = 1,000,000 bytes)");
  
  float fs = 0.000512*sd.vol()->freeClusterCount()*sd.vol()->blocksPerCluster();

  Serial.print("Free Space: ");
  Serial.print(fs);
  Serial.println(" MB (MB = 1,000,000 bytes)");

  float percentage = 0.1 * (0.000512 * cardSize);
  if (fs <= percentage){
    Serial.print("LOW MEMORY SPACE!");
  }
  
}

void writeFile(){
  myFile = sd.open("test.txt", FILE_WRITE);
  if (myFile){
    DateTime now = RTC.now();
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 

    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print('/');
    myFile.print(now.year(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.println(); 

    myFile.close();
  } else {
    Serial.println("error opening file");
  }
}

void readFile(){
  myFile = sd.open("test.txt");
  if (myFile){
    Serial.println("Reading...");
    while(myFile.available()){
      Serial.write(myFile.read());
    }
    Serial.println("Done.");
    myFile.close();
  } else {
    Serial.println("error opening file");
  }
}

void removeFile(){
  Serial.println("Removing...");
  if (!sd.remove("test.txt")){
    Serial.println("ERROR removing file.");
  } else {
    Serial.println("Done.");
  }
}

void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Wire.begin(1101000);
  RTC.begin();
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  #if USE_SDIO
    if (!sd.cardBegin()) {
      Serial.println("\ncardBegin failed");
      return;
    }
    if (!sd.begin(SD_CHIP_SELECT)){
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
    if (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50))){
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

  Serial.println();
  getSpace();
}

void loop() {
  writeFile();
  delay(1000);
}
