/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** General Data Transfer Rate Test
 * This example demonstrates basic data transfer functionality with the 
 updated library. This example will display the transfer rates acheived using
 the slower form of high-speed transfer using blocking-writes.
 */


#include <SPI.h>
#include "RF24.h"

/*************  USER Configuration *****************************/
                                          // Hardware configuration
RF24 radio(8,10);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

/***************************************************************/

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.

double data[32];                           //Data buffer for testing data transfer speeds
double data2[32];  

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
  
  for(int i=0; i<32; i++){
     data[i] = 0;               //Load the buffer with random data
  }
  radio.powerUp();                        //Power up the radio
}

void loop(void){


     while(radio.available()){     
      radio.openReadingPipe(2,pipes[0]); 
      radio.read(&data,120);
      radio.openReadingPipe(1,pipes[1]);
      radio.read(&data2,120);
     }
     delay(50);
     
     Serial.print(data[0]);  Serial.print("--");
     Serial.print(data[1]); Serial.print("--"); 
     Serial.print(data[2]); Serial.print("--");
     Serial.print(data[3]); Serial.print("--");
     Serial.print(data[4]); Serial.print("--");
     Serial.print(data[5]); Serial.print("--");
     Serial.print(data[6]); Serial.print("--");
     Serial.print(data[7]); Serial.print("--");
    
    
     Serial.print(data2[0]); Serial.print("--");
     Serial.print(data2[1]); Serial.print("--");
     Serial.print(data2[2]); Serial.print("--");
     Serial.print(data2[3]); Serial.print(":");
     Serial.print(data2[4]); Serial.print(":");
     Serial.print(data2[5]); Serial.print("--");
     Serial.print(data2[6]); Serial.print("--");
      Serial.print(data2[7]); Serial.print("--");
     Serial.println("E");

}
