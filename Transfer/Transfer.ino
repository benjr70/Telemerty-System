#include <SPI.h>
#include "RF24.h"

//****************************radio object and pins
RF24 radio(8,10);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.


struct data{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t mx, my, mz;
  int distance1, distance2, distance3, distance4;
 
};

struct data2{
  float flat, flon, mph;
  byte hour, minute, second, hundredths;
  int rotaryencoder = 0; 
  int rotaryencoder2 = 0;
};

struct data data;
struct data2 data2;

void setup(void) {

  Serial.begin(115200);

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
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  radio.powerUp();                        //Power up the radio
}

void loop(void){

  while(radio.available()){     
    radio.openReadingPipe(2,pipes[0]); 
    radio.read(&data,sizeof(data));
    radio.openReadingPipe(1,pipes[1]);
    radio.read(&data2, sizeof(data2));
  }
  delay(50);
/*
  Serial.print(data2.rotaryencoder); Serial.print("--");
  Serial.print(data2.rotaryencoder2); Serial.print("--");
  Serial.print(data.ax); Serial.print("--");
  Serial.print(data.ay); Serial.print("--");
  Serial.print(data.az); Serial.print("--");
  Serial.print(data.gx); Serial.print("--");
  Serial.print(data.gy); Serial.print("--");
  Serial.print(data.gz); Serial.print("--");
  Serial.print(data.mx); Serial.print("--");
  Serial.print(data.my); Serial.print("--");
  Serial.print(data.mz); Serial.print("--");
  Serial.print(data.flat); Serial.print("--");
  Serial.print(data.flon);Serial.print("--");
  Serial.print(data2.hour);Serial.print("--");
  Serial.print(data2.minute);Serial.print("--");
  Serial.print(data2.second);Serial.print("--");
  Serial.print(data2.hundredths);Serial.print("--");
  Serial.print(data.mph);Serial.print("--");
    Serial.print(data.distance1);Serial.print("--");
  Serial.print(data.distance2);Serial.print("--");
  Serial.print(data.distance3);Serial.print("--");
  Serial.print(data.distance4);Serial.print("--");
  Serial.println("E");
*/
  Serial.print(data2.rotaryencoder);Serial.print("\t");
  Serial.print(data2.rotaryencoder2); Serial.print("\t");
  Serial.print(data.ax); Serial.print("\t");
  Serial.print(data.ay); Serial.print("\t");
  Serial.print(data.az); Serial.print("\t");
  Serial.print(data.gx);Serial.print("\t");
  Serial.print(data.gy); Serial.print("\t");
  Serial.print(data.gz); Serial.print("\t");
  Serial.print(data.mx); Serial.print("\t");
  Serial.print(data.my); Serial.print("\t");
  Serial.print(data.mz); Serial.print("\t");
  Serial.print(data2.flat, 6);Serial.print("\t");
  Serial.print(data2.flon, 6);Serial.print("\t");
  Serial.print(data2.hour);Serial.print(":");
  Serial.print(data2.minute);Serial.print(":");
  Serial.print(data2.second);Serial.print(":");
  Serial.print(data2.hundredths);Serial.print("\t");
  Serial.print(data2.mph);Serial.print("\t");
  Serial.print(data.distance1);Serial.print("\t");
  Serial.print(data.distance2);Serial.print("\t");
  Serial.print(data.distance3);Serial.print("\t");
  Serial.print(data.distance4);Serial.print("\t");
  
  Serial.println("E");
}
