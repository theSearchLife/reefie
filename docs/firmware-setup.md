# Firmware setup

## Firmware setup

VS Code

PlatformIO with dependencies

USB serial permissions

if USB device not found remove "apt remove brltty"

Depending on IDE need to manually configure to download mode\
\
Configure EZO probes to I2C mode



### Programming RTC

{% embed url="https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/arduino-usage" %}

Time set to UTC. As in the example compiling device timezone was UTC+2, therefore subtracted 7200 seconds to stre UTC 0.

````arduino
```cpp

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

void setup () {
  Serial.begin(115200);


  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

   if (rtc.lostPower()) {
     Serial.println("RTC lost power, let's set the time!");
     // When time needs to be set on a new device, or after a power loss, the
     // following line sets the RTC to the date & time this sketch was compiled
     // Get compile time
    DateTime compileTime(F(__DATE__), F(__TIME__));
    // Subtract 2 hours (7200 seconds)
    DateTime adjustedTime = compileTime - TimeSpan(7200);
    // Adjust the RTC time
    rtc.adjust(adjustedTime);
}

void loop () {
}
```
````





## Development notes

### I2C addresses

* 0x36 - MAX17048 - Fuel Gauge, Battery monitoring (on dev board)
* 0x48 - ADS1115 - ADC
* 0x49 - ADS1115 - (If A0/Addrees pin Soldered to HIGH) - ADC
* 0x64 - EZO-EC - Embedded Conductivity Circuit
* 0x66 - EZO-RTD Embedded Temperature Circuit
* 0x76 - MS5803-14BA - Pressure sensor

Can use Adafruit TestBed > I2CScan example to test/detect all I2C devices. [https://learn.adafruit.com/scanning-i2c-addresses/arduino-using-testbed](https://learn.adafruit.com/scanning-i2c-addresses/arduino-using-testbed)

