#include <SPI.h>
#include "RF24.h"

//****************************radio object and pins
RF24 radio(8,10);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.


struct data{
  float lat,lon;
  int ax,ay,az;
  
};
struct data2{
  int yaw, pitch, roll;
};

struct data data;
struct data2 data2;
int i = 0;
void setup(void) {

  Serial.begin(115200);

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
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();  
  radio.powerUp();         //Power up the radio
}

void loop(void){

  while(radio.available()){     
    radio.openReadingPipe(2,pipes[0]); 
    radio.read(&data,sizeof(data));
    radio.openReadingPipe(1,pipes[1]);
    radio.read(&data2,sizeof(data2));
    Serial.print(data.lat);Serial.print("\t");
    Serial.print(data.lon);Serial.print("\t");
    Serial.print(data.ax);Serial.print("\t");
    Serial.print(data.ay);Serial.print("\t");
    Serial.print(data.az);Serial.print("\t");
    Serial.print(data2.roll);Serial.print("\t");
    Serial.print(data2.pitch);Serial.print("\t");
    Serial.print(data2.yaw);Serial.println("\t");
  }
  
  delay(5);

   
   


}
