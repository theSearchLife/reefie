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
#include <SparkFun_MS5803_I2C.h> // Click here to get the library: http://librarymanager/All#SparkFun_MS5803-14BA
#include "RTClib.h" // Date and time functions using a DS3231 RTC connected via I2C and Wire lib


#define HEADER "time,temperature,do_adc,do_volt,do_saturation,turb_adc,turb_volt,turb_ntu,conductivity,tds,salinity,specific_gravity\n"
#define DATA_TEMPLATE "%lu,%.3f,%u,%.4f,%.2f,%d,%.4f,%d,\n" // Template for formatting data

// Global Variables
File dataFile;
String fileName;
int fileIndex = 0;
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



MS5803 sensor(ADDRESS_HIGH); // Instantiate the sensor using ADDRESS_HIGH
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

Ezo_board EC = Ezo_board(100, "EC");      //create an EC circuit object who's address is 100 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"

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

const int numLog = 14;
String dataNames[numLog] = {"unix_timestamp", "human_timestamp", "temperature", "do_volt", "do_saturation", "turb_volt", 
                                  "turb_ntu", "conductivity", "tds", "salinity", "specific_gravity", 
                                  "pressure_abs", "pressure_relative", "altitude_delta"};

// Function Declarations
// SD Card functions
bool initializeSDCard();
String generateFileName();
void createBaseDirectory();
void createNewFile(const String &name);
void appendDataToFile(const String &name);
void writeHeader(const String &name);

double sealevel(double P, double A);
double altitude(double P, double P0);



void step1(); //forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();

Sequencer3 Seq( &step1, 815, //calls the steps in sequence with time in between them
                &step2, 815, 
                &step3, 2000); 

//Unit conversions
void voltageToTurbidity(float* voltage, float* turbidity);
float doSaturation(float calibration, float reading);

int16_t turbidity_adc, do_adc, ref_adc;
float turbidity_volts, do_volts, turbidity, ref_volt;
float do_saturation;
float volts_sensor;

//Create variables to store results
float temperature_c, temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;

// Create Variable to store altitude in (m) for calculations;
double base_altitude = 384.0; // in (m)

void setup() {
  // Start Serial USB logging
  Serial.begin(SERIAL_BAUD_RATE);

  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()){
    // Get compile time
    DateTime compileTime(F(__DATE__), F(__TIME__));

    // Subtract 2 hours (7200 seconds) to adjust to UTC 0 timezone
    DateTime adjustedTime = compileTime - TimeSpan(7200);

    // Adjust the RTC time
    rtc.adjust(adjustedTime);
  }

  // Initialize SD card and create new file
  if (initializeSDCard()) {
    createBaseDirectory();
    fileName = generateFileName();
    createNewFile(fileName);
    writeHeader(fileName);
  }

  if (!ads.begin(ADDR_ADS1115_1, &Wire)) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  sensor.reset();
  // Begin communication with the sensor
  // sensor.begin(); // Begin the sensor using Wire
  sensor.begin(Wire, 0x76); // Or, from v1.1.3, we can also do this
  pressure_baseline = sensor.getPressure(ADC_4096);

  Seq.reset();                  //initialize the sequencer
  delay(3000);
  EC.send_cmd("o,tds,1");        //send command to enable TDS output
  delay(300);
  EC.send_cmd("o,s,1");        //send command to enable salinity output
  delay(300);
  EC.send_cmd("o,sg,1");      //send command to enable specific gravity output
  delay(300);

}

  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

