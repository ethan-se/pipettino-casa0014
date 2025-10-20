// Ethan Taylor Oct 2025 - test script for sending 
// TCS3200 sensor readings to Vespera via MQTT

//outputs values to serial monitor, and to an MQTT broker connected to vespera

// Code for networking via MQTT used from:
// Duncan Wilson Oct 2025 - v1 - MQTT messager to vespera
// available at: https://github.com/ucl-casa-ce/casa0014/tree/main/vespera/workshop-sketch/mkr1010_mqtt_simple

// works with MKR1010

// --- import libraries ---

#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h" 
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010

// --- define connectivity parameters ---
const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

// create wifi object and mqtt object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Make sure to update your lightid value below with the one you have been allocated
String lightId = "24"; // the topic id number or user number being used.

// Here we define the MQTT topic we will be publishing data to
String mqtt_topic = "student/CASA0014/luminaire/" + lightId;            
String clientId = ""; // will set once i have mac address so that it is unique

// NeoPixel Configuration - we need to know this to know how to send messages 
// to vespera 
const int num_leds = 72;
const int payload_size = num_leds * 3; // x3 for RGB

// Create the byte array to send in MQTT payload this stores all the colours 
// in memory so that they can be accessed in for example the rainbow function
byte RGBpayload[payload_size];

// --- define pins ---

// This is the pin that outputs the sensor readings. 
// The output is a square wave whose frequency varies 
// based on the colour and light of recorded light.  
int outputFreq = 2; //"OUT" on breakout board

// These pins control the scale of the output frequency of sensor readings.
// Different cominbations of HIGH and LOW voltages will modify the
// output frequency as-per the table in the specifications sheet.
int outputFreqSelect1 = 3; //"S0" on breakout board
int outputFreqSelect2 = 4; //"S1" on breakout board

// These pins control the four photodiodes (red, green, blue and clear light) 
// on the sensor. Different combinations of HIGH and LOW voltages 
// will select particular a photodiode to output a reading, as-per 
// the spec sheet. This is how individual RGB values are obtained.
int lightSelect1 = 5; //"S2" on breakout board
int lightSelect2 = 6; //"S3" on breakout board

// This pin controls the activation of the 4 white onboard LEDs
// on the breakout board
int whiteLEDs = 7;

// --- Initialise variables to store colour readings later ---
int redFreq, greenFreq, blueFreq; //raw light frequency readings to be remapped to RGB format
int r, g, b; //RGB values pushed via MQTT

void setup() {
  Serial.begin(115200);
  //while (!Serial); // Wait for serial port to connect (useful for debugging)
  Serial.println("Vespera");

  // --- Tell Arduino to send data to the control pins ---
  pinMode(outputFreqSelect1, OUTPUT);
  pinMode(outputFreqSelect2, OUTPUT);
  pinMode(lightSelect1, OUTPUT);
  pinMode(lightSelect2, OUTPUT);
  pinMode(whiteLEDs, OUTPUT);

  // --- Tell Arduino to recieve data from the sensor's 
  // output frequency pin ---
  pinMode(outputFreq, INPUT);

  // --- Activate the white LEDs and leave them on ---
  digitalWrite(whiteLEDs, HIGH); //off for now

  // --- Set the scale of the output frequency from sensor ---
  digitalWrite(outputFreqSelect1, HIGH); 
  digitalWrite(outputFreqSelect2, LOW);
  // ^ Frequency scaling configuration:
  // (1) LOW + (2) LOW = power down
  // (1) LOW + (2) HIGH = 2% scaling
  // (1) HIGH + (2) LOW = 20% scaling
  // (1) HIGH + (2) HIGH = 100% scaling
  // Higher scaling = higher frequency = higher range of values.


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
  
  Serial.println("Set-up complete");
}
 
void loop() {
  // Reconnect if necessary
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  }
  // keep mqtt alive
  mqttClient.loop();

  // *** READING COLOUR VALUES FROM SENSOR ***

  // --- Configure photodiodes to read RED light ---
  digitalWrite(lightSelect1, LOW);
  digitalWrite(lightSelect2, LOW);

  // --- write reading for red light to "red" variable ---
  redFreq = pulseIn(outputFreq, LOW);
  // --- remap frequency of red light to RGB value readable by Vespera ---
  r = map(redFreq, 30, 120, 255, 0); // values 30/120 as max/min colour intensity. Calibrated in CE lab using lego pieces

  // --- Configure photodiodes to read GREEN light ---
  digitalWrite(lightSelect1, HIGH);
  digitalWrite(lightSelect2, HIGH);

  // --- write reading for green light to "green" variable ---
  greenFreq = pulseIn(outputFreq, LOW);
  g = map(greenFreq, 80, 270, 255, 0); // values 80/270 as max/min colour intensity. Calibrated in CE lab using lego pieces

  // --- Configure photodiodes to read BLUE light ---
  digitalWrite(lightSelect1, LOW);
  digitalWrite(lightSelect2, HIGH);

  // --- write reading for blue light to "blue" variable ---
  blueFreq = pulseIn(outputFreq, LOW);
  b = map(blueFreq, 25, 300, 255, 0); // values 25/300 as max/min colour intensity. Calibrated in CE lab using lego pieces

  // --- Display readings on serial monitor ---
  Serial.print("Red : ");
  Serial.print(r);
  Serial.print(", Green : ");
  Serial.print(g);
  Serial.print(", Blue : ");
  Serial.print(b);
  Serial.println();

  for(int n=0; n<num_leds; n++){
    send_RGB_to_pixel(r,g,b,n);
    delay(50);
  }
  delay(2000);
  send_all_off();
}

// Function to update the R, G, B values of a single LED pixel
// RGB can a value between 0-254, pixel is 0-71 for a 72 neopixel strip
void send_RGB_to_pixel(int r, int g, int b, int pixel) {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Update the byte array with the specified RGB color pattern
    RGBpayload[pixel * 3 + 0] = (byte)r; // Red
    RGBpayload[pixel * 3 + 1] = (byte)g; // Green
    RGBpayload[pixel * 3 + 2] = (byte)b; // Blue

    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    // Serial.println("Published whole byte array after updating a single pixel.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_RGB_to_pixel*.");
  }
}

void send_all_off() {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Fill the byte array with the specified RGB color pattern
    for(int pixel=0; pixel < num_leds; pixel++){
      RGBpayload[pixel * 3 + 0] = (byte)0; // Red
      RGBpayload[pixel * 3 + 1] = (byte)0; // Green
      RGBpayload[pixel * 3 + 2] = (byte)0; // Blue
    }
    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    Serial.println("Published an all zero (off) byte array.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_all_off*.");
  }
}

void send_all_random() {
  // Check if the mqttClient is connected before publishing
  if (mqttClient.connected()) {
    // Fill the byte array with the specified RGB color pattern
    for(int pixel=0; pixel < num_leds; pixel++){
      RGBpayload[pixel * 3 + 0] = (byte)random(50,256); // Red - 256 is exclusive, so it goes up to 255
      RGBpayload[pixel * 3 + 1] = (byte)random(50,256); // Green
      RGBpayload[pixel * 3 + 2] = (byte)random(50,256); // Blue
    }
    // Publish the byte array
    mqttClient.publish(mqtt_topic.c_str(), RGBpayload, payload_size);
    
    Serial.println("Published an all random byte array.");
  } else {
    Serial.println("MQTT mqttClient not connected, cannot publish from *send_all_random*.");
  }
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}



