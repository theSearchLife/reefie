#include <Arduino.h>
#include "reefie.h"

Reefie reefie;

// Timer handle
TimerHandle_t xOneShotTimer;
bool timerTriggered = false;

// TimerHandle_t xLoggingTimer;
// bool logTimerTriggered = false;

// void vLogTimerCallback(TimerHandle_t xTimer){
//   logTimerTriggered = true;
// }

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
  // reefie.setupLogFile(); //Create Folders and files for writing data.
  // reefie.setupSensors(); // Initialize ADS1115, pressure sensor and EZO boards.
  // reefie.state = STATE_TESTS;

}


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
      reefie.state = STATE_READ_DATA;
      delay(LOGGING_DELAY); // Wait for 5 seconds before next write

      break;
    }
    case STATE_ERROR:
    {
      break;
    }
    case STATE_TESTS:
    {
      Serial.println("Tests");

      break;
    }
  }
  delay(10);
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