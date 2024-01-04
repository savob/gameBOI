void LEDOn(byte LED) {
  bitSet(LEDStatus, LED);
}
void LEDOff(byte LED) {
  bitClear(LEDStatus, LED);
}

void disableOutput() {
  digitalWrite(outputEnablePin, HIGH);
}
void enableOutput() {
  digitalWrite(outputEnablePin, LOW);
}

void resetLEDs() {
  // Resets the contents of shift registers
  digitalWrite(resetDataPin, LOW); // Resets data in shift register data
  delayMicroseconds(10);
  lockOutputs(); // Locks in blank values
  digitalWrite(resetDataPin, HIGH); // Allows data into shift register
  
  LEDStatus = 0; // Clears config
}

void updateLEDs() {
  // Shifts out the LED statuses, from LED 31 to 0

  shiftOut(serialDataPin, serialClockPin, MSBFIRST, (LEDStatus >> 24)); // Outputs 24 to 31
  shiftOut(serialDataPin, serialClockPin, MSBFIRST, (LEDStatus >> 16)); // Outputs 16 to 23
  shiftOut(serialDataPin, serialClockPin, MSBFIRST, (LEDStatus >> 8)); // Outputs 8 to 15
  shiftOut(serialDataPin, serialClockPin, MSBFIRST, LEDStatus); // Outputs 0 to 7

  lockOutputs(); // Locks in outputs
}

void lockOutputs () {
  // Clocks the output latches on the shift registers
  digitalWrite(storageClockPin, LOW);   // Sets the storage clock low for the rising edge
  delayMicroseconds(10); // Minor delay
  digitalWrite(storageClockPin, HIGH);  // Rising edge to lock in outputs
}

void setUpperRow (word row) {
  // Sets upper row to a word
  for (int i = 0; i < 16; i++) {
    bitWrite(LEDStatus, (15 - i), bitRead(row, i));
  }
}

void setLowerRow (word row) {
  // Sets lower row to a word
  for (int i = 0; i < 16; i++) {
    bitWrite(LEDStatus, (16 + i), bitRead(row, i));
  }
}
