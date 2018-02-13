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

#define maxAct 1000 //Maximum number of activities per text file
int actFileCountR; //activity file counter
int actCounterR;   //activity counter per file

void getSpace() {
  float totalSize = 0.000512 * cardSize;
  float freeSize = 0.000512 * sd.vol()->freeClusterCount() * sd.vol()->blocksPerCluster();

  Serial.print("Volume is FAT");
  Serial.println(sd.vol()->fatType());

  Serial.print("cardSize: ");
  Serial.println(cardSize);

  Serial.print("Total Space: ");
  Serial.print(totalSize);
  Serial.println(" MB (MB = 1,000,000 bytes)");

  Serial.print("Free Space: ");
  Serial.print(freeSize);
  Serial.println(" MB (MB = 1,000,000 bytes)");

  float lowLevel = 0.1 * totalSize;
  if (freeSize <= lowLevel) {
    Serial.print("LOW MEMORY SPACE!");
  }

}

void checkSpace() {
  float totalSize = 0.000512 * cardSize;
  float freeSize = 0.000512 * sd.vol()->freeClusterCount() * sd.vol()->blocksPerCluster();

  float lowLevel = 0.1 * totalSize; //10% of total size
  if (freeSize <= lowLevel) {
    //Make a noise thru buzzer
    deleteOldFiles();
    Serial.println("LOW MEMORY SPACE!");
    Serial.print("Remaining Space: ");
    Serial.print(freeSize);
    Serial.println(" MB (MB = 1,000,000 bytes)");
  }
}

SdFile file;

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

void writeFile() {
  if (!sd.exists("Activities")) {
    sd.mkdir("Activities");
    sd.chdir("Activities");
  } else {
    sd.chdir("Activities");
  }

  String fileName = "activity";
  fileName.concat(actFileCountR);
  fileName.concat(".txt");

  if (actCounterR < maxAct) {
    myFile = sd.open(fileName, FILE_WRITE);
    if (myFile) {
      myFile.println(actCounterR);
      myFile.println("GPS RTC GSM");
      myFile.println("Orientation");
      myFile.println("-end_of_activity-");
      myFile.close();
      actCounterR++;

      Serial.println(actCounterR);
      Serial.println("GPS RTC GSM");
      Serial.println("Orientation");
      Serial.println("-end_of_activity-");

      myFile.close();
    } else {
      Serial.println("ERROR WRITING ACTIVITY FILE!");
    }
  } else {
    actFileCountR++;
    actCounterR = 0;
  }
  if (!sd.chdir()) {
    Serial.println("ERROR GOING BACK TO THE ROOT");
  }
  updateFileLog();
}

void updateFileLog() {
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

void readFileLog() {
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
  }

  Serial.print("actFileCount: ");
  Serial.println(actFileCountR);
  Serial.print("actCounter: ");
  Serial.println(actCounterR);

}

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
  writeFile();
  checkSpace();
  delay(1000);
}
