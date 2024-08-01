#include "reefie.h"


const int Reefie::numLog = 16;
const String Reefie::dataNames[Reefie::numLog] = {
    "unix_timestamp", "human_timestamp", "temperature", "do_volt", "do_saturation", "turb_volt", 
    "turb_ntu", "conductivity", "tds", "salinity", "specific_gravity", 
    "pressure_abs", "pressure_relative", "altitude_delta","battery_voltage","battery_soc"
};

Reefie::Reefie()
    :   lipo(MAX1704X_MAX17048), // Create a MAX17048
        battery_voltage(0),
        battery_soc(0),
        battery_alert(false),
        pressure_sensor(ADDRESS_HIGH),
        EC(Ezo_board(ADDR_EZO_EC, "EC")),
        RTD(Ezo_board(ADDR_EZO_RTD, "RTD")),
        state(STATE_INIT),
        readStates(READ_STATE_INIT),
        u8g2_l(U8G2_R0, /* reset=*/ U8X8_PIN_NONE),
        u8g2_r(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
        
{
  pressure_state.state_current = false;
  fuelgauge_state.state_current = false;
  rtc_state.state_current = false;
  sdCard_state.state_current = false;
  logfile_state.state_current = false;
  ads_state.state_current = false;
  ec_state.state_current = false;
  rtd_state.state_current = false;

  display_cycle = 0;
}

//Public functions
bool Reefie::begin(){
  // Start Serial USB logging
  Serial.begin(SERIAL_BAUD_RATE);
  
  Wire.begin();

  u8g2_r.setI2CAddress(ADDR_OLED_RIGHT); //Right Screen
  u8g2_r.begin();
  u8g2_l.setI2CAddress(ADDR_OLED_LEFT); //Left Screen
  u8g2_l.begin();
  u8g2_l.setFont(u8g2_font_ncenB10_tr);
  u8g2_r.setFont(u8g2_font_ncenB10_tr);
  displayUpdateInit();
  if(initFuelGauge()) fuelgauge_state.state_current = true;
  displayUpdateInit();
  if(initRTC()) rtc_state.state_current = true;
  displayUpdateInit();

  if(initSDCard())
  {
    sdCard_state.state_current = true;
    displayUpdateInit();
    return true;
  }else return false;
    
}

void Reefie::deepSleepStart(){
    Serial.println("ESP32 is going into deep sleep now, and will not wake up until manually power cycled.");
    // Ensure no wake-up source is set
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    // Delay to allow serial message to be sent
    delay(2000);
    // Enter deep sleep
    esp_deep_sleep_start();
}

//Private functions
bool Reefie::initSDCard(){
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return false;
  }
  Serial.println("SD Card initialized.");
  return true;
}

bool Reefie::initRTC(){
    if (!rtc.begin(&Wire)) {
    Serial.println("Couldn't find RTC");
    return false;
    }else{
      if (rtc.lostPower()){
          // Get compile time
          DateTime compileTime(F(__DATE__), F(__TIME__));
          // Subtract 2 hours (7200 seconds) to adjust to UTC 0 timezone
          DateTime adjustedTime = compileTime + TimeSpan(TIMEZONE_OFFSET);
          // Adjust the RTC time
          rtc.adjust(adjustedTime);
          Serial.println("RTC power lost, reinitialzied time");
      }
      return true;
    }
}

bool Reefie::initFuelGauge(){
  if (lipo.begin(Wire) == false){ // Connect to the MAX17043 using the default wire port
      Serial.println(F("MAX17043 not detected"));
      // deepSleepStart();
      return false;
  }else
  {
    // Quick start restarts the MAX17043 in hopes of getting a more accurate
    // guess for the SOC.
    lipo.quickStart();
    // We can set an interrupt to alert when the battery SoC gets too low.
    // We can alert at anywhere between 1% - 32%:
    lipo.setThreshold(BATTERY_ALARM_TH); // Set alert threshold to 20%.
    Serial.print("Battery alarm Threshold set to: ");Serial.print(BATTERY_ALARM_TH);Serial.println("%");
    return true;
  }
}

