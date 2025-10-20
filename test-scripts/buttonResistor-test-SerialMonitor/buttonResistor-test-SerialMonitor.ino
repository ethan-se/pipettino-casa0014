// Ethan Taylor Oct 2025
// Test script for button functionality on Arduino
// Used in prototyping Pipettino, a controller for the Vespera light display in CE Labs

// Connect 10k Ohm resistor to ground to prevent short circuiting!

int button = 0;
bool buttonState;

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT);
}

void loop() {
  buttonState = digitalRead(button);
  if (buttonState == HIGH) {
  Serial.println("this button is pressed");
  }
  else{
    Serial.println("this button not pressed");
  }
}