void loop() {  
  

  ads.setGain(GAIN_FOUR);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  float multiplier = 0.0625F;
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // turbidity_adc = ads.readADC_Differential_2_3();

  // Function to average sensor values
  int numSamples = 100;
  int sum = 0.0;
  for (int i = 0; i < numSamples; i++) {
      sum += ads.readADC_Differential_2_3();
  }
  turbidity_adc =  sum / numSamples;


  // turbidity_adc = ads.readADC_SingleEnded(2);
  // turbidity_volts = turbidity_adc * multiplier/1000;
  turbidity_volts = ads.computeVolts(turbidity_adc);
  
  volts_sensor = (turbidity_volts + 1.65)*2;
  voltageToTurbidity(&volts_sensor, &turbidity);

  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  do_adc = ads.readADC_SingleEnded(DO_PIN);
  do_volts = ads.computeVolts(do_adc);

  

  do_saturation = doSaturation(DO_CALIBRATION, do_volts);

  // Serial.print("Turbidity ADC: "); Serial.print(turbidity_adc); Serial.print("  "); Serial.print(volts_sensor,5); Serial.print("V");
  // Serial.print("  "); Serial.print(turbidity,4);Serial.println("NTU");
  // Serial.print("DO ADC: "); Serial.print(do_adc); Serial.print("  "); Serial.print(do_volts,6); Serial.println("V");
  // appendDataToFile(fileName);

  // Read temperature from the sensor in deg C. This operation takes about
  temperature_c = sensor.getTemperature(CELSIUS, ADC_512);
  // Read temperature from the sensor in deg F. Converting
  // to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);
  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);
  // Let's do something interesting with our data.
  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.
  pressure_relative = sealevel(pressure_abs, base_altitude);
  // Taking our baseline pressure at the beginning we can find an approximate
  // change in altitude based on the differences in pressure.
  altitude_delta = altitude(pressure_abs , pressure_baseline);


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

String formatDateTime(DateTime dateTime) {
  char dateTimeString[20]; // Buffer to hold the formatted date and time string

  // Format the date and time
  snprintf(dateTimeString, sizeof(dateTimeString), "%04d-%02d-%02dT%02d:%02d:%02d",
           dateTime.year(), dateTime.month(), dateTime.day(),
           dateTime.hour(), dateTime.minute(), dateTime.second());

  // Convert the C-string to a String object and return it
  return String(dateTimeString);
}

void createBaseDirectory(){
  if (!SD.exists("/data")) {
    if (!SD.mkdir("/data")) {
      Serial.println("Failed to create base directory: /data");
      return;
    }else{
      Serial.println("Created base directory: /data");
    }
  }else{
    Serial.println("Base directory exists: /data");
  }
}

String generateFileName() {
  String name;
  DateTime time_now = rtc.now();
  char folderName[11];
  char fileName[23];

  while (true) {
    snprintf(folderName, sizeof(folderName), "%04d-%02d-%02d",
          time_now.year(), time_now.month(), time_now.day());
    snprintf(fileName, sizeof(fileName), "%04d-%02d-%02d_%02d-%02d_%03d",
          time_now.year(), time_now.month(), time_now.day(),
          time_now.hour(), time_now.minute(), fileIndex);
    name = "/data/" + String(folderName) + "/" + String(fileName) + ".csv";

    if (!SD.exists(name)) {
      break;
    }
    fileIndex++;
  }
  return name;
}

void createNewFile(const String &name) {
  // Extract the directory path
  int lastSlashIndex = name.lastIndexOf('/');
  String dirPath = name.substring(0, lastSlashIndex);

  // Check if the directory exists and create it if it doesn't
  if (!SD.exists(dirPath)) {
    if (!SD.mkdir(dirPath)) {
      Serial.println("Failed to create directory: " + dirPath);
      return;
    }else{
      Serial.println("Created directory: " + dirPath);
    }
  } else{
    Serial.println("Directory exists: " + dirPath);
  }

  dataFile = SD.open(name, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Failed to create file: " + name);
    return;
  }
  dataFile.flush();
  dataFile.close();
  Serial.println("Created file: " + name);
}

// {"time", "temperature", "do_volt", "do_saturation", "turb_volt", 
//                                   "turb_ntu", "conductivity", "tds", "salinity", "specific_gravity", 
//                                   "pressure_abs", "pressure_relative", "altitude_delta"};

