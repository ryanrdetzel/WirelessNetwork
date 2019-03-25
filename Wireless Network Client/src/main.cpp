#include <RHReliableDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <LowPower.h>

#include "sensor_types.h"
#include "sensor_temperature.h"

#define CLIENT_ADDRESS 10
#define SERVER_ADDRESS 1

#define SENSOR_TYPE SENSOR_TEMPERATURE
//TODO read type and address (id) from eeprom

RH_RF69 rf69;
RHReliableDatagram manager(rf69, CLIENT_ADDRESS);

// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void setup() {
  Serial.begin(9600);
  while (!Serial) 
    ;
  if (!manager.init())
    Serial.println("init failed");
  //driver.setTxPower(14, true);

  rf69.sleep();
}

void loop() {
  rf69.sleep();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
  int data_size = 10; //Based off type

  uint8_t *data = new uint8_t[data_size];

  switch(SENSOR_TYPE){
    case SENSOR_TEMPERATURE:
      get_temperature();
    break;
  } 
  data[0] = SENSOR_TYPE;
  // type
  //uint8_t sensor_type = SENSOR_TYPE;

  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else {
      Serial.println("No reply, is rf69_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");

  delete [] data;
  delay(5000);
}