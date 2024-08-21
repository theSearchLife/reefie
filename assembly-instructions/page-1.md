---
description: Preparation of sensors
---

# Sensors

ressure

<details>

<summary>MS583730BA01-50</summary>

**Components**

* Sensor [https://www.digikey.it/en/products/detail/te-connectivity-measurement-specialties/MS583730BA01-50/5629501?s=N4IgTCBcDaILIGUCsAOAzAdjQBgEIEFsBGAWiWxAF0BfIA](https://www.digikey.it/en/products/detail/te-connectivity-measurement-specialties/MS583730BA01-50/5629501?s=N4IgTCBcDaILIGUCsAOAzAdjQBgEIEFsBGAWiWxAF0BfIA)
* 3D printed casing
* Cable&#x20;
* SMD 0.1uF Capacitor
* o-ring
* Flexible Urethane 8800
  * Syringe to inject liquid inside casing

**Steps**

* 3D print
  * File name:
    * MS583730BA01-50-Enclosure\_inner
    * MS583730BA01-50- Enclosure\_outer
  * Print settings
    * 6 Layer walls
    * 100% infill
  * Material: PETG

<img src="../.gitbook/assets/Screenshot from 2024-07-30 16-39-47.png" alt="" data-size="original">

* Solder capacitor and cable

<img src="../.gitbook/assets/2020_0110_161123_004.JPG" alt="" data-size="original">![](../.gitbook/assets/2020\_0110\_165156\_006.JPG)

* Apply conformal coating on soldering
* Install o-ring to sensor
* Clean cable with IPA
* Insert sensor to casing

![](<../.gitbook/assets/Screenshot from 2024-07-30 17-30-42.png>)

* Mount external casing

![](<../.gitbook/assets/Screenshot from 2024-07-30 17-31-30.png>)

* Apply tape on the end to prevent leaking

![](<../.gitbook/assets/Screenshot from 2024-07-30 17-31-54.png>)

* Fill casing with Urethane (using syringe)

![](<../.gitbook/assets/Screenshot from 2024-07-30 17-32-18.png>)

* Cure it



</details>

<details>

<summary>MS580314BA01-00</summary>

**Components**

* Sensor
* 3D printed
  * MS580314BA01-00-Enclosure\_inner
  * MS580314BA01-00 - Enclosure\_outer
* Cable
* Flexible Urethane 8800

**Steps**

* 3D print
  * 6 Layer walls
  * 100% infill
  * PETG

<img src="../.gitbook/assets/Screenshot from 2024-07-30 17-22-37.png" alt="" data-size="original">

* Solder capacitor and cable
* Apply conformal coating
* Clean cable with IPA
* Insert sensor to casing

<img src="../.gitbook/assets/Screenshot from 2024-07-30 17-46-16.png" alt="" data-size="original">

* Mount external casing
* Apply tape to prevent Urethane leaking

<img src="../.gitbook/assets/Screenshot from 2024-07-30 17-48-16.png" alt="" data-size="original">

* Fill casing with Urethane
* Cure itdone

<img src="../.gitbook/assets/Screenshot from 2024-07-30 17-48-44.png" alt="" data-size="original">

</details>

## Turbidity sensor

* Sensor [https://www.dfrobot.com/product-1394.html](https://www.dfrobot.com/product-1394.html)
* 3D printed Casing
  * File name: Turbidity\_Cover
  * Print settings:
    * 6 Layer wall
    * 100% infill
    * 10mm Brim width
  * Material: PETG

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-30 17-58-01.png" alt=""><figcaption></figcaption></figure>

* Desolder JST connector

<div>

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-30 19-55-05 (1).png" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-30 19-55-03.png" alt=""><figcaption></figcaption></figure>

</div>

* Solder the cable
  * Red - VCC
  * Black - GND
  * Green - Signal

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-30 19-56-50.png" alt=""><figcaption></figcaption></figure>

* Hot Glue the wires to keep them in place
* Fill hot glue around sensor are to seal it, preventing Urethane filling sensor are

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-30 20-43-11.png" alt=""><figcaption></figcaption></figure>

* Install casing
* Fill with Urethane

<figure><img src="../.gitbook/assets/signal-2024-07-30-214602.jpeg" alt=""><figcaption></figcaption></figure>

## Temperature

### Assemble EZO™ Carrier Board

<div>

<figure><img src="../.gitbook/assets/PXL_20240530_123209979 (2).jpg" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/PXL_20240530_123338038 (2).jpg" alt=""><figcaption></figcaption></figure>

</div>

<div>

<figure><img src="../.gitbook/assets/PXL_20240530_123438771 (2).jpg" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/PXL_20240530_123451866 (1).jpg" alt=""><figcaption></figcaption></figure>

</div>

<figure><img src="../.gitbook/assets/PXL_20240530_123528584 (1).jpg" alt=""><figcaption></figcaption></figure>

### EZOboard I2C mode selection

{% hint style="warning" %}
Must change communication mode to I2c
{% endhint %}

It can be done easily buy connecting pins to the power source using jumper wires.\
Also, guide in here&#x20;

{% embed url="https://www.instructables.com/UART-AND-I2C-MODE-SWITCHING-FOR-ATLAS-SCIENTIFIC-E/" %}

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 19-09-56.png" alt=""><figcaption></figcaption></figure>

### Calibration

Using one of the Demos example send calibration command with known temperature

Best with boiling water at sea level for 100 degrees C

{% embed url="https://github.com/Atlas-Scientific/Ezo_I2c_lib/tree/master/Examples/I2c_lib_examples" %}

> **i2c\_scan\_and\_comms\_example:** scans the I2C line for an ezo device (lowest address thats on the line) and lets you send commands to it using the serial terminal. Use the "ADDR" command to change that devices' address.
>
> **iot\_cmd\_sample\_code:** large example that demonstrates polling 3 devices for readings and also lets you send commands to any of them from the serial terminal. Use the "HELP" command for more details

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 19-55-03.png" alt=""><figcaption></figcaption></figure>

## Dissolved Oxygen

[https://files.atlas-scientific.com/Mini\_DO\_probe.pdf](https://files.atlas-scientific.com/Mini\_DO\_probe.pdf)

<figure><img src="../.gitbook/assets/image (6).png" alt=""><figcaption></figcaption></figure>

Enter value into the config file

## Conductivity

#### Change mode to to I2C

<figure><img src="../.gitbook/assets/image (8).png" alt=""><figcaption></figcaption></figure>

#### Calibration

{% embed url="https://github.com/Atlas-Scientific/Ezo_I2c_lib/tree/master/Examples/I2c_lib_examples" %}

> **i2c\_scan\_and\_comms\_example:** scans the I2C line for an ezo device (lowest address thats on the line) and lets you send commands to it using the serial terminal. Use the "ADDR" command to change that devices' address.
>
> **iot\_cmd\_sample\_code:** large example that demonstrates polling 3 devices for readings and also lets you send commands to any of them from the serial terminal. Use the "HELP" command for more details

1. Add probe directly into a solution bag
2. Send command&#x20;

<figure><img src="../.gitbook/assets/image (7).png" alt=""><figcaption></figcaption></figure>







