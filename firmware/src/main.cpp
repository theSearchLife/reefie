#include <Arduino.h>
#include "reefie.h"

Reefie reefie;

// Timer handle
TimerHandle_t xOneShotTimer;
bool timerTriggered = false;

void vOneShotTimerCallback(TimerHandle_t xTimer){
  timerTriggered = true;
}

void readAllData();

void setup() {

  xOneShotTimer = xTimerCreate("OneShotTimer", 
                                pdMS_TO_TICKS(700),
                                pdFALSE,
                                NULL,
                                vOneShotTimerCallback);

  reefie.begin(); //Initialize I2C ,RTC, Fuel gauge, SD card
  reefie.setupLogFile(); //Create Folders and files for writing data.
  reefie.setupSensors(); // Initialize ADS1115, pressure sensor and EZO boards.

}

  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

void loop() {


  switch (reefie.state)
  {
    case STATE_INIT:
    {
      //Reserved state to reinitialize after errors etc
      reefie.state = STATE_READ_DATA;
      break;
    }
    case STATE_READ_DATA:
    {
      readAllData();
      break;
    }
    case STATE_LOG_DATA:
    {
      reefie.printSensorData();
      reefie.appendDataToFile(reefie.fileName);

      break;
    }
    case STATE_ERROR:
    {
      break;
    }
  }

   delay(LOGGING_DELAY); // Wait for 5 seconds before next write
}

void readAllData()
{
  switch (reefie.readStates)
  {
    case READ_STATE_INIT:
    {
      reefie.RTD.send_read_cmd();
      timerTriggered = false;
      xTimerChangePeriod(xOneShotTimer, pdMS_TO_TICKS(700), 0);
      reefie.readStates = READ_STATE_READ_TEMP;
      xTimerStart(xOneShotTimer, 0);
      break;
    }
    case READ_STATE_READ_TEMP:
    {
      if(timerTriggered){
        reefie.RTD.receive_read_cmd();

        if ((reefie.RTD.get_error() == Ezo_board::SUCCESS) && (reefie.RTD.get_last_received_reading() > -1000.0)) {        //if the temperature reading has been received and it is valid
          reefie.RTD_temp = reefie.RTD.get_last_received_reading();
          reefie.EC.send_read_with_temp_comp(reefie.RTD_temp);                               //send readings from temp sensor to EC sensor
          
        } else {                                                                                      //if the temperature reading is invalid
          reefie.EC.send_read_with_temp_comp(25.0);                                                          //send default temp = 25 deg C to EC sensor
        }
        
        xTimerChangePeriod(xOneShotTimer, pdMS_TO_TICKS(1000), 0);
        reefie.readStates = READ_STATE_READ_EC;
        xTimerStart(xOneShotTimer, 0);
        timerTriggered = false;

      }
      break;
    }
    case READ_STATE_READ_EC:
    {
      if(timerTriggered)
      {
        reefie.readStates = READ_STATE_READ_OTHER;
        timerTriggered = false;
        reefie.EC.receive_cmd(reefie.EC_data, 32);       //put the response into the buffer
        reefie.parseEZOData();
      }
      break;
    }
    case READ_STATE_READ_OTHER:
    {
      reefie.readBattery();
      reefie.readAnalogSensors();
      reefie.readPressureSensor();

      reefie.readStates = READ_STATE_INIT;
      reefie.state = STATE_LOG_DATA;
      break;
    }
  }
}