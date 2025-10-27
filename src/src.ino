// Ethan Taylor (ethan-se) Oct 2025 - Pipettino source code

// On button press, sends TCS3200 sensor readings to Vespera via MQTT

// Code extended and reused from workshop-sketch/mkr1010_mqtt_simple.ino by Duncan Wilson (djdunc), available at: https://github.com/ucl-casa-ce/casa0014/tree/main/vespera/workshop-sketch/mkr1010_mqtt_simple

// Comments outline what code is:
// Original ("START/END: Original code"),
// Extended ("START/END: Extended code from workshop sketch"), where added code is declared with the commented prefix "NEW:",
// Reused ("START/END: unmodified code from workshop sketch").

// For Arduino MKR1010

// --- PROGRAM BEGIN ---

// START: unmodified code from workshop sketch:
// Import libraries
#include <SPI.h>
#include <WiFiNINA.h>           // library to enable WiFi connectivity on MKR1010
#include <PubSubClient.h>       // library to enable publishing and subscribing to the MQTT server mqtt.cetools.org
#include "arduino_secrets.h"    // header containing WiFi and MQTT credentials. Make your own and place in the same directory as this .ino file
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010

// Define connectivity parameters
const char* ssid          = SECRET_SSID;      //you must define this in arduino_secrets.h
const char* password      = SECRET_PASS;      //you must define this in arduino_secrets.h
const char* mqtt_username = SECRET_MQTTUSER;  //you must define this in arduino_secrets.h
const char* mqtt_password = SECRET_MQTTPASS;  //you must define this in arduino_secrets.h
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

// create wifi object and mqtt object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Make sure to update your lightid value below with the one you have been allocated
String lightId = "24"; // String selects topic assigned to Ethan

// Here we define the MQTT topic we will be publishing data to
String mqtt_topic = "student/CASA0014/luminaire/" + lightId;   //concatenate string to get path to publish topic         
String clientId = ""; // initialise empty string to be filled upon obtaining mac address

// NeoPixel Configuration - we need to know this to know how to send messages 
// to vespera 
const int num_leds = 72; // this is the total number of lights on vespera
const int payload_size = num_leds * 3; // 72x3 for RGB - this is the total number of LEDs on Vespera

// Create the byte array to send in MQTT payload this stores all the colours 
// in memory so that they can be accessed in for example the rainbow function
byte RGBpayload[payload_size];
// END: unmodified code from workshop sketch:

// START: original code:
// Define pins
int button = 6;            // This pin is connected to the tactile button
bool buttonState;          // Initialise variable to record current status of tactile button
bool lastButtonState;      // Initialise variable for saving what state the tactile button was in during last loop execution
 
// This pin recieves readings from the TCS3200 colour sensor to Arduino:
int outputFreq = 4;        // Connected to "OUT" on SEN0101 breakout board. 

// These pins control the range of values output by the TCS3200 colour sensor:
int outputFreqSelect1 = 0; // Connected to "S0" on SEN0101 breakout board
int outputFreqSelect2 = 1; // Connected to "S1" on SEN0101 breakout board

// These pins control which lightwaves (red, green or blue) are read by the colour sensor at any given time:
int lightSelect1 = 2;      // Connected to "S2" on SEN0101 breakout board
int lightSelect2 = 3;      // Connected to "S3" on SEN0101 breakout board

// This pin controls the activation of the 4 white onboard LEDs:
int whiteLEDs = 5;         // Connected to "LED" on SEN0101 breakout board. Some breakout boards don't have this header and have the white LEDs always on

// This pin controls the activation of an external yellow LED wired to Arduino:
int busyLED = 7; // yellow LED

// Initialise variables to store colour readings later
int redFreq, greenFreq, blueFreq; // Stores raw light frequency readings from colour sensor
int redRemap, blueRemap, greenRemap; // Stores light frequency readings in the process of being converted to RGB format 
int r, g, b; // Stores light frequency readings converted to RGB format, which are read by Vespera

// Set parameters to calibrate colour sensor readings
int rHigh = 410;
int rLow = 85;
int gHigh = 462;
int gLow = 130;
int bHigh = 385;
int bLow = 100;
// ^ Calibrated in CE Lab at 6pm, 27th October 2025 using brightly coloured felt

// END: original code

