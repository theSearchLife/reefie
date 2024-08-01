---
description: Components preparation
---

# Hardware

## 3D printing

Slicing done with PrusaSlicer [https://www.prusa3d.com/page/prusaslicer\_424/](https://www.prusa3d.com/page/prusaslicer\_424/)

### Internal components

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 09-39-46.png" alt=""><figcaption><p>Orientation</p></figcaption></figure>

1. ConnectingRingTop
2. ConnectingRingBottom
3. VD\_holder
4. BatteryHolder\_v004
5. ReedSwitchHolder

* Material - PETG
* Default settings for
  * 0.2mm speed
  * Generic PETG
* Infill - 100%

<div>

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 09-41-25.png" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 09-41-12.png" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 09-41-06.png" alt=""><figcaption></figcaption></figure>

</div>

1. SensorStackAttachment
2. Display\_holder\_1.3

* Material - PETG
* Default settings for
  * 0.2mm speed
  * Generic PETG
* Infill - 100%
* Add smart fill support as shown in images

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 10-09-46.png" alt=""><figcaption></figcaption></figure>

{% hint style="danger" %}
Cable clamp inner diameter needs to be adjusted to each individual cable size before exporting STL.
{% endhint %}

* Filename: 5x **o-ring-protection** and 10x **CableGland\_cableclamp**
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4

### External components

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-11 14-30-09.png" alt=""><figcaption><p>Orientation Handle_curved</p></figcaption></figure>

* **Handle\_curved**
  * Filament - PETG
  * Default settings for
    * 0.2mm speed
    * Generic PETG
  * Infill - 100%
  * Add Brim 7mm

<figure><img src="../.gitbook/assets/Screenshot from 2024-07-11 14-32-55.png" alt=""><figcaption></figcaption></figure>

* 2x **Clamp\_top** and 2x **Clamp\_bottom**
  * Material: PETG
  * Default settings for
    * 0.2mm speed
  * Infill - 100%

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 10-03-59.png" alt=""><figcaption></figcaption></figure>

* Filename: knob
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4
* Brim (optional)

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 10-06-40.png" alt=""><figcaption></figcaption></figure>

* Filename: SensorCover
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 10-07-12.png" alt=""><figcaption></figcaption></figure>

* Filename: SensorMountingPlate
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4

### Sealing Flanges

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 09-45-55.png" alt=""><figcaption></figcaption></figure>

{% hint style="danger" %}
Before exporting STL,  measure inside tube diameter and adjust spreadsheet value for flange\_from\_tube to be around 0.6mm smaller than inner diameter. Default inner diameter value is 94mm. flange\_from\_tube value is to compensate from actual tolerances. \
Example, if real inner diamter is 95mm, make flange\_from\_tube to 0.2mm, if real inner diameter is 93mm, make flange from tube -0.8mm.&#x20;
{% endhint %}

* Material: PC blend (natural color)
* Default settings for 0.2 Layer height
  * Perimeter - 6
  * Infill - 100%
  * Brim
    * Width - 20mm
    * Type - Outer and inner brim

### Switch

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 14-07-31.png" alt=""><figcaption></figcaption></figure>

* Filename: **SwitchSpring** and **SwitchHolder**, **SwitchHolder Mirror**
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4

<div>

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 17-00-10.png" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 17-00-07.png" alt=""><figcaption></figcaption></figure>

</div>

* Filename: **SwitchLid\_withText**
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4
* Dual color
  * Black external
  * White text and arrows

<figure><img src="../.gitbook/assets/Screenshot from 2024-08-01 17-02-51.png" alt=""><figcaption></figcaption></figure>

* Filename: **Switchslider\_withText**
* Material: PETG
* Default settings for 0.2
* Infill - 100%
* Perimeter - 4
* Dual color
  * White main color
  * Black text



##

## Post-processing

### Cleaning up 3D prints

* Debur any parts with brim
* Using a sharp chisel remove any parts required support
* Using a torch lighter remove any stringing if present

### Heat inserts

Parts that require heat inserts

All heatinserts are for M3, insert height is 5.7mm.

* 2xFlangeSeal\_Open
  * 6x top
  * 6x bottom
  * ![](../.gitbook/assets/PXL\_20240801\_112006341.jpg)![](../.gitbook/assets/PXL\_20240801\_112003674.jpg)
* 2xClamp bottom
  * 4x sides, insert from the bottom
  * 2x on 45degree angle, insert from outside
  * ![](../.gitbook/assets/PXL\_20240801\_114310070.jpg)![](../.gitbook/assets/PXL\_20240801\_114324629.jpg)
* 2xClamp top
  * 3x top
  * 2x on 45degree angle, insert from outside
  * ![](../.gitbook/assets/PXL\_20240801\_115316935.jpg)
* Sensor mounting plate
  * 4x sides
  * ![](../.gitbook/assets/PXL\_20240801\_120057983.jpg)
* SwitchHolder and SwitchHolder\_mirror
  * 2x top
  * ![](../.gitbook/assets/signal-2024-08-01-183218\_002.jpeg)



### Coating



## Lids cutting

### Main shape

1.  3D print templates

    1. EndCap\_holePunch
    2. EndCap\_sensorHoles

    ![](<../.gitbook/assets/Screenshot from 2024-08-01 17-09-33.png>)
2.  Punch 6 mounting holes

    ![](../.gitbook/assets/PXL\_20240715\_152238180.jpg)![](../.gitbook/assets/PXL\_20240715\_152443422.jpg)
3.  Drill mounting holes

    ![](../.gitbook/assets/PXL\_20240715\_153038017.jpg)![](../.gitbook/assets/PXL\_20240715\_153213740.jpg)
4.  Add template to mark outline

    ![](../.gitbook/assets/PXL\_20240715\_153657840.jpg)![](../.gitbook/assets/PXL\_20240715\_153751269.jpg)![](../.gitbook/assets/PXL\_20240715\_154101431.jpg)
5. Cut outline
6. Debur mounting holes
7. Sand and polish edges

<figure><img src="../.gitbook/assets/PXL_20240716_092432860.jpg" alt=""><figcaption></figcaption></figure>

### Sensors and Pressure relief valve modifications

Use the same template for each version

<div>

<figure><img src="../.gitbook/assets/signal-2024-08-01-171500_003.jpeg" alt=""><figcaption></figcaption></figure>

 

<figure><img src="../.gitbook/assets/signal-2024-08-01-171500_002.jpeg" alt=""><figcaption></figcaption></figure>

</div>

* Sensor side&#x20;
  * Punch 5 holes around the center
  * Drill with 16 mm step drill
  * Debur edges
* Pressure relief side
  * Punch 1 center hole
  * Drill with 10mm drill
  * Debur edges

## Tube cutting

1. (Optional) 3D print clamps for guiding the hacksaw
   1. File names-
      1. TubeCuttingClampRoundA
      2. TubeCuttingClampRoundB
      3. TubCuttingHolder
2. Measure 20cm length
3. Add guide clamps
4. Cut
5. Debur inside edges

<table data-card-size="large" data-view="cards"><thead><tr><th></th><th></th><th></th><th data-hidden data-card-cover data-type="files"></th></tr></thead><tbody><tr><td></td><td><img src="../.gitbook/assets/signal-2024-07-11-144226_002.jpeg" alt=""></td><td></td><td></td></tr><tr><td></td><td><img src="../.gitbook/assets/signal-2024-07-11-144226_003.jpeg" alt=""></td><td></td><td></td></tr><tr><td></td><td><img src="../.gitbook/assets/signal-2024-07-11-144226_004.jpeg" alt=""></td><td></td><td></td></tr><tr><td></td><td></td><td><img src="../.gitbook/assets/signal-2024-07-11-144226_005.jpeg" alt=""></td><td></td></tr><tr><td></td><td></td><td></td><td></td></tr></tbody></table>

