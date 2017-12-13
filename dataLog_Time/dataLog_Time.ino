#include <Wire.h>
#include <SD.h>
#include "RTClib.h"

RTC_DS1307 RTC;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 4;   

void setup() {
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
    Wire.begin();
    RTC.begin();
  // Check to see if the RTC is keeping time.  If it is, load the time from your computer.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }    

  Serial.print("\nInitializing SD card...");
  
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect) || !SD.begin(4)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }
}

void loop(void) {
  DateTime now = RTC.now(); 
  int months = now.month();
  int days = now.day();
  int years = now.year();
  int hours = now.hour();
  int minutes = now.minute();
  int seconds = now.second();

  File dataFile = SD.open("dataLog.txt", FILE_WRITE);

  if (dataFile){
    dataFile.print(months);
    dataFile.print("-");
    dataFile.print(days);
    dataFile.print("-");
    dataFile.print(years);
    dataFile.println("");
    dataFile.print(hours);
    dataFile.print(":");
    dataFile.print(minutes);
    dataFile.print(":");
    dataFile.print(seconds);
    dataFile.println("");
    dataFile.close();
    Serial.print(months);
    Serial.print("-");
    Serial.print(days);
    Serial.print("-");
    Serial.print(years);
    Serial.println("");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.println("");
  } else{
    Serial.println("error opening datalog.txt");
  }
  delay(1000);
}
