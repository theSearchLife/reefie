# Usage / Maintenance

## Data description

* **unix\_timestamp** - It shows the time in Unix format, representing the number of seconds that have elapsed since January 1, 1970 (UTC). This value is used for precise time-stamping of sensor data.
*   **human\_timestamp** - It shows the time in a human-readable format (e.g., YYYY-MM-DD HH:MM

    ), making it easier to understand the exact date and time when the data was recorded.
* **temperature** - It shows the water temperature in degrees Celsius (°C), an important factor that affects dissolved oxygen levels, conductivity, and other water quality parameters.
* **do\_volt** - It shows the voltage output from the dissolved oxygen (DO) sensor. This raw voltage value is used to calculate the concentration or saturation level of dissolved oxygen in the water.
* **do\_saturation** - It shows the percentage of oxygen saturation in the water. This value indicates the amount of dissolved oxygen relative to the maximum amount that water can hold at a specific temperature and pressure.
* **turb\_volt** - It shows the voltage output from the turbidity sensor. This raw voltage value is used to determine the turbidity of the water, which indicates how clear or murky it is.
* **turb\_ntu** - It shows the turbidity of the water in Nephelometric Turbidity Units (NTU). This value indicates the concentration of suspended particles in the water, which can affect light penetration and aquatic life. (Calibration is not accurate, use only for relative values)
* **conductivity** - It shows the electrical conductivity of the water in microsiemens per centimeter (µS/cm). This value indicates the water's ability to conduct electricity, which is influenced by the presence of dissolved salts and minerals.
* **tds** - It shows the total dissolved solids (TDS) in the water, typically measured in milligrams per liter (mg/L). This value represents the combined content of all inorganic and organic substances dissolved in the water.
* **salinity** - It shows the concentration of salt in the water, typically measured in parts per thousand (ppt) or practical salinity units (PSU). This value is important for understanding the water's chemical properties and its suitability for different aquatic species.
* **specific\_gravity** - It shows the specific gravity of the water, which is a dimensionless value indicating the density of the water relative to pure water. This value is affected by temperature, salinity, and other factors.
* **pressure\_abs** - It shows the absolute pressure in the water, (Bars). This value includes both the atmospheric pressure and the water pressure, useful for depth calculations.
* **depth** - It shows the depth of the water column, typically measured in meters (m). This value is derived from the pressure sensor and indicates how deep the sensor is below the water surface.
* **battery\_voltage** - It shows the voltage level of the sensor's battery, typically measured in volts (V). This value indicates the current power level of the sensor's battery.
* **battery\_soc** - It shows the state of charge (SOC) of the sensor's battery as a percentage (%). This value represents how much charge is left in the battery, with 100% being fully charged and 0% being fully depleted.

## Usage

### Opening Closing

* Unscrew pressure valve plug (blue) or insert vacuum adapter plug (red) to open the vent
  * so it's easier to remove or insert flanges

{% hint style="warning" %}
Ensure inside of the tube is grease as well as the o-rings

Areas cover with grease are clear out of any debris
{% endhint %}

{% hint style="danger" %}
Ensure no vacuum adapter is inserted before submerging
{% endhint %}

### Check sealing

It is recommended to check if it seals with manual vacuum pump before each submersion.&#x20;

Manual Vacuum pump MVM8900 \
[https://bluerobotics.com/store/watertight-enclosures/enclosure-tools-supplies/vacuum-pump-kit-r2-rp/](https://bluerobotics.com/store/watertight-enclosures/enclosure-tools-supplies/vacuum-pump-kit-r2-rp/)

[https://bluerobotics.com/learn/using-the-vacuum-test-plug/](https://bluerobotics.com/learn/using-the-vacuum-test-plug/)

### SD card

Remove or add SD card only when device is turned off.

### Config

File in the root directory "config.txt"

Default values

```cpp
LOGGING_DELAY=10000
BATTERY_ALARM_TH=20
BASELINE_ALTITUDE=384.0
DO_CALIBRATION=0.583250
```

Logging delay - milliseconds value for logging data period

Battery alarm threshold - battery percentage value to trigger an event (currently no event implemented). Battery value is displayed on OLED with other senor values as well as logged in the data files.

Baseline altitude - To be used only MS5803 Sensor is being used to calculate relative pressure. Currently MS5837 is used.

Dissolved oxygen calibration - Value of the sensor when sensor is outside water. Used as number for calculating % of dissolved oxygen.&#x20;

{% hint style="info" %}
DO\_CALIBRATION should be updated with the new value on location.
{% endhint %}

## Maintenance

### Sensors&#x20;

#### Turbidity

Clean with a soft sponge without damanging the plastic casing. Ensure there are no debris between two wings.

#### Conductivity

From [https://files.atlas-scientific.com/Mini\_EC\_K\_1.0\_probe.pdf](https://files.atlas-scientific.com/Mini\_EC\_K\_1.0\_probe.pdf)

<div>

<figure><img src=".gitbook/assets/Screenshot from 2024-08-21 17-17-52.png" alt=""><figcaption></figcaption></figure>

 

<figure><img src=".gitbook/assets/Screenshot from 2024-08-21 17-18-17.png" alt=""><figcaption></figcaption></figure>

</div>

#### Dissolved oxygen

From [https://files.atlas-scientific.com/Mini\_DO\_probe.pdf](https://files.atlas-scientific.com/Mini\_DO\_probe.pdf)

<div>

<figure><img src=".gitbook/assets/image (1).png" alt=""><figcaption></figcaption></figure>

 

<figure><img src=".gitbook/assets/image (2).png" alt=""><figcaption></figcaption></figure>

 

<figure><img src=".gitbook/assets/image (3).png" alt=""><figcaption></figcaption></figure>

 

<figure><img src=".gitbook/assets/image (4).png" alt=""><figcaption></figcaption></figure>

 

<figure><img src=".gitbook/assets/image (5).png" alt=""><figcaption></figcaption></figure>

</div>

### O-rings

{% hint style="danger" %}
Use plastic tools, NOT steel or other hard tools, to remove the O-rings from the sealing flange. Hard tools can create scrapes that will compromise sealing ability.
{% endhint %}

{% hint style="danger" %}
Always inspect the O-rings before operation in water and especially when replacing the end-caps, aluminum flanges, or penetrators. Any dust, hair, or other particles can cause a leak.
{% endhint %}
