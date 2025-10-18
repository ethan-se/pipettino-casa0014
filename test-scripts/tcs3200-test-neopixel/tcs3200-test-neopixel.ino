// Ethan Taylor Oct 2025 - TCS32000 (SKU SEN0101) output test script. 
// Outputs sensor data to serial monitor AND neopixel RGB LED.

// works with MKR1010

  // --- Import libraries ---
  #include <Adafruit_NeoPixel.h>

  // --- Define pins ---

  #define NEOPIXEL_PIN 1 //pin connecting to "DATA IN" header on neopixel
  #define NUMPIXELS 1

  // This is the pin that outputs the sensor readings. 
  // The output is a square wave whose frequency varies 
  // based on the colour and intensity of recorded light.  
  int outputFreq = 2; //"OUT" on breakout board

  // These pins control the output frequency of sensor readings:
  // Configuration involves setting a combination of HIGH and LOW
  // volatages to these pins. Refer to specifications.
  int outputFreqSelect1 = 3; //"S0" on breakout board
  int outputFreqSelect2 = 4; //"S1" on breakout board

  // These pins control the four photodiodes (red, green, blue and clear light) 
  // on the sensor. Different combinations of HIGH and LOW voltages 
  // will select particular a photodiode to output a reading, as-per 
  // the spec sheet. This is how individual RGB values are obtained.
  int lightSelect1 = 5; //"S2" on breakout board
  int lightSelect2 = 6; //"S3" on breakout board

  // This pin controls the activation of the 4 white onboard LEDs
  // on the breakout board.
  int whiteLEDs = 7; //"LED" on breakout board

  // --- Initialise NeoPixel object ---
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  // ^ This tells Arduino about:
  // 1. How many NeoPixel lights are there on the breakout board?
  // 2. Which pin is connected to the NeoPixel?
  // 3. What RGB bitstream is used for this NeoPixel?
  // 4. What bitstream frequency do we need for this NeoPixel?
  int pixelBrightness = 50; //controls brightness of NeoPixel lights

  // --- Initialise variables to store colour readings later ---
  int red, green, blue;

void setup() {
  // --- Establish serial commication to monitor readings ---
  Serial.begin(9600); 

  // --- Initialise NeoPixel behaviour ---
  pixels.begin(); //turn on neoPixel
  pixels.show();  //send data immediately to NeoPixel. Since there is no data right after initialisation, this funciton call turns all lights off
  pixels.setBrightness(pixelBrightness); //brightness setting, using an 8-bit integer

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
  digitalWrite(whiteLEDs, HIGH);
}

void loop() {
  // --- Set the scale of the output frequency from sensor ---
  digitalWrite(outputFreqSelect1, HIGH); 
  digitalWrite(outputFreqSelect2, HIGH);
  // ^ Frequency scaling configuration:
  // LOW + LOW = power down
  // LOW + HIGH = 2% scaling
  // HIGH + LOW = 20% scaling
  // HIGH + HIGH = 100% scaling
  // Higher scaling = higher frequency = higher range of values.

  // --- Configure photodiodes to read RED light ---
  digitalWrite(lightSelect1, LOW);
  digitalWrite(lightSelect2, LOW);

  // --- write reading for red light to "red" variable ---
  red = pulseIn(outputFreq, LOW); //

  // --- Configure photodiodes to read GREEN light ---
  digitalWrite(lightSelect1, HIGH);
  digitalWrite(lightSelect2, HIGH);

  // --- write reading for green light to "green" variable ---
  green = pulseIn(outputFreq, LOW);

  // --- Configure photodiodes to read BLUE light ---
  digitalWrite(lightSelect1, LOW);
  digitalWrite(lightSelect2, HIGH);

  // --- write reading for blue light to "blue" variable ---
  blue = pulseIn(outputFreq, LOW);

  // --- Display readings on serial monitor ---
  Serial.print("Red : ");
  Serial.print(red);
  Serial.print(", Green : ");
  Serial.print(green);
  Serial.print(", Blue : ");
  Serial.print(blue);
  Serial.println();

  // --- Display readings on NeoPixel ---
    for (int i=0; i<NUMPIXELS; i++) { // this loop ensures all lights are used simultaneously to display colour readings, regadless of the amount of pixels one has on their NeoPixel strip
    pixels.setPixelColor(i, red, green, blue);
  }
  pixels.show(); // send colour data to NeoPixel

  delay (1000); //pause for 1 second between readings
}