// Ethan Taylor Oct 2025 - TCS32000 (SKU SEN0101) output test script 
// for colour sensing.

// works with MKR1010

// --- Define pins ---

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
int red, green, blue;

void setup() {
  // --- Establish serial commication to monitor readings ---
  Serial.begin(9600); 

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

    // --- Set the scale of the output frequency from sensor ---
  digitalWrite(outputFreqSelect1, HIGH); 
  digitalWrite(outputFreqSelect2, LOW);
  // ^ Frequency scaling configuration:
  // (1) LOW + (2) LOW = power down
  // (1) LOW + (2) HIGH = 2% scaling
  // (1) HIGH + (2) LOW = 20% scaling
  // (1) HIGH + (2) HIGH = 100% scaling
  // Higher scaling = higher frequency = higher range of values.
}

void loop() {
  // --- Configure photodiodes to read RED light ---
  digitalWrite(lightSelect1, LOW);
  digitalWrite(lightSelect2, LOW);

  // --- write reading for red light to "red" variable ---
  red = pulseIn(outputFreq, LOW);

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

  delay (1000); //pause for 1 second between readings
}