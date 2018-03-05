#include "Wire.h"


#define DEBUG
//*************************************************************************gps

#include <SoftwareSerial.h>

// Connect the GPS RX/TX to arduino pins 10 and 9
SoftwareSerial serial = SoftwareSerial(10,9);

const unsigned char UBX_HEADER[] = { 0xB5, 0x62 };

struct NAV_PVT {
  unsigned char cls;
  unsigned char id;
  unsigned short len;
  unsigned long iTOW;
  unsigned int year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  char valid;
  unsigned long tAcc;
  long nano;
  unsigned char fixType;
  char flags;
  char flags2;
  unsigned char numSV;
  long lon;
  long lat;
  long height;
  long hMSL;
  unsigned long hAcc;
  unsigned long vAcc;
  long ve1N;
  long ve1E;
  long ve1D;
  long gspeed;
  long headMot;
  unsigned long sAcc;
  unsigned long headAcc;
  unsigned int pDOP;
  unsigned char reserved1[6];
  long headVeh;
  int magDec;
  unsigned int magAcc;
};

NAV_PVT pvt;

//******************************************************************************

//hourFormat12();
// I2Cdev and MPU9150 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
 
#include "MPU9150.h"
#include "helper_3dmath.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU9150 accelGyroMag;


int s=0;
int sec=0;
int hrs=0;
int minutes=0;
int initialHours = 0;// = pvt.hour; //variable to initiate hours
int initialMins = 0;// = pvt.min;//variable to initiate minutes
int initialSecs = 0;// = pvt.sec;//variable to initiate seconds

struct data{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;
  int16_t distance1, distance2,distance3,distance4;
  
};

struct data2{
  float flat, flon, mph;
  byte hour, minute, second, hundredths;
  int rotaryencoder = 0; 
  int rotaryencoder2 = 0;
};

struct data data;
struct data2 data2;
int porte;
//********************************************************pipe and radio stuff
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8); 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };  
  


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
  
  //*******************************************Transfor pipe set up
  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
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
  serial.begin(9600);

}

void loop() {
  //********************************time stuff
 
 data2.hour = hours();
 data2.minute = mins();
 data2.second = secs();
 data2.hundredths = millis() % 1000;



  //*******************************************************GPS stuff
  
  if(processGPS()){
  data2.flat = pvt.lat/10000000.0f;
  data2.flon = pvt.lon/10000000.0f;
  data2.mph =  pvt.gspeed;
   // Serial.print("Speed:");      Serial.print(pvt.gspeed);
   // Serial.print(" lat/lon: "); Serial.print(pvt.lat/10000000.0f); Serial.print(","); Serial.print(pvt.lon/10000000.0f);
   // Serial.print(" height: ");  Serial.print(pvt.height/1000.0f);
   // Serial.print(" hMSL: ");    Serial.print(pvt.hMSL/1000.0f);
   // Serial.print(" hAcc: ");    Serial.print(pvt.hAcc/1000.0f);
   // Serial.print(" vAcc: ");    Serial.print(pvt.vAcc/1000.0f);
    //Serial.println("gps updated");
  }
  
  //***********************************************read raw accel/gyro/mag measurements from device 
  accelGyroMag.getMotion9(&data.ax, &data.ay, &data.az, &data.gx, &data.gy, &data.gz, &data.mx, &data.my, &data.mz);

  data.mx = data.mx - (236+404); // substract mag offset to the x and y
  data.my = data.my - 6923;
  
  //******************************************print all the stuff

#ifdef DEBUG
  //Serial.print(data2.rotaryencoder); Serial.print("\t");
  //Serial.print(data2.rotaryencoder2); Serial.print("\t");
  
 // Serial.print(data.ax); Serial.print("\t");
 // Serial.print(data.ay); Serial.print("\t");
 // Serial.print(data.az); Serial.print("\t");
 // Serial.print(data.gx); Serial.print("\t");
 // Serial.print(data.gy); Serial.print("\t");
//  Serial.print(data.gz); Serial.print("\t");
  Serial.print(data.mx); Serial.print("\t");
  Serial.print(data.my); Serial.println("\t");
 // Serial.print(data.mz); Serial.print("\t");
//  Serial.print(data2.flat, 6); Serial.print("\t");
//  Serial.print(data2.flon, 6);Serial.print("\t");
 // Serial.print(data2.mph);Serial.print("\t");
//  Serial.print(mins());Serial.print(":");Serial.print(secs());Serial.print(".");Serial.print(millis()%1000);Serial.println("\t");
  //Serial.print(data2.hour);Serial.print(":");Serial.print(data2.minute);Serial.print(":");Serial.print(data2.second);Serial.print(".");Serial.print(data2.hundredths);Serial.print("\t");
#endif
  
  //******************************rotartencoder to reset after one rotation
//  if(data2.rotaryencoder >2000){
//    data2.rotaryencoder = 1;
//  }
//  if(data2.rotaryencoder < 0){
//    data2.rotaryencoder = 1999;
//  }
//  if(data2.rotaryencoder2 >2000){
//    data2.rotaryencoder2 = 1;
//  }
//  if(data2.rotaryencoder2 < 0){
//    data2.rotaryencoder2 = 1999;
//  }

//  data2.rotaryencoder %= 2000;
//  data2.rotaryencoder2 %= 2000;
  
  //******************************pipe writes
 
  //delay(13);
  radio.openWritingPipe(pipes[0]);
  radio.writeFast(&data,sizeof(data));
  
  radio.openWritingPipe(pipes[1]);
  radio.writeFast(&data2,sizeof(data2));
  //delay(13);

}

//******************************************
//for roatry encoder 1
//********************************************
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction

  
  if(PORTE == 48) {
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

void calcChecksum(unsigned char* CK) {
  memset(CK, 0, 2);
  for (int i = 0; i < (int)sizeof(NAV_PVT); i++) {
    CK[0] += ((unsigned char*)(&pvt))[i];
    CK[1] += CK[0];
  }
}

bool processGPS() {
  static int fpos = 0;
  static unsigned char checksum[2];
  const int payloadSize = sizeof(NAV_PVT);
  
  while ( serial.available() ) {
    byte c = serial.read();
    if ( fpos < 2 ) {
      if ( c == UBX_HEADER[fpos] )
        fpos++;
      else
        fpos = 0;
    }
    else {
      if ( (fpos-2) < payloadSize )
        ((unsigned char*)(&pvt))[fpos-2] = c;

      fpos++;

      if ( fpos == (payloadSize+2) ) {
        calcChecksum(checksum);
      }
      else if ( fpos == (payloadSize+3) ) {
        if ( c != checksum[0] )
          fpos = 0;
      }
      else if ( fpos == (payloadSize+4) ) {
        fpos = 0;
        if ( c == checksum[1] ) {
          return true;
        }
      }
      else if ( fpos > (payloadSize+4) ) {
        fpos = 0;
      }
    }
  }
  return false;
}

int seconds()
{
    s = initialHours*3600;
    s = s+(initialMins*60);
    s = s+initialSecs;
    s = s+(millis()/1000);
    return s;
}
//this method is for hours
int hours()
{
    hrs = seconds();
    hrs = hrs/3600;
    hrs = hrs%24;
    return hrs;
}
//this method is for minutes
int mins()
{
    minutes = seconds();
    minutes = minutes/60;
    minutes = minutes%60;
    return minutes;
}
 
int secs()
{
    sec = seconds();
    sec = sec%60;
    return sec;
}
