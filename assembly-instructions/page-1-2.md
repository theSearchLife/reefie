# Electronics wiring

## Custom cables

command for generating image from main directory

```
wireviz -f p -o docs/.gitbook/assets/ hardware/analog_harness.yaml
```

<figure><img src="../docs/.gitbook/assets/analog_harness.png" alt=""><figcaption></figcaption></figure>

## Prepared cables

Cable images from top to down from cable plan

<figure><img src="../.gitbook/assets/PXL_20240802_122926096.jpg" alt=""><figcaption></figcaption></figure>

<div>

<figure><img src="../.gitbook/assets/PXL_20240802_123036044.jpg" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/PXL_20240802_122956658.jpg" alt=""><figcaption></figcaption></figure>

</div>

<figure><img src="../.gitbook/assets/PXL_20240802_123128041.jpg" alt=""><figcaption></figcaption></figure>

## Turbidity voltage divider

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-02 15-56-28.png" alt=""><figcaption></figcaption></figure>

Stepping down turbidity output from around 3.5-4.2V to 1.75-2.1V.

Differential negative input is 1.65V. Using ADS1115 gain of 4. Measuring difference between 1.65-2.674V

<figure><img src="../.gitbook/assets/signal-2024-08-02-160024.jpeg" alt=""><figcaption></figcaption></figure>

Pinout is of X9 connector in cable schematic. Extra pins used to daisy chain power wires.

## OLED displays I2C address change

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-02 16-03-55.png" alt=""><figcaption></figcaption></figure>

Solder address resistor to the right to change default address of 0x78 to 0x7A. This should be done on the OLED on the right side (left side from the back view like the photo)

## ESP32 3V output to 5V converter

<figure><img src="../.gitbook/assets/PXL_20240802_123600984.jpg" alt=""><figcaption></figcaption></figure>

Solder 3V3 and GND pins to the JST female connector. Add hot glue or liquid tape to prevent wires from bending and breaking.&#x20;

## Power switch cable

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-02 16-09-46.png" alt=""><figcaption></figcaption></figure>

<figure><img src="../.gitbook/assets/PXL_20240802_123617506.jpg" alt=""><figcaption></figcaption></figure>

Cable connecting the battery (female) to the ESP32 (male). GND connects directly, power side goes through toggle switch and reed switch in parallel. \
Reed switch is to be controller via external magnet outside the enclosure, but if for any reason it fails and for debugging additional toggle switch is added that can also turn on the device.

