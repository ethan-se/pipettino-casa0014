This section is incomplete!
# How to build Pipettino
## Requirements
### Software
Please install/prepare the following software components:
- Arduino IDE 2.3.6, with the following libraries installed:
```
SPI@1.0
WiFiNINA@1.9.1
PubSubClient@2.8
```
- An `arduino_secrets.h` file with the following credentials to hand:
    - Your Wi-Fi SSID
    - Your Wi-Fi password
    - An MQTT username for `mqtt.cetools.org`
    - Your MQTT password 
### Hardware
<!-- Review after build complete -->
#### Electronics
- Arduino MKR 1010 WiFi <!-- Check what the actual gauge of my wire is -->
- 24AWG solid core wire of various colours
- Several 1-pin wire headers, male and female
- A 400-point breadboard with detachable power bus (or any oblong-shaped breadboard thinner than x cm)
- a TCS3200 Color Sensor, shipped with 4 white LEDs on a breakout board. I used one from DFRobot ([SEN0101](https://wiki.dfrobot.com/TCS3200_Color_Sensor__SKU_SEN0101_))
- One yellow LED
- One tactile button
- One 220 Ohm resistor
- One 10k Ohm resistor
- Micro USB cable - test beforehand to make sure it is capable of data transfer to Arduino
#### Housing materials
- One cardboard rolls from kitchen paper
- Two cardboard rolls from toilet paper, plus a couple spare
- 500ml plastic water bottle
- Duct tape
- Some form of cable management, like cable ties or tape
- Corrugated cardboard
#### Tools
- A pair of scissors
- A pair of compasses
- A pencil
- A ruler
- Wire cutter
- Wire stripper
- Wire crimping tool
- Soldering iron
- Flux

## Software installation
Ensure that all the prerequisite software in the [Requirements](#requirements) section have been installed, then follow these instructions:
1. Download the _folder_ `/src/` and all its contents in this repository to your local machine.
2. Open Arduino IDE on your local machine and navigate to where you downloaded the `/src/` folder.
3. Open `src.ino` in Arduino IDE.
4. Connect your Arduino MKR1010 WiFi to your local machine.
5. Select the correct port of your Arduino MKR1010 WiFi that you just plugged in, using the drop-down menu at the top of the program window.
6. Click the "Upload" icon (arrow pointing right) at the top left of the program window.

At this stage, your installation is a success if:
- Arduino IDE complied and uploaded the code with no errors.
- The onboard RGB LED immediately turns blue.
- If you are in range of your WiFi network, the onboard RGB LED eventually turns green after several seconds.

## Build instructions
<!-- #### Before you begin:
1. If you are using a 400-pin breadboard with detachable power buses, use a pair of scissors to cut off the adhesive label that joins the power buses to the centre part of the breadboard.
2. Duct tape one of the power buses on top of either side of the bridge of the centre breadboard. We only need one side available for this project.

    If the short side of your breadboard is thinner than PLACEHOLDER cm, please skip the above steps.
3. Prepare the following wires
- 3 male-to-male headered wires 
- 8 male-to-female headered wires
- 2 wires that are male at one end, and stripped bare on other end -->


<!-- - 2X MALE TO BARE for two wires that connect the tactile button to the breadboard. The bare ends will be soldered on in later steps. -->

<!-- 7X MALE TO FEMALE for these Arduino pins:
    - 0
    - 1
    - 2
    - 3
    - 4
    - 5
    - 7 -->

<!-- 1x MALE TO FEMALE for one wire connecting the cathode of the yellow LED to the breadboard -->

<!-- 3x male to male for vcc, ground and button (pin 6) pins -->
### Assembling electionic components
1. Wire up your Arduino MKR1010, TCS3200 Color Sensor, LED and button to your breadboard as-per the following diagram:

    (image)

    Try to place the Arduino MKR1010, TCS3200 Color Sensor and breadboard in the same relative positions as in the diagram, along a horizonal axis, before you wire them up. This will make the later steps much easier.
2. Verify that [the code you uploaded to your Arduino MKR1010 WiFi earlier](#software-2) works correctly by carrying out the following checks:
- Plug your Arduino into mains power using the micro USB cable. The four white LEDs on the TCS3200 Color Sensor should be lit up.
- Press the tactile button once the Arduino is plugged in and the yellow LED is off. The yellow LED should light up and turn off after a couple of seconds.
3. Unplug your micro USB cable from mains power, but keep the end of the cable connected to your Arduino plugged in.
### Securing Arduino, breadboard and sensor to enclosure
4. Make a single, straight vertical cut down the height of your cardboard roll for *kitchen paper* so that it can be laid flat on a table.

    (image)
5. Fix your Arduino and your breadboard to the inner surface of the cardboard roll using duct tape, as in the following picture:

    (image)

    Ensure that the yellow LED, button and micro USB cable are positioned to the side closest to the breadboard. Also ensure that the TCS3200 Color Sensor is positioned to the side closest to your Arduino.
6. Make a single, straight vertical cut down the height of your cardboard roll for *toilet paper*
7. Roll the kitchen paper cardboard back up, placing the toilet paper cardboard over its centre and fixing both rolls together with duct tape.

    (image)
8. Take your plastic water bottle, remove the lid, and use a pair of scissors to cut your plastic water bottle close to its neck like so:

    (image)
9. Apply a small amount of duct tape to both sides of your TCS3200 Color Sensor, as shown below:
    
    (image)
10. Fix the TCS3200 Color Sensor to the inside of the bottleneck, so that it covered the hole squarely.
    
    (image)
### Enclosing actuators and power cable

<!-- Next steps: cardboard circles for pipette, making the button area, leaving space for LED, fixing LED and button to structure, taping it all together, (optional) covering the rest with black masking tape -->