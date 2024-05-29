# Description

## BOM





## Firmware setup

VS Code

PlatformIO with dependencies

USB serial permissions

if USB device not found remove "apt remove brltty"

## Development notes

### I2C addresses

* 0x36 - MAX17048  - Fuel Gauge, Battery monitoring (on dev board)
* 0x48 - ADS1115  - ADC
* 0x49 - ADS1115 - (If A0/Addrees pin Soldered to HIGH) - ADC
* 0x76 - MS5803-14BA - Pressure sensor
*

Can use Adafruit TestBed > I2CScan example to test/detect all I2C devices. [https://learn.adafruit.com/scanning-i2c-addresses/arduino-using-testbed](https://learn.adafruit.com/scanning-i2c-addresses/arduino-using-testbed)

## Useful links / References



<details>

<summary>SparkFun Thing Plus - ESP32 WROOM (USB-C)</summary>

* Sparkfun [https://www.sparkfun.com/products/20168](https://www.sparkfun.com/products/20168)
* Schematic [https://cdn.sparkfun.com/assets/5/9/7/4/1/SparkFun\_Thing\_Plus\_ESP32-WROOM\_C\_schematic2.pdf](https://cdn.sparkfun.com/assets/5/9/7/4/1/SparkFun\_Thing\_Plus\_ESP32-WROOM\_C\_schematic2.pdf)
* Graphical datasheet [https://cdn.sparkfun.com/assets/3/9/5/f/e/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C\_graphical\_datasheet2.pdf](https://cdn.sparkfun.com/assets/3/9/5/f/e/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C\_graphical\_datasheet2.pdf)
* GitHub Hardware repositore with firmware test for hardware features [https://github.com/sparkfun/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C](https://github.com/sparkfun/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C)

</details>

* SparkFun Thing Plus - ESP32 WROOM (USB-C)
  * Sparkfun [https://www.sparkfun.com/products/20168](https://www.sparkfun.com/products/20168)
  * Schematic [https://cdn.sparkfun.com/assets/5/9/7/4/1/SparkFun\_Thing\_Plus\_ESP32-WROOM\_C\_schematic2.pdf](https://cdn.sparkfun.com/assets/5/9/7/4/1/SparkFun\_Thing\_Plus\_ESP32-WROOM\_C\_schematic2.pdf)
  * Graphical datasheet [https://cdn.sparkfun.com/assets/3/9/5/f/e/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C\_graphical\_datasheet2.pdf](https://cdn.sparkfun.com/assets/3/9/5/f/e/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C\_graphical\_datasheet2.pdf)
  * GitHub Hardware repositore with firmware test for hardware features [https://github.com/sparkfun/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C](https://github.com/sparkfun/SparkFun\_Thing\_Plus\_ESP32\_WROOM\_C)
* SparkFun Pressure Sensor Breakout - MS5803-14BA
  * Sparkfun [https://www.sparkfun.com/products/12909](https://www.sparkfun.com/products/12909)
  * Schematic - [https://cdn.sparkfun.com/datasheets/Sensors/Weather/MS5803-14BA\_Breakout\_v10.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Weather/MS5803-14BA\_Breakout\_v10.pdf)
  * Datasheet [https://cdn.sparkfun.com/datasheets/Sensors/Weather/ms5803\_14ba.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Weather/ms5803\_14ba.pdf)
* ADS115 Adafruit
  * downloads [https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/downloads](https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/downloads)
  * Pinouts [https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/pinouts](https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/pinouts)
* Gravity: Analog Turbidity Sensor for Arduino
  * DFRobot  [https://www.dfrobot.com/product-1394.html](https://www.dfrobot.com/product-1394.html)
  * DFRobot wiki  [https://wiki.dfrobot.com/Turbidity\_sensor\_SKU\_\_SEN0189](https://wiki.dfrobot.com/Turbidity\_sensor\_SKU\_\_SEN0189)
  * Sensor [https://dfimg.dfrobot.com/nobody/wiki/8e585d98aafe2bab22be39c5b68165c5.pdf](https://dfimg.dfrobot.com/nobody/wiki/8e585d98aafe2bab22be39c5b68165c5.pdf)
  * Schematic [https://github.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/blob/master/SEN0189%20Turbidity%20sensor%28V1.0%29%20schematic.pdf](https://github.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/blob/master/SEN0189%20Turbidity%20sensor\(V1.0\)%20schematic.pdf)
  * Probe [https://raw.githubusercontent.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/master/Probe\_Dimension.png](https://raw.githubusercontent.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/master/Probe\_Dimension.png)
  * Adapter [https://raw.githubusercontent.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/master/Adapter\_Dimension.png](https://raw.githubusercontent.com/Arduinolibrary/DFRobot\_Turbidity\_sensor/master/Adapter\_Dimension.png)
* Gravity: Analog TDS Sensor/ Meter for Arduino
  * DFRobot [https://www.dfrobot.com/product-1662.html](https://www.dfrobot.com/product-1662.html)
  * DFRobot wiki [https://wiki.dfrobot.com/Gravity\_\_Analog\_TDS\_Sensor\_\_\_Meter\_For\_Arduino\_SKU\_\_SEN0244](https://wiki.dfrobot.com/Gravity\_\_Analog\_TDS\_Sensor\_\_\_Meter\_For\_Arduino\_SKU\_\_SEN0244)
  * Schematic [https://raw.githubusercontent.com/Arduinolibrary/Gravity\_Analog\_TDS\_Sensor\_For\_Arduino/master/Analog%20TDS%20Sensor(V1.0)%20Schematic.pdf](https://raw.githubusercontent.com/Arduinolibrary/Gravity\_Analog\_TDS\_Sensor\_For\_Arduino/master/Analog%20TDS%20Sensor\(V1.0\)%20Schematic.pdf)
  * Adapter layout [https://raw.githubusercontent.com/Arduinolibrary/Gravity\_Analog\_TDS\_Sensor\_For\_Arduino/master/Analog%20TDS%20Sensor(V1.0)%20Layout.pdf](https://raw.githubusercontent.com/Arduinolibrary/Gravity\_Analog\_TDS\_Sensor\_For\_Arduino/master/Analog%20TDS%20Sensor\(V1.0\)%20Layout.pdf)
  * GitHub library [https://github.com/DFRobot/GravityTDS](https://github.com/DFRobot/GravityTDS)
