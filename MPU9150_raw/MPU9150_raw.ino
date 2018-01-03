#include "Wire.h"

//*************************************************************************gps
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(10,9);
static void smartdelay(unsigned long ms);
float print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
//******************************************************************************

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

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define LED_PIN 13
bool blinkState = false;

int counter = 0; // for rotary encoder

//****************************
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8); 
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };    
double data[32];
double data2[32];
//*******************************************************************

void setup() {

  //******************************************************************* for Accel/gryo
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelGyroMag.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelGyroMag.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");

    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);

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
//*************************************************************

     //*******************************************Transfor
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
     //******************************************

    //****************************************************************gps
    ss.begin(9600);
   //**********************************************
}

void loop() {

  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.f_get_position(&flat, &flon, &age);
  gps.stats(&chars, &sentences, &failed);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
 // flat = print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
 // flon = print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    // read raw accel/gyro/mag measurements from device
    accelGyroMag.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelGyroMag.getAcceleration(&ax, &ay, &az);
    //accelGyroMag.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro/mag x/y/z values
//  Serial.print("a/g/m:\t");

  Serial.print(counter); Serial.print("\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
 Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.print(gz); Serial.print("\t");
   Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.print(mz); Serial.print("\t");
    Serial.print(flat); Serial.print("\t");
    Serial.print(flon);Serial.print("\t");
    Serial.print(hour);Serial.print(":");Serial.print(minute);Serial.print(":");Serial.print(second);Serial.print("\t");
    Serial.print(gps.f_speed_mps());Serial.println("\t");

  if(counter >2000){
    counter = 1;
  }
  if(counter < 0){
    counter = 1999;
  }
    radio.openWritingPipe(pipes[0]);
    data[0] = counter;
    data[1] = ax;
    data[2] = ay;
    data[3] = az; 
    data[4] = gx;
    data[5] = gy;
    data[6] = gz;
    data[7] = mx;
    
    radio.writeFast(&data,120);
    radio.openWritingPipe(pipes[1]);
    data2[0] = my;
    data2[1] = mz;
    data2[2] = flat;
    data2[3] = flon;
    data2[4] = hour;
    data2[5] = minute;
    data2[6] = second;
    data2[7] = gps.f_speed_mps();
    radio.writeFast(&data2,120);
   // Serial.print(mag); Serial.print("\t");
  //  for (int i=0; i<mag; i++)
 //       Serial.print("*");
//    Serial.print("\n");


    //delay(50);
    smartdelay(50);
}

//******************************************
//for roatry encoder
//********************************************
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
    counter++;
  }else{
    counter--;
  }
}
//******************************************
//for roatry encoder
//********************************************
void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
    counter--;
  }else{
    counter++;
  }
}

//****************************From GPS code
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

float print_float(float val, float invalid, int len, int prec)
{
  char latlon[32];
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    sprintf(latlon,"%f%d", val, prec);
    Serial.print(latlon);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
  
}
//***********************end from GPS code
