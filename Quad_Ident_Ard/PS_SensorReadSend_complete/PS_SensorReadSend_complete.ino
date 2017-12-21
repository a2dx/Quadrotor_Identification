#include "HX711.h"

#define INTERRUPTPIN 2
#define BAUDRATE 115200

#define WEIGHT_DOUT_PIN  5
#define WEIGHT_CLK_PIN  4
#define CURRENTMESSUREPIN 0

HX711 scale(WEIGHT_DOUT_PIN, WEIGHT_CLK_PIN);


// This flag will be set if interrupt is triggered
volatile uint8_t readSensorFLAG = 0;

// Check to record
 uint8_t recordFLAG = 0;

// This variable counts the interrupts, if it reaches 6, all sensor will be read and the data will be send
uint8_t rotationPart = 0;

// DataArray, this array stores all sensor data
byte data[8];
// temporary variable to store the last measured weight, if the loading cell can not provide new data
int16_t weight = 0;

uint8_t currentThrottleSet = 0;

void setup() {

  // Setup Serial
  Serial.begin(BAUDRATE);

  // Setup beeper to check if the arduino is too slow to process sensor data
  pinMode(10, OUTPUT);

  // Setup Loadinc Cell
  initLoadingCell();

  // Setup Interrupt
  initInterrupt();

  // Double beep to show that the arduino is ready
  PORTB |= (1 << PB2);
  delay(150);
  PORTB &= ~(1 << PB2);
  delay(150);
  PORTB |= (1 << PB2);
  delay(150);
  PORTB &= ~(1 << PB2);;
}

void loop() {

  
    // check if the interrupt triggered
    if (readSensorFLAG == 1) {
      rotationPart++;
      // check if the motor did a full circulation
      if (rotationPart == 6) {
        // read the sensor and send the data
        processSensorData();
        rotationPart = 0;
      }
      // Reset flag
      readSensorFLAG = 0;
    }
  

  // check if start/stop signal is send
  if (Serial.available() > 0) {
     currentThrottleSet = Serial.read();
  
      // send initial timestamp
      //processSensorData();
  
  }
}

// Read and send all Sensor Data
void processSensorData() {
  
  // get TimeStamp
  long timeStamp = micros();

  // check if loadcell can provide new data, if not use last data
  if(scale.is_ready()){
    weight = scale.get_units() * 10000;
  } 
  // get current
  int16_t current = analogRead(CURRENTMESSUREPIN);

  // write data
  data[0] = (int)((timeStamp & 0xFF));
  data[1] = (int)((timeStamp >> 8) & 0xFF);
  data[2] = (int)((timeStamp >> 16) & 0xFF);
  data[3] = (int)((timeStamp >> 24) & 0xFF);

  //current Throttle set
  data[4] = currentThrottleSet;
 
  //weight
  data[5] = (int)((weight & 0xFF));
  data[6] = (int)((weight >> 8) & 0xFF);

  //current
  data[7] = (int)((current & 0xFF));
 
  // send data
  Serial.write(data, 8);
}

/*
 * Sets the readSensorFlag
 */
void setReadSensorFlag() {
  
  
    
    // checks if the arduino can process the data, if the arduino is too slow it will activate the beeper
    if (readSensorFLAG == 1) {
      PORTB |= (1 << PB2);
    }


  // Set the readSensorFlag to read the sensor data in the main loop
  readSensorFLAG = 1;
}

/*
 * Setup functions
 */

 // Init interrupt
void initInterrupt() {
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), setReadSensorFlag, RISING);
}

// Init LoadingCell
void initLoadingCell(){
  scale.set_scale(-738650);
  scale.tare(); 
}
