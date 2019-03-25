#include <RHReliableDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>

#define SERVER_ADDRESS 1

RH_RF69 rf69;
RHReliableDatagram manager(rf69, SERVER_ADDRESS);

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  if (!manager.init())
    Serial.println("init failed");
}

void loop() {
  if (manager.available()) {
    uint8_t len = sizeof(buf);
    uint8_t from;

    if (manager.recvfromAck(buf, &len, &from)) {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
      // Send a reply back to the originator client
      if (!manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }
  }
}