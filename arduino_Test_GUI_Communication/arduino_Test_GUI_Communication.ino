#define BAUD 115200 //115200

int16_t i = 58;
int16_t setThrottle = 0;
byte myData[] = {1, 2, 3, 4, 42};
int testNum = 42;
int ledGreen = 8;
int ledRed = 13;


void setup() {
  // put your setup code here, to run once:

   setupSerial();
   pinMode(ledGreen, OUTPUT);
   pinMode(ledRed, OUTPUT);

}




void loop() {
  analogWrite(ledRed, 250);
   Serial.write(myData,5);
    analogWrite(ledRed, 1);
   //Serial.flush();
   //Serial.println(488);
   //testNum = (myData[0] + 38)/testNum; 
   delay(1);
   receiveSerial();
}

void receiveSerial(){
  
  while(Serial.available()){ // if there is data to read
    myData[0] = Serial.read();
    
  }
 
  analogWrite(ledGreen, myData[0]*2);
}

void setupSerial(){
   Serial.begin(BAUD);
}