void appendDataToFile(const String &name) {
  dataFile = SD.open(name, FILE_APPEND);
  DateTime now = rtc.now();
  String dateTimeString = formatDateTime(now);
  
  if (dataFile) {
    dataFile.print(now.unixtime());dataFile.print(",");
    dataFile.print(dateTimeString);dataFile.print(",");
    dataFile.print(RTD_temp);dataFile.print(",");
    dataFile.print(do_volts);dataFile.print(",");
    dataFile.print(do_saturation);dataFile.print(",");
    dataFile.print(volts_sensor);dataFile.print(",");
    dataFile.print(turbidity);dataFile.print(",");
    dataFile.print(EC_float);dataFile.print(",");
    dataFile.print(TDS_float);dataFile.print(",");
    dataFile.print(SAL_float);dataFile.print(",");
    dataFile.print(SG_float);dataFile.print(",");
    dataFile.print(pressure_abs);dataFile.print(",");
    dataFile.print(pressure_relative);dataFile.print(",");
    dataFile.print(altitude_delta);
    dataFile.println(); // Example value to append
    dataFile.close();
    Serial.println("Data written to file: " + name);
  } else {
    Serial.println("Failed to open file for appending");
  }
}

void writeHeader(const String &name)
{
  dataFile = SD.open(name, FILE_APPEND);
  if (dataFile) {
    for (int i = 0; i < numLog; i++) {
      dataFile.print(dataNames[i]);
      if (i < numLog - 1) {
        dataFile.print(",");
      }
    }

    dataFile.println(); // Example value to append
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
  // receive_and_print_reading(RTD);             //get the reading from the RTD circuit
  RTD.receive_read_cmd();

  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {        //if the temperature reading has been received and it is valid
    RTD_temp = RTD.get_last_received_reading();
    EC.send_read_with_temp_comp(RTD_temp);                               //send readings from temp sensor to EC sensor
    
  } else {                                                                                      //if the temperature reading is invalid
    EC.send_read_with_temp_comp(25.0);                                                          //send default temp = 25 deg C to EC sensor
  }
  // Serial.print(" ");
}

void step3() {
  EC.receive_cmd(EC_data, 32);       //put the response into the buffer

  EC_str = strtok(EC_data, ",");       //let's parse the string at each comma.
  TDS = strtok(NULL, ",");                  //let's parse the string at each comma.
  SAL = strtok(NULL, ",");                  //let's parse the string at each comma 
  SG = strtok(NULL, ",");                  //let's parse the string at each comma.

  // receive_and_print_reading(EC);               //get the reading from the EC circuit and print it
  // Serial.println();

  EC_float=atof(EC_str);
  TDS_float=atof(TDS);
  SAL_float=atof(SAL);
  SG_float=atof(SG);

  Serial.print("Turbidity -");Serial.print(turbidity_adc);// ADC: "); Serial.print(turbidity_adc);
  Serial.print("  "); Serial.print(volts_sensor,5); Serial.print("V");
  Serial.print("  "); Serial.print(turbidity,4);Serial.print("NTU");
  Serial.print("  Dissolved Oxygen -");// ADC: "); Serial.print(do_adc);
  Serial.print("  "); Serial.print(do_volts,6); Serial.print("V");
  Serial.print("  "); Serial.print(do_saturation,2);Serial.print("%  ");
  Serial.print("RTD: ");
  Serial.print(RTD_temp);
  Serial.print("°C ");
  Serial.print("EC: ");                      //we now print each value we parsed separately.
  Serial.print(EC_str);                     //this is the EC value.

  Serial.print(" TDS: ");                  //we now print each value we parsed separately.
  Serial.print(TDS);                       //this is the TDS value.

  Serial.print(" SAL: ");                      //we now print each value we parsed separately.
  Serial.print(SAL);                       //this is the salinity point.
  
  Serial.print(" SG: ");                      //we now print each value we parsed separately.
  Serial.print(SG);                       //this is the specific gravity point.

  Serial.print("  TempC = ");
  Serial.print(temperature_c);

  Serial.print("  TempF = ");
  Serial.print(temperature_f);

  Serial.print("  Pressure abs (mbar)= ");
  Serial.print(pressure_abs);

  Serial.print("  Pressure rel (mbar)= ");
  Serial.print(pressure_relative);

  Serial.print("  Altitude change (m) = ");
  Serial.print(altitude_delta);

  Serial.println();
  appendDataToFile(fileName);
}

double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return (P / pow(1 - (A / 44330.0), 5.255));
}

double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}


