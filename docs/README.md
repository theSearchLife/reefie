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

<summary></summary>



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
