#ifndef REEFIE_STATE_H
#define REEFIE_STATE_H

#include <Arduino.h>

#include "ap_config.h"

#include <SD.h>
#include <SPI.h>
// #include "FS.h" // To be used for more complex file managing
#include <Wire.h>    //include arduinos i2c library
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
#include "RTClib.h" // Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <SparkFun_MS5803_I2C.h> // Click here to get the library: http://librarymanager/All#SparkFun_MS5803-14BA
#include <Adafruit_ADS1X15.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Ezo_i2c_util.h> //brings in common print statements
#include <U8g2lib.h>
#include "oled_images.h"


typedef struct{
    bool state_current;
    bool state_changed;
}device_state_t;

typedef enum{
    STATE_INIT = 0,
    STATE_READ_DATA,
    STATE_LOG_DATA,
    STATE_WAITING,
    STATE_ERROR,
    STATE_TESTS
} appState_t;

typedef enum{
    READ_STATE_INIT = 0,
    READ_STATE_READ_TEMP,
    READ_STATE_READ_EC,
    READ_STATE_READ_OTHER
} readStates_t;

class Reefie{
public:
    Reefie(); //Consturctor
    bool begin();
    void deepSleepStart();
    void setupLogFile();
    void setupSensors();
    void parseEZOData();

    void printSensorData();
    void readBattery();
    void readAnalogSensors();
    void readPressureSensor();
    void appendDataToFile(const String &name);

    void displayData();
    void displayEyesInit();
    void displayEyesCycle();
    void displaySDCardError();
    void displayEyesCycleRandom();
    void displayUpdateInit();

    void readConfigFile();

    bool RTDreadLoop();

    int16_t turbidity_adc;
    float turbidity_volts_divided, turbidity_volts, turbidity;

    int16_t do_adc;
    float do_volts, do_saturation;
    
    Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
    appState_t state;
    readStates_t readStates;
    SFE_MAX1704X lipo;
    RTC_DS3231 rtc;
    double battery_voltage; // Variable to keep track of LiPo voltage
    double battery_soc; // Variable to keep track of LiPo state-of-charge (SOC)
    bool battery_alert; // Variable to keep track of whether alert has been triggered.
    MS5803 pressure_sensor; // Instantiate the sensor using ADDRESS_HIGH

    Ezo_board EC; //= Ezo_board(100, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
    Ezo_board RTD; //= Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"

    // Static constants
    static const int numLog;
    static const String dataNames[];
    String fileName;

    char EC_data[32];          //we make a 32-byte character array to hold incoming data from the EC sensor.
    char *EC_str;                     //char pointer used in string parsing.
    char *TDS;                       //char pointer used in string parsing.
    char *SAL;                       //char pointer used in string parsing (the sensor outputs some text that we don't need).
    char *SG;                       //char pointer used in string parsing.

    float EC_float;               //float var used to hold the float value of the conductivity.
    float TDS_float;                 //float var used to hold the float value of the total dissolved solids.
    float SAL_float;                 //float var used to hold the float value of the salinity.
    float SG_float;                 //float var used to hold the float value of the specific gravity.
    float RTD_temp;

    float temperature_c, temperature_f;
    double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;

    device_state_t pressure_state;
    device_state_t fuelgauge_state;
    device_state_t rtc_state;
    device_state_t sdCard_state;
    device_state_t logfile_state;
    device_state_t ads_state;
    device_state_t ec_state;
    device_state_t rtd_state;

    // U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2_l; //Single page mode
    // U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2_r; //Single page mode

    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_l; //Full Buffer mode
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_r; //Full Buffer mode

    int battery_alarm_th_config;
    uint32_t logging_delay_config;
    float baseline_altitude_config;
    float do_calibration_config;
    
private:
    File dataFile;

    int display_cycle;
    
    bool initSDCard();
    bool initRTC();
    bool initFuelGauge();
    void createBaseDirectory();
    String generateFileName();
    void createNewFile(const String &name);
    void writeHeader(const String &name);
    void createDefaultConfigFile(const char* configFileName);
    void parseConfigLine(String line);
    void voltageToTurbidity(float* voltage, float* turbidity);
    float doSaturation(float calibration, float reading);
    double sealevel(double P, double A);
    double altitude(double P, double P0);
    String formatDateTime(DateTime dateTime);
    String formatDateTimeDisplay(DateTime dateTime);

    void displayBothEyes(int index_left, int index_right, int pause);

    bool checkPressureSensor(TwoWire &wirePort);
    int read_attempts;
    

};

#endif