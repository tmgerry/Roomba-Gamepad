# RoboVacum-Gamepad
Simple library for communicating with "iRobot Open Interface", using microcontrollers compatible with Arduino. Includes example using ESP32 to directly control Roomba e6.

This was inspired by watching my basic roomba, with only random walk navigation, *just* miss crumbs on the floor. I chose to take a different, more satisfying approach than others have, by enabling gamepad control to give you the power to directly vacuum any debris at your wim.

In this repository there is the sourcecode for the library, as well as an example using it.

## Example Setup

**Notes-**
+ The example configuration utilizes "Bluepad32" to enable gamepad connections on the ESP32. [See License](https://github.com/ricardoquesada/bluepad32?tab=License-1-ov-file)
+ The button mapaping is designed around an Xbox One Wireless Controller Model 1708.

### Hardware
+ Roomba e6 (should work with other Roombas, but only tested with this one)
+ [Adafruit ESP32 QT PY Pico](https://www.adafruit.com/product/5395)
+ [SparkFun Buck Regulator Breakout - 3.3V (AP63203)](https://www.sparkfun.com/products/18356)
+ [7 Pin Mini-DIN Connector](https://www.adafruit.com/product/2438) If you want to do this the right way, use this.
    + If you want to just get it up and running, use 22 awg Solid Core Wire (Size is important to "jerry-rig" the connector) 
+ [Push Button Switch](https://www.digikey.com/en/products/detail/zf-electronics/KFB2ANA1BBB/2027294) (SPST, On-Off) A switch like this, rated for the voltage and current. You will need to drill a hole to install the button on the top side of the roomba.
+ [Terminal Blocks (2.54 mm Spacing)](https://www.adafruit.com/product/2141) I prefer to use these to connect wires to microcontrollers in this type of scenario, but if you have a method you prefer instead that will work fine as well.
+ Stranded wire (20-24 awg wire is perfect for this application)
+ [WAGO 221 Lever Nuts](https://www.digikey.com/en/products/detail/wago-corporation/221-412-VE00-2500/13175703) (Only necessary If you want to be able to quickly remove the microcontroller.) 
    + Note: Using these negatively impacts signal integrity on the TX/RX wires, but at these speeds (9600 bits/s) it doesn't affect performance. This is a vacuum controlled with an xbox controller, not a spaceship.



### Wiring
Wiring is straightforward, requiring only 4 connections.

On the roomba side; Power, Ground, TX and RX are needed.

Connect power (+) from the roomba to one side of the the switch, and then from the other side of the switch to the buck converter. Then connect ground straight to the buck converter. (On the Roomba e6 this is ~14.4 Volts nominal, so if using a different buck converter ensure it is rated for it.)

Connect the TX/RX Pins to the microcontrollers RX/TX pins. Make sure it is wired TX -> RX and RX -> TX, otherwise it will not work.

### Firmware
Load the arduino project located in the "example" folder into arduino IDE, Ensure "BluePad32" is properly setup. [(Link)](https://github.com/ricardoquesada/bluepad32)

### User Instructions

1. Turn on microcontroller using the push button
1. Turn on controller
1. Wait for the controller status light to show connected
1. Vacuum ***all*** the crumbs
