#include "CurieIMU.h"

boolean blinkState = false;          // state of the LED
unsigned long loopTime = 0;          // get the time since program started
unsigned long interruptsTime = 0;    // get the time when free fall event is detected


void setup() {
  Serial.begin(9600); // initialize Serial communication
  while(!Serial) ;    // wait for serial port to connect.

  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Free Fall Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_FREEFALL, 1000); // 1g=1000mg
  CurieIMU.setDetectionDuration(CURIE_IMU_FREEFALL, 50);  // 50ms
  CurieIMU.interrupts(CURIE_IMU_FREEFALL);

  Serial.println("IMU initialisation complete, waiting for events...");
}

void loop() {
  // if free fall event is detected in 1000ms, LED will be turned up
  loopTime = millis();
  if(abs(loopTime -interruptsTime) < 1000 )    
    blinkState = true;
  else
    blinkState = false;
  digitalWrite(13, blinkState);
}

static void eventCallback(){
  if (CurieIMU.getInterruptStatus(CURIE_IMU_FREEFALL)) {
    Serial.println("free fall detected! ");
    interruptsTime = millis(); 
  }
}
