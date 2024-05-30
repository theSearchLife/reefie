#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
// #include "FS.h" // To be used for more complex file managing

#include "ap_config.h"

#include <Adafruit_ADS1X15.h>

// Global Variables
File dataFile;
String fileName;
int fileIndex = 0;

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

// Function Declarations
// SD Card functions
bool initializeSDCard();
String generateFileName();
void createNewFile(const String &name);
void appendDataToFile(const String &name);

//Unit conversions
void voltageToTurbidity(float* voltage, float* turbidity);

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

}

void loop() {  
  int16_t turbidity_adc, do_adc;
  float turbidity_volts, do_volts, turbidity;

  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  turbidity_adc = ads.readADC_SingleEnded(TURBIDITY_PIN);
  turbidity_volts = ads.computeVolts(turbidity_adc);
  voltageToTurbidity(&turbidity_volts, &turbidity);

  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  do_adc = ads.readADC_SingleEnded(DO_PIN);
  do_volts = ads.computeVolts(do_adc);

  appendDataToFile(fileName);
  delay(LOGGING_DELAY); // Wait for 5 seconds before next write
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

void voltageToTurbidity(float* voltage, float* turbidity){
  *turbidity = -1120.4*(*voltage)*(*voltage) + 5742.3*(*voltage) - 4352.9;
}