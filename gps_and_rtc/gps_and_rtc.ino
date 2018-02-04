
#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial mySerial(7,8); //pins
    
void setup(){
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600); 
  getgps();
}

void loop(){
 sendData( "AT+CGNSINF",1000,DEBUG);   

} 

void getgps(void){
  Serial.println("Turning on the GPS...");
  sendData( "AT+CGNSPWR=1",1000,DEBUG);   
 
 
}
void sendData(String command, const int timeout, boolean debug){
    String response = "";    
   mySerial.println(command); 
   delay(5);
    if(debug){
      long int time = millis();   
      while( (time+timeout) > millis()){
        while(mySerial.available()){       
          response += char(mySerial.read());
        }  
      }    
      Serial.println(response);
      Serial.println(response[25]);
      
       if (response[25] == '1') {
       digitalWrite(4, HIGH);
       Serial.println("GPS READY ");
                                }
 
    }    
}