// START: unmodified code from workshop sketch:
void setup() {
  Serial.begin(115200);
  //while (!Serial); // Wait for serial port to connect (useful for debugging)
  Serial.println("Vespera");
  // END: unmodified code from workshop sketch

  // START: original code
  // Send data to the control pins
  pinMode(outputFreqSelect1, OUTPUT);
  pinMode(outputFreqSelect2, OUTPUT);
  pinMode(lightSelect1, OUTPUT);
  pinMode(lightSelect2, OUTPUT);
  pinMode(whiteLEDs, OUTPUT);
  pinMode(busyLED, OUTPUT);

  // Recieve data from the colour sensor's output pin
  pinMode(outputFreq, INPUT);

  // Listen for button presses
  pinMode(button, INPUT);

  // Activate the white LEDs
  digitalWrite(whiteLEDs, HIGH); //off for now

  // Indicate that device is busy during startup
  digitalWrite(busyLED, HIGH); 

  // Configure the range of readings sent from the colour sensor's output pin
  digitalWrite(outputFreqSelect1, HIGH); // (1)
  digitalWrite(outputFreqSelect2, LOW);  // (2)
  // ^ Frequency scaling configuration:
  // (1) LOW + (2) LOW = power down
  // (1) LOW + (2) HIGH = 2% scaling
  // (1) HIGH + (2) LOW = 20% scaling
  // (1) HIGH + (2) HIGH = 100% scaling
  // Higher scaling = lower range of values.

  // END: original code

  // START: Extended code from workshop sketch:
  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

  Serial.print("This device is Vespera ");
  Serial.println(lightId);

  // Connect to WiFi
  startWifi();

  // Connect to MQTT broker
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(2000);
  mqttClient.setCallback(callback);
  
  digitalWrite(busyLED, LOW); // NEW: Indicate that device ready to recieve user input after MQTT connection is established
  Serial.println("Set-up complete");
}
 
void loop() {
  // Reconnect if necessary
  if (!mqttClient.connected()) {
    digitalWrite(busyLED, HIGH); // NEW: Indicate that device is busy during reconnection attempts
    reconnectMQTT();
    digitalWrite(busyLED, LOW);  // NEW: Indicate that device is ready for user input
  }
  
  if (WiFi.status() != WL_CONNECTED){
    digitalWrite(busyLED, HIGH); // NEW: Indicate that device is busy during reconnection attempts
    startWifi();
    digitalWrite(busyLED, LOW);  // NEW: Indicate that device is ready for user input
  }
  // keep mqtt alive
  mqttClient.loop();
  // END: modified code from workshop sketch

  // START: Original code:
  buttonState = digitalRead(button);      //check button state
  if (buttonState != lastButtonState) {   //detect if button has been pressed
    // if button has been pressed...

    // Alert user that device is busy
    digitalWrite(busyLED, HIGH); 

    // Configure colour sensor to detect the frequency of RED lightwaves
    digitalWrite(lightSelect1, LOW);
    digitalWrite(lightSelect2, LOW);
    // Store reading for light frequency
    redFreq = pulseIn(outputFreq, LOW);
    // Remap frequency reading to an RGB value
    redRemap = map(redFreq, rLow, rHigh, 255, 0); // TO CALIBRATE
    // Constrain RGB value in case of integer overflow
    r = constrain(redRemap, 0, 255); // Note: because of the way "constrain()" is implemented, I have not used this function within the above line of code. See Arduino documentation: https://docs.arduino.cc/language-reference/en/functions/math/constrain/
    
    // Configure colour sensor to detect the frequency of GREEN lightwaves
    digitalWrite(lightSelect1, HIGH);
    digitalWrite(lightSelect2, HIGH);
    // Store reading for light frequency
    greenFreq = pulseIn(outputFreq, LOW);
    // Remap frequency reading to an RGB value
    greenRemap = map(greenFreq, gLow, gHigh, 255, 0); // TO CALIBRATE
    // Constrain RGB value in case of integer overflow
    g = constrain(greenRemap, 0, 255); // Note: because of the way "constrain()" is implemented, I have not used this function within the above line of code. See Arduino documentation: https://docs.arduino.cc/language-reference/en/functions/math/constrain/

    // Configure colour sensor to detect the frequency of BLUE lightwaves
    digitalWrite(lightSelect1, LOW);
    digitalWrite(lightSelect2, HIGH);
    // Store reading for light frequency
    blueFreq = pulseIn(outputFreq, LOW);
    // Remap frequency reading to an RGB value
    blueRemap = map(blueFreq, bLow, bHigh, 255, 0); // TO CALIBRATE
    // Constrain RGB value in case of integer overflow
    b = constrain(blueRemap, 0, 255); // Note: because of the way "constrain()" is implemented, I have not used this function within the above line of code. See Arduino documentation: https://docs.arduino.cc/language-reference/en/functions/math/constrain/
    
    // Display readings on serial monitor
    Serial.println();
    Serial.print("R : ");
    Serial.print(r);
    Serial.print(", G : ");
    Serial.print(g);
    Serial.print(", B : ");
    Serial.print(b);
    Serial.println();
    Serial.print("Rm : ");
    Serial.print(redRemap);
    Serial.print(", Gm : ");
    Serial.print(greenRemap);
    Serial.print(", Bm : ");
    Serial.print(blueRemap);
    Serial.println();
    Serial.print("Rf : ");
    Serial.print(redFreq);
    Serial.print(", Gf : ");
    Serial.print(greenFreq);
    Serial.print(", Bf : ");
    Serial.print(blueFreq);
    
    // Match Vespera lights to converted sensor readings, one-by-one, in numerical order
    for(int n=0; n<num_leds; n++){
      send_RGB_to_pixel(r,g,b,n);
      delay(50);
      }
    // Indicate that device is ready for user input
    digitalWrite(busyLED, LOW);
    }
    else {
      //do nothing, restart loop until next button press
    }
    //save this loop's button state
    lastButtonState = buttonState;
  }
//END: Original code

// --- PROGRAM END ---