void Reefie::setupLogFile(){
    createBaseDirectory();
    fileName = generateFileName();
    createNewFile(fileName);
    writeHeader(fileName);
    logfile_state.state_current = true;
}

bool Reefie::checkPressureSensor(TwoWire &wirePort) {
    // Begin communication with the sensor
    pressure_sensor.begin(Wire, 0x76); // Or, from v1.1.3, we can also do this

    // Attempt to read a coefficient from the sensor's PROM
    wirePort.beginTransmission(0x76);
    wirePort.write(CMD_PROM);
    if (wirePort.endTransmission() != 0) {
        return false;
    }

    // Assuming coefficient should not be zero, adjust this check as needed
    return true;
}

void Reefie::setupSensors(){

  // Begin communication with the sensor
    if(checkPressureSensor(Wire))
    {
      // pressure_sensor.begin(Wire, 0x76); // Or, from v1.1.3, we can also do this
      pressure_sensor.reset();
      pressure_baseline = pressure_sensor.getPressure(ADC_4096);
      pressure_state.state_current = true;
    }
    
      displayUpdateInit();

      if (!ads.begin(ADDR_ADS1115_1, &Wire)) {
      Serial.println("Failed to initialize ADS.");
    }else{
      ads_state.state_current = true;
    }
    displayUpdateInit();

    Serial.println("Configuring Conductivity sensor");
    EC.send_cmd("k,1.0");        //send command to Set probe type
    delay(300);
    EC.send_cmd("o,ec,1");        //send command to enable EC output
    delay(300);
    EC.send_cmd("o,tds,1");        //send command to enable TDS output
    delay(300);
    EC.send_cmd("o,s,1");        //send command to enable salinity output
    delay(300);
    EC.send_cmd("o,sg,1");      //send command to enable specific gravity output
    delay(300);

    char receive_buffer[32];                  //buffer used to hold each boards response
    

    Serial.print("Testing EC - ");
    EC.send_read_cmd();
    delay(1000);
    EC.receive_cmd(receive_buffer, 32);   //put the response into the buffer
    delay(300);
    if(EC.get_error() == Ezo_board::SUCCESS) ec_state.state_current = true;
    Serial.println(EC.get_error());
    displayUpdateInit();

    Serial.print("Testing RTD - ");
    if(RTDreadLoop()) rtd_state.state_current = true;
    Serial.println(RTD_temp);
    displayUpdateInit();
    
}

bool Reefie::RTDreadLoop(){
  read_attempts = 0;
  bool result = true;
    do{
      RTD.send_read_cmd();
      delay(600);
      RTD.receive_read_cmd();
      read_attempts++;
    }while(RTD.get_error() != Ezo_board::SUCCESS && read_attempts!=RTD_READ_ATTEMPTS);
    if(read_attempts == RTD_READ_ATTEMPTS) result = false;
    RTD_temp = RTD.get_last_received_reading();
    return result;
}

void Reefie::createBaseDirectory(){
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

String Reefie::generateFileName() {
  String name;
  DateTime time_now = rtc.now();
  char folderName[11];
  char fileName[23];
  int fileIndex = 0;

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

void Reefie::createNewFile(const String &name) {
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

void Reefie::writeHeader(const String &name)
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

void Reefie::parseEZOData()
{
  EC_str = strtok(EC_data, ",");       //let's parse the string at each comma.
  TDS = strtok(NULL, ",");                  //let's parse the string at each comma.
  SAL = strtok(NULL, ",");                  //let's parse the string at each comma 
  SG = strtok(NULL, ",");                  //let's parse the string at each comma.

  EC_float=atof(EC_str);
  TDS_float=atof(TDS);
  SAL_float=atof(SAL);
  SG_float=atof(SG);
}

void Reefie::printSensorData()
{
  Serial.print("Turbidity -");Serial.print(turbidity_adc);// ADC: "); Serial.print(turbidity_adc);
  Serial.print("  "); Serial.print(turbidity_volts,5); Serial.print("V");
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
  Serial.print("  Battery Voltage (V) = ");
  Serial.print(battery_voltage);
  Serial.print("  Battery soc (%) = ");
  Serial.print(battery_soc);
  Serial.println();
}

void Reefie::readBattery()
{
  // lipo.getVoltage() returns a voltage value (e.g. 3.93)
  battery_voltage = lipo.getVoltage();
  // lipo.getSOC() returns the estimated state of charge (e.g. 79%)
  battery_soc = lipo.getSOC();
  // lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
  battery_alert = lipo.getAlert();
}

void Reefie::readAnalogSensors()
{
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
  turbidity_volts_divided = ads.computeVolts(turbidity_adc);
  turbidity_volts = (turbidity_volts_divided + 1.65)*2;
  voltageToTurbidity(&turbidity_volts, &turbidity);

  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  do_adc = ads.readADC_SingleEnded(DO_PIN);
  do_volts = ads.computeVolts(do_adc);
  do_saturation = doSaturation(DO_CALIBRATION, do_volts);
}

void Reefie::voltageToTurbidity(float* voltage, float* turbidity){
  *turbidity = -1120.4*(*voltage)*(*voltage) + 5742.3*(*voltage) - 4352.9;
}

float Reefie::doSaturation(float calibration, float reading)
{
  float saturation;
  saturation = reading/calibration * 100;
  return saturation;
}

void Reefie::readPressureSensor()
{
    // Read temperature from the sensor in deg C. This operation takes about
  temperature_c = pressure_sensor.getTemperature(CELSIUS, ADC_512);
  // Read temperature from the sensor in deg F. Converting
  // to Fahrenheit is not internal to the sensor.
  // Additional math is done to convert a Celsius reading.
  temperature_f = pressure_sensor.getTemperature(FAHRENHEIT, ADC_512);
  // Read pressure from the sensor in mbar.
  pressure_abs = pressure_sensor.getPressure(ADC_4096);
  // Let's do something interesting with our data.
  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.
  pressure_relative = sealevel(pressure_abs, BASELINE_ALTITUDE);
  // Taking our baseline pressure at the beginning we can find an approximate
  // change in altitude based on the differences in pressure.
  altitude_delta = altitude(pressure_abs , pressure_baseline);
}

double Reefie::sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return (P / pow(1 - (A / 44330.0), 5.255));
}

double Reefie::altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return (44330.0 * (1 - pow(P / P0, 1 / 5.255)));
}

String Reefie::formatDateTime(DateTime dateTime) {
  char dateTimeString[20]; // Buffer to hold the formatted date and time string

  // Format the date and time
  snprintf(dateTimeString, sizeof(dateTimeString), "%04d-%02d-%02dT%02d:%02d:%02d",
           dateTime.year(), dateTime.month(), dateTime.day(),
           dateTime.hour(), dateTime.minute(), dateTime.second());

  // Convert the C-string to a String object and return it
  return String(dateTimeString);
}

String Reefie::formatDateTimeDisplay(DateTime dateTime) {
  char dateTimeString[20]; // Buffer to hold the formatted date and time string

  // Format the date and time
  snprintf(dateTimeString, sizeof(dateTimeString), "%04d-%02d-%02d  %02d:%02d",
           dateTime.year(), dateTime.month(), dateTime.day(),dateTime.hour(), dateTime.minute());

  // Convert the C-string to a String object and return it
  return String(dateTimeString);
}

void Reefie::appendDataToFile(const String &name) {
  dataFile = SD.open(name, FILE_APPEND);
  DateTime now = rtc.now();
  String dateTimeString = formatDateTime(now);
  
  if (dataFile) {
    dataFile.print(now.unixtime());dataFile.print(",");
    dataFile.print(dateTimeString);dataFile.print(",");
    dataFile.print(RTD_temp);dataFile.print(",");
    dataFile.print(do_volts);dataFile.print(",");
    dataFile.print(do_saturation);dataFile.print(",");
    dataFile.print(turbidity_volts);dataFile.print(",");
    dataFile.print(turbidity);dataFile.print(",");
    dataFile.print(EC_float);dataFile.print(",");
    dataFile.print(TDS_float);dataFile.print(",");
    dataFile.print(SAL_float);dataFile.print(",");
    dataFile.print(SG_float);dataFile.print(",");
    dataFile.print(pressure_abs);dataFile.print(",");
    dataFile.print(pressure_relative);dataFile.print(",");
    dataFile.print(altitude_delta);dataFile.print(",");
    dataFile.print(battery_voltage);dataFile.print(",");
    dataFile.print(battery_soc);
    dataFile.println(); // Example value to append
    dataFile.close();
    Serial.println("Data written to file: " + name);
  } else {
    Serial.println("Failed to open file for appending");
    displaySDCardError();
    deepSleepStart();
  }
}

void Reefie::displaySDCardError()
{
  u8g2_l.clearBuffer();
  u8g2_l.setCursor(0, 12);
  u8g2_l.print(F("NO SD CARD"));
  u8g2_l.setCursor(0, 25);
  u8g2_l.print(F("ESP32 SLEEPS"));
  u8g2_l.setCursor(0, 38);
  u8g2_l.print(F("MANUALY"));
  u8g2_l.setCursor(0, 51);
  u8g2_l.print(F("POWER"));
  u8g2_l.setCursor(0, 63);
  u8g2_l.print(F("CYCLE"));
  u8g2_l.sendBuffer();

  u8g2_r.clearBuffer();
  u8g2_r.sendBuffer();
}

void Reefie::displayUpdateInit()
{
  u8g2_l.clearBuffer();

  u8g2_l.setCursor(0, 12);
  u8g2_l.print(F("Fuel Gauge"));
  u8g2_l.setCursor(120, 12);
  u8g2_l.print(fuelgauge_state.state_current);

  u8g2_l.setCursor(0, 25);
  u8g2_l.print(F("RTC"));
  u8g2_l.setCursor(120, 25);
  u8g2_l.print(rtc_state.state_current);

  if(rtc_state.state_current){
    DateTime now = rtc.now();
    String dateTimeString = formatDateTimeDisplay(now);
    u8g2_l.setCursor(0, 38);
    u8g2_l.print((dateTimeString));
  }

  u8g2_l.setCursor(0, 50);
  u8g2_l.print(F("SD Card"));
  u8g2_l.setCursor(120, 50);
  u8g2_l.print(sdCard_state.state_current);

  u8g2_l.setCursor(0, 63);
  u8g2_l.print(F("Pressure"));
  u8g2_l.setCursor(120, 63);
  u8g2_l.print(pressure_state.state_current);

  u8g2_l.sendBuffer();

  u8g2_r.clearBuffer();

  u8g2_r.setCursor(0, 12);
  u8g2_r.print(F("ADS1115"));
  u8g2_r.setCursor(120, 12);
  u8g2_r.print(ads_state.state_current);

  u8g2_r.setCursor(0, 25);
  u8g2_r.print(F("Conductivity"));
  u8g2_r.setCursor(120, 25);
  u8g2_r.print(ec_state.state_current);

  u8g2_r.setCursor(0, 38);
  u8g2_r.print(F("RTD"));
  u8g2_r.setCursor(120, 38);
  u8g2_r.print(rtd_state.state_current);

  u8g2_r.sendBuffer();

  delay(300);
}

void Reefie::displayData()
{
  u8g2_l.clearBuffer();
  u8g2_l.setCursor(0, 12);
  u8g2_l.print(F("Turb: "));
  u8g2_l.print(turbidity_volts);
  u8g2_l.setCursor(0, 25);
  u8g2_l.print(F("DO: "));
  u8g2_l.print(do_saturation);
  u8g2_l.setCursor(0, 38);
  u8g2_l.print(F("Temp: "));
  u8g2_l.print(RTD_temp);
  u8g2_l.setCursor(0, 51);
  u8g2_l.print(F("EC: "));
  u8g2_l.print(EC_str);
  u8g2_l.setCursor(0, 64);
  u8g2_l.print(F("TDS: "));
  u8g2_l.print(TDS);
  u8g2_l.sendBuffer();

  u8g2_r.clearBuffer();	
  u8g2_r.setCursor(0, 12);
  u8g2_r.print(F("SAL: "));
  u8g2_r.print(SAL);
  u8g2_r.setCursor(0, 25);
  u8g2_r.print(F("SG: "));
  u8g2_r.print(SG);
  u8g2_r.setCursor(0, 38);
  u8g2_r.print(F("Pressure: "));
  u8g2_r.print(pressure_abs);
  u8g2_r.setCursor(0, 51);
  u8g2_r.print(F("Depth: "));
  u8g2_r.print(pressure_abs);
  u8g2_r.setCursor(0, 64);
  u8g2_r.print(F("Battery: "));
  u8g2_r.print(battery_soc);
  u8g2_r.sendBuffer();
  
}

void Reefie::displayBothEyes(int index_left, int index_right, int pause)
{
  u8g2_r.clearBuffer();
  u8g2_r.drawXBM(0,0,128,64,right_eyeallArray[index_right]);
  u8g2_r.sendBuffer();
  u8g2_l.clearBuffer();
  u8g2_l.drawXBM(0,0,128,64,left_eyeallArray[index_left]);
  u8g2_l.sendBuffer();
  delay(pause);
}

void Reefie::displayEyesInit()
{
  displayBothEyes(0,0,500);
  displayBothEyes(6,7,500);
  displayBothEyes(7,6,500);
  displayBothEyes(5,5,1000);
}

void Reefie::displayEyesCycle()
{
  displayBothEyes(3,3,500);
  displayBothEyes(1,1,1000);
  // displayBothEyes(5,6,500);
  displayBothEyes(4,4,2000);
}

void Reefie::displayEyesCycleRandom()
{

  switch (display_cycle)
  {
  case 0:
    displayBothEyes(6,7,500);
    displayBothEyes(3,4,500);
    displayBothEyes(1,1,500);
    displayBothEyes(0,0,500);
    break;
  case 1:
    displayBothEyes(0,0,500);
    displayBothEyes(4,3,500);
    displayBothEyes(1,1,500);
    displayBothEyes(0,0,500);
    break;
  case 2:
    displayBothEyes(7,6,500);
    displayBothEyes(6,7,500);
    displayBothEyes(7,6,500);
    displayBothEyes(5,5,500);
    break;
  case 3:
    displayBothEyes(0,0,500);
    displayBothEyes(0,1,500);
    displayBothEyes(0,0,1000);
    break;
  case 4:
    displayBothEyes(0,0,500);
    displayBothEyes(0,1,500);
    displayBothEyes(1,0,500);
    displayBothEyes(0,0,500);
    break;
  case 5:
    displayBothEyes(0,0,500);
    displayBothEyes(4,3,500);
    displayBothEyes(1,1,500);
    displayBothEyes(0,0,500);
    break;
  case 6:
    displayBothEyes(7,6,500);
    displayBothEyes(4,3,500);
    displayBothEyes(1,1,500);
    displayBothEyes(0,0,500);
    break;
  default:
    displayBothEyes(0,0,500);
    displayBothEyes(1,1,1000);
    displayBothEyes(4,3,1000);
    break;
  }
  display_cycle++;
  if(display_cycle==7) display_cycle = 0;
}