#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
// #include "FS.h" // To be used for more complex file managing

#include "ap_config.h"

// Global Variables
File dataFile;
String fileName;
int fileIndex = 0;

// Function Declarations
// SD Card functions
bool initializeSDCard();
String generateFileName();
void createNewFile(const String &name);
void appendDataToFile(const String &name);

void setup() {
  // Start Serial USB logging
  Serial.begin(SERIAL_BAUD_RATE);

  // Initialize SD card and create new file
  if (initializeSDCard()) {
    fileName = generateFileName();
    createNewFile(fileName);
  }
}

void loop() {
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
