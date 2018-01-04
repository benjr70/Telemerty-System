#include "Wire.h"
#include <Time.h>
//*************************************************************************gps
/*
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(10,9);
*/
static void smartdelay(unsigned long ms);

//******************************************************************************

//hourFormat12();
// I2Cdev and MPU9150 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU9150.h"
#include "helper_3dmath.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU9150 accelGyroMag;

const int trigPin1 = 22;
const int echoPin1 = 23;
const int trigPin2 = 24;
const int echoPin2 = 25;
const int trigPin3 = 26;
const int echoPin3 = 27;
const int trigPin4 = 28;
const int echoPin4 = 29;
long duration1,duration2,duration3,duration4;
unsigned long age, date,chars = 0;
unsigned short sentences = 0, failed = 0;


struct data{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;
  int16_t distance1, distance2,distance3,distance4;
  
};

struct data2{
  //float flat, flon, mph;
 // byte hour, minute, second, hundredths;
  int rotaryencoder = 0; 
  int rotaryencoder2 = 0;
};

struct data data;
struct data2 data2;

//********************************************************pipe and radio stuff
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8); 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };    
//*******************************************************************

void setup() {

  //************************************************************** for Accel/gryo
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  
  // initialize serial communication
  Serial.begin(115200);
  
  // initialize device
  // Serial.println("Initializing I2C devices...");
  accelGyroMag.initialize();
  
  // verify connection
  // Serial.println("Testing device connections...");
  // Serial.println(accelGyroMag.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
  
  //configure Arduino LED pin for output
  //pinMode(LED_PIN, OUTPUT);
  
  //********************************************************** Rotary Encoder
  pinMode(2, INPUT);           // set pin to input
  pinMode(3, INPUT);           // set pin to input
  
  digitalWrite(2, HIGH);       // turn on pullup resistors
  digitalWrite(3, HIGH);       // turn on pullup resistors
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
  
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
  
  pinMode(18, INPUT);           // set pin to input
  pinMode(19, INPUT);           // set pin to input
  
  digitalWrite(18, HIGH);       // turn on pullup resistors
  digitalWrite(19, HIGH);       // turn on pullup resistors
  //Setting up interrupt
  //A rising pulse from encodenren activated ai4(). AttachInterrupt 4 is DigitalPin nr 18 on moust Arduino.
  attachInterrupt(4, ai4, RISING);
  
  //B rising pulse from encodenren activated ai5(). AttachInterrupt 5 is DigitalPin nr 19 on moust Arduino.
  attachInterrupt(5, ai5, RISING);
  
  
  //************************************************************* distance sensor set up
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPin4, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin4, INPUT); // Sets the echoPin as an Input

  //clears trigpins
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);
  //*******************************************Transfor pipe set up
  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.enableDynamicPayloads();
  radio.setPayloadSize(128);
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.openWritingPipe(pipes[0]);
  radio.startListening(); 
  radio.powerUp();
  radio.stopListening();
  
  //****************************************************************gps software serial
  //ss.begin(9600);

}

void loop() {

  
  //*******************************************************GPS stuff
  /*
  gps.f_get_position(&data2.flat, &data2.flon, &age);
  gps.stats(&chars, &sentences, &failed);
  gps.crack_datetime(&year, &month, &day, &data2.hour, &data2.minute, &data2.second, &data2.hundredths, &age);
  data2.mph = gps.f_speed_mps();
  */
  //***********************************************read raw accel/gyro/mag measurements from device 
  accelGyroMag.getMotion9(&data.ax, &data.ay, &data.az, &data.gx, &data.gy, &data.gz, &data.mx, &data.my, &data.mz);
  
  //******************************************print all the stuff
 
  Serial.print(data2.rotaryencoder); Serial.print("\t");
  Serial.print(data2.rotaryencoder2); Serial.print("\t");
  Serial.print(data.ax); Serial.print("\t");
  Serial.print(data.ay); Serial.print("\t");
  Serial.print(data.az); Serial.print("\t");
  Serial.print(data.gx); Serial.print("\t");
  Serial.print(data.gy); Serial.print("\t");
  Serial.print(data.gz); Serial.print("\t");
  Serial.print(data.mx); Serial.print("\t");
  Serial.print(data.my); Serial.print("\t");
  Serial.print(data.mz); Serial.print("\t");
 // Serial.print(data2.flat, 6); Serial.print("\t");
 // Serial.print(data2.flon, 6);Serial.print("\t");
 // Serial.print(data2.hour);Serial.print(":");Serial.print(data2.minute);Serial.print(":");Serial.print(data2.second);Serial.print(":");Serial.print(data2.hundredths);Serial.print("\t");
 // Serial.print(data2.mph);Serial.print("\t");
  Serial.print(data.distance1);Serial.print("\t");
  Serial.print(data.distance2);Serial.print("\t");
  Serial.print(data.distance3);Serial.print("\t");
  Serial.print(data.distance4);Serial.println("\t");
  
  //******************************rotartencoder to reset after one rotation
  if(data2.rotaryencoder >2000){
    data2.rotaryencoder = 1;
  }
  if(data2.rotaryencoder < 0){
    data2.rotaryencoder = 1999;
  }
  if(data2.rotaryencoder2 >2000){
    data2.rotaryencoder2 = 1;
  }
  if(data2.rotaryencoder2 < 0){
    data2.rotaryencoder2 = 1999;
  }
  
  //******************************pipe writes
  delay(13);
  radio.openWritingPipe(pipes[0]);
  radio.writeFast(&data,sizeof(data));
  
  radio.openWritingPipe(pipes[1]);
  radio.writeFast(&data2,sizeof(data2));
  delay(13);

  //smartdelay(50);
  
  //************************distance stuff
  // Clears the trigPin
 // digitalWrite(trigPin1, LOW);
  //delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH,5000);
  // Calculating the distance
  data.distance1= duration1*0.034/2;

    // Clears the trigPin
 // digitalWrite(trigPin2, LOW);
 // delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH,5000);
  // Calculating the distance
  data.distance2= duration2*0.034/2;

    // Clears the trigPin
 // digitalWrite(trigPin3, LOW);
 // delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration3 = pulseIn(echoPin3, HIGH,5000);
  // Calculating the distance
  data.distance3= duration3*0.034/2;

    // Clears the trigPin
 // digitalWrite(trigPin4, LOW);
  //delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin4, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration4 = pulseIn(echoPin4, HIGH,5000);
  // Calculating the distance
  data.distance4 = duration4*0.034/2;
  
}

//******************************************
//for roatry encoder 1
//********************************************
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
    data2.rotaryencoder++;
  }else{
    data2.rotaryencoder--;
  }
}
//******************************************
//for roatry encoder 1
//********************************************
void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
    data2.rotaryencoder--;
  }else{
    data2.rotaryencoder++;
  }
}
//******************************************
//for roatry encoder 2
//********************************************
void ai4() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(18)==LOW) {
    data2.rotaryencoder2 ++;
  }else{
    data2.rotaryencoder2 --;
  }
}
//******************************************
//for roatry encoder 2
//********************************************
void ai5() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(19)==LOW) {
    data2.rotaryencoder2 --;
  }else{
    data2.rotaryencoder2 ++;
  }
}
//****************************for GPS code
/*
static void smartdelay(unsigned long ms)
{
  
  unsigned long start = millis();
  do 
  {
    while (ss.available()){
      gps.encode(ss.read());
    }
  } while (millis() - start < ms);

}
*/
