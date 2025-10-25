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
- [Prerequisites](#prerequisites)
- [Assembly](#assembly)
- [Architecture](#architecture)
- [Areas for Improvement](#areas-for-improvement)
- [Further Reading](#further-reading)

## Prerequisites
Here are the following software and hardware components required to build this project:
### Software
<!-- Get a proper dump from json file using arduinoCLI -->
- Arduino IDE
- Libraries
    - SPI
    - WiFiNINA
    - PubSubClient
    - wifi_drv
- Credentials (as `arduino_secrets.h`)
    - WiFi SSID
    - WiFi password
    - MQTT username for `mqtt.cetools.org`
    - MQTT password 
### Hardware
<!-- Review after build complete -->
- Arduino MKR 1010 WiFi
- Solid core wire of various colours (what gauge?)
- A 400-point breadboard with detachable power bus (or any oblong-shaped breadboard thinner than x cm)
- a TCS3200 Color Sensor, shipped with 4 white LEDs on a breakout board. I used one from DFRobot ([SEN0101](https://wiki.dfrobot.com/TCS3200_Color_Sensor__SKU_SEN0101_))
- One yellow LED
- One tactile button
- Micro USB cable - test beforehand to make sure it is capable of data transfer to Arduino
- Several cardboard rolls from toilet/kitchen paper
- Volvic 500ml water bottle
- Duct tape
- Scissors
- Pair of compasses
- Corrugated cardboard

## Assembly
<!-- Not started -->
### Software
See readme inside src folder.
### Hardware
Build instructions in a separate readme.

## Architecture
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