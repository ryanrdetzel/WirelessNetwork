#include <SPI.h>
#include <Wire.h>
#include <BMx280MI.h>

#define BMP_280_I2C_ADDRESS 0x76

BMx280I2C bmx280(BMP_280_I2C_ADDRESS);

float temperature = 0;
float humidity = 0;

int reading_delay = 1000;

void init(int reading_delay=1000) {
    reading_delay = reading_delay;

    Wire.begin();
    /* BMP280 */
    if (!bmx280.begin()) {
        Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
        while (1);
    }

    //reset sensor to default parameters.
    bmx280.resetToDefaults();
    bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
    if (bmx280.isBME280())
        bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

    pinMode(5, INPUT_PULLUP); // NEEDED?
}

void get_temperature(){
  if (!bmx280.measure()){
    Serial.println("could not start measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish
  do{
    delay(100);
  } while (!bmx280.hasValue());

  temperature = bmx280.getTemperature();
  humidity = bmx280.getHumidity();
}