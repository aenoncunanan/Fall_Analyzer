#define FallMemory A0
#define falseAlarmButton A3

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

void setup() {
  Serial.begin(9600);
  while(!Serial){
    
  }
  pinMode(FallMemory, OUTPUT);
  pinMode(falseAlarmButton, INPUT_PULLUP);

}

void loop() {

  Serial.println(digitalRead(falseAlarmButton));
  
  boolean flag = true;
  unsigned long int fallStart = millis();
  
  while(flag == true){
    if(millis() - fallStart <= 10000){
      if (digitalRead(falseAlarmButton) == LOW){
        Serial.println("BUTTON PRESSED!");
        flag = false;
        delay(5000);
      } else{
        Serial.println("BUZZ!!!");
        fallBuzz();
        delay(1000);
      } 
    }else {
      Serial.println("SENDING A TEXT MESSAGE!");
      flag = false;
      delay(5000);
    }
  }


}
