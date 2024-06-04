#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
// #include "FS.h" // To be used for more complex file managing

#include "ap_config.h"

#include <Adafruit_ADS1X15.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>    //include arduinos i2c library
#include <sequencer3.h> //imports a 3 function sequencer
#include <Ezo_i2c_util.h> //brings in common print statements

#define HEADER "time,temperature,do_adc,do_volt,do_saturation,turb_adc,turb_volt,turb_ntu,conductivity,tds,salinity,specific_gravity\n"
#define DATA_TEMPLATE "%s,%.2f,%.2f\n" // Template for formatting data

// Global Variables
File dataFile;
String fileName;
int fileIndex = 0;

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

Ezo_board EC = Ezo_board(100, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"

// Function Declarations
// SD Card functions
bool initializeSDCard();
String generateFileName();
void createNewFile(const String &name);
void appendDataToFile(const String &name);
void appendHeaderData(const String &name);


void step1(); //forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();

Sequencer3 Seq( &step1, 815, //calls the steps in sequence with time in between them
                &step2, 815, 
                &step3, 2000); 

//Unit conversions
void voltageToTurbidity(float* voltage, float* turbidity);
float doSaturation(float calibration, float reading);


void setup() {
  // Start Serial USB logging
  Serial.begin(SERIAL_BAUD_RATE);

  // Initialize SD card and create new file
  if (initializeSDCard()) {
    fileName = generateFileName();
    createNewFile(fileName);
  }

  if (!ads.begin(ADDR_ADS1115_1)) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  Seq.reset();                            //initialize the sequencer

}

  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

void loop() {  
  int16_t turbidity_adc, do_adc, ref_adc;
  float turbidity_volts, do_volts, turbidity, ref_volt;

  ads.setGain(GAIN_FOUR);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  float multiplier = 0.0625F;
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  turbidity_adc = ads.readADC_Differential_2_3();
  // turbidity_adc = ads.readADC_SingleEnded(2);
  // turbidity_volts = turbidity_adc * multiplier/1000;
  turbidity_volts = ads.computeVolts(turbidity_adc);
  float volts_sensor;
  volts_sensor = (turbidity_volts + 1.65)*2;
  voltageToTurbidity(&volts_sensor, &turbidity);

  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  do_adc = ads.readADC_SingleEnded(DO_PIN);
  do_volts = ads.computeVolts(do_adc);

  float do_saturation;

  do_saturation = doSaturation(DO_CALIBRATION, do_volts);

  Serial.print("Turbidity ADC: "); Serial.print(turbidity_adc); Serial.print("  "); Serial.print(volts_sensor,5); Serial.print("V");
  Serial.print("  "); Serial.print(turbidity,4);Serial.println("NTU");
  Serial.print("DO ADC: "); Serial.print(do_adc); Serial.print("  "); Serial.print(do_volts,6); Serial.println("V");


  // appendDataToFile(fileName);
  Seq.run(); 

  delay(1000);
  // delay(LOGGING_DELAY); // Wait for 5 seconds before next write
}

bool initializeSDCard() {
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return false;
  }
  Serial.println("SD Card initialized.");
  return true;
}

String generateFileName() {
  String name;
  while (true) {
    name = "/data" + String(fileIndex) + ".csv";
    if (!SD.exists(name)) {
      break;
    }
    fileIndex++;
  }
  return name;
}

void createNewFile(const String &name) {
  dataFile = SD.open(name, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Failed to create file");
    return;
  }
  dataFile.println("Timestamp,Value"); // Write header to the new file
  dataFile.close();
  Serial.println("Created file: " + name);
}

void appendDataToFile(const String &name) {
  dataFile = SD.open(name, FILE_APPEND);
  if (dataFile) {
    dataFile.print(millis());
    dataFile.print(",");
    dataFile.println(random(100)); // Example value to append
    dataFile.close();
    Serial.println("Data written to file: " + name);
  } else {
    Serial.println("Failed to open file for appending");
  }
}

void appendHeaderData(const String &name)
{
  dataFile = SD.open(name, FILE_APPEND);
  if (dataFile) {
    dataFile.print(millis());
    dataFile.print(",");
    dataFile.println(random(100)); // Example value to append
    dataFile.close();
    Serial.println("Data written to file: " + name);
  } else {
    Serial.println("Failed to open file for appending");
  } 
}

void voltageToTurbidity(float* voltage, float* turbidity){
  *turbidity = -1120.4*(*voltage)*(*voltage) + 5742.3*(*voltage) - 4352.9;
}

float doSaturation(float calibration, float reading)
{
  float saturation;
  saturation = reading/calibration * 100;
  return saturation;
}

void step1() {
  //send a read command. we use this command instead of RTD.send_cmd("R"); 
  //to let the library know to parse the reading
  RTD.send_read_cmd();
}

void step2() {
  receive_and_print_reading(RTD);             //get the reading from the RTD circuit

  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {        //if the temperature reading has been received and it is valid
    EC.send_read_with_temp_comp(RTD.get_last_received_reading());                               //send readings from temp sensor to EC sensor
  } else {                                                                                      //if the temperature reading is invalid
    EC.send_read_with_temp_comp(25.0);                                                          //send default temp = 25 deg C to EC sensor
  }
  Serial.print(" ");
}

void step3() {
  receive_and_print_reading(EC);               //get the reading from the EC circuit and print it
  Serial.println();
}