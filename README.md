# Pipettino
Pipettino is a portmanteau of the nouns "Pipette" and "Arduino". Pipettes, or eyedroppers, are a staple icon of colour pickers in any modern graphics software.
By combining this iconography with the computing power of an Arduino, one may bring forth this previously digital-only utility into the physical realm.

## What's this all about?
Pipettino is an Arduino-powered interface to control an electric light unit, [Vespera](https://github.com/ucl-casa-ce/casa0014/tree/main/vespera), by sending MQTT messages over Wi-Fi.

Pipettino works using an active sensor that detects the frequency of light waves being reflected from a target object into its tip. 

These readings are converted to a colour value that Vespera can display. At the press of a button, this colour value is published as a message to an MQTT server at the following topic: `mqtt.cetools.org/student/CASA0014/luminaire/24`. 

Vespera can subscribe to this topic. It reads messages sent by Pipettino to match its lights to the colour of the targeted object.

Alternatively, one can view Vespera's lights remotely using [this web viewer](https://www.iot.io/projects/lumi/).

<!-- Upload photos inside /docs/ directory! -->

<!-- <p align="center">
    <img src="/docs/pip-beta.jpg" alt="Near-complete build of 'Pipettino' interface for the Vespera light display" width="500"/>
</p> -->

## Contents
<!-- rename to match final section names -->
- [Prerequisites](#prerequisites)
- [Assembly](#assembly)
- [Architecture](#architecture)
- [Areas for Improvement](#areas-for-improvement)
- [Further Reading](#further-reading)

## What do I need to build it?
Here are the following software and hardware components required to build this project:
### Software
- Arduino IDE 2.3.6, with the following libraries installed:
```
SPI@1.0
WiFiNINA@1.9.1
PubSubClient@2.8
```
- An `arduino_secrets.h` file with the following credentials to hand:
    - Your WiFi SSID 
    - Your WiFi password
    - An MQTT username for `mqtt.cetools.org`
    - Your MQTT password 
### Hardware
<!-- Review after build complete -->
#### Electronics
- Arduino MKR 1010 WiFi
<!-- Check what the actual gauge of my wire is -->
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
Put this in the build tutorial:
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

## How can I build this device?
Please follow these instructions:
### Software
Ensure that all the prerequisite software has been installed, then follow these instructions:
1. Download the _folder_ `/src/` and all its contents in this repository to your local machine.
2. Open Arduino IDE on your local machine and navigate to where you downloaded the `/src/` folder.
3. Open `src.ino` in Arduino IDE.
4. Connect your Arduino MKR1010 WiFi to your local machine.
5. Select the correct port of your Arduino MKR1010 WiFi that you just plugged in, using the drop-down menu at the top of the program window.
6. Click the "Upload" icon (arrow pointing right) at the top left of the program window.

At this stage, your installation is a success if:
- Arduino IDE complied and uploaded the code with no errors.
- The onboard RGB LED lights up blue immediately.
- If you are in range of your WiFi network, the onboard RGB LED eventually turns green after several seconds.

### Hardware
#### Before you begin:
1. If you are using a 400-pin breadboard with detachable power buses, use a pair of scissors to cut off the adhesive label that joins the power buses to the centre part of the breadboard.
2. Duct tape one of the power buses on top of either side of the bridge of the centre breadboard. We only need one side available for this project.
<!-- Image goes here -->
If the short side of your breadboard is thinner than PLACEHOLDER cm, please skip the above steps.
3. Prepare wires to connect specific components:
    - PLACEHOLDER total male-to-male headered wires 
        - ...for these Arduino pins:
            - VCC
            - GND
   - 8 total male-to-female headered wires:
        -...for these Arduino pins:
            - 0
            - 1
            - 2
            - 3
            - 4
            - 5
            - 7
        - ...for one wire connecting the cathode of the yellow LED to the breadboard
    - 2 total wires that are male at one end, and stripped bare on other end: 
        - ...for two wires that connect the tactile button to the breadboard. The bare ends will be soldered on in later steps.

#### Assembly
1. Wire up your Arduino MKR1010, TCS3200 Color Sensor, LED and button to your breadboard as-per the following diagram:
<!-- Image goes here -->
Try to place the Arduino MKR1010, TCS3200 Color Sensor and breadboard in the same relative positions as in the diagram, along a horizonal axis, before you wire them up. This will make the later steps much easier.
2. Verify that [the code you uploaded to your Arduino MKR1010 WiFi earlier](#software-2) works correctly by carrying out the following checks:
    - Plug your Arduino into mains power using the micro USB cable. The four white LEDs on the TCS3200 Color Sensor should be lit up.
    - Press the tactile button once the Arduino is plugged in and the yellow LED is off. The yellow LED should light up and turn off after a couple of seconds.
3. Unplug your micro USB cable from mains power, but keep the end of the cable connected to your Arduino plugged in.
4. Make a single, straight vertical cut down the height of your cardboard roll for *kitchen paper* so that it can be laid flat on a table.
<!-- image goes here -->
5. Fix your Arduino and your breadboard to the inner surface of the cardboard roll using duct tape, as in the following picture:
<!-- image goes here -->
Ensure that the yellow LED, button and micro USB cable are positioned to the side closest to the breadboard. Also ensure that the TCS3200 Color Sensor is positioned to the side closest to your Arduino.
6. Make a single, straight vertical cut down the height of your cardboard roll for *toilet paper*
7. Roll the kitchen paper cardboard back up, placing the toilet paper cardboard over its centre and fixing both rolls together with duct tape.
<!-- image goes here -->
8. Take your plastic water bottle, remove the lid, and use a pair of scissors to cut your plastic water bottle close to its neck like so:
<!-- image goes here -->
9. Apply a small amount of duct tape to both sides of your TCS3200 Color Sensor, as shown below:
<!-- image goes here -->
10. Fix the TCS3200 Color Sensor to the inside of the bottleneck, so that it covered the hole squarely.
<!-- image goes here -->

<!-- Next steps: cardboard circles for pipette, making the button area, leaving space for LED, fixing LED and button to structure, taping it all together, (optional) covering the rest with black masking tape -->

## How does it work?
<!-- Not started -->
Here is how the different software and hardware components interact with each other.
### Software
A diagram on draw.io

how everything in the src folder works together.
### Hardware
A diagram on draw.io

Sensor > MCU > Gateway > Cloud > Node (Vespera)

## Areas of Improvement
<!-- Elaborate -->
- Increase colour detection accuracy
- Improve user experience with "flow" of data to Vespera

## Further Reading
<!-- Review at later date -->
- https://wiki.dfrobot.com/TCS3200_Color_Sensor__SKU_SEN0101_
- https://randomnerdtutorials.com/arduino-color-sensor-tcs230-tcs3200/