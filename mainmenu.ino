void mainMenu() {
  unsigned long start = millis(); // used to determine the length of button presses
  // Note that the inputs will be low when pressed
  bool cyclePressed = !(digitalRead(cycleButton));
  bool selectPressed = !(digitalRead(selectButton));

  // Display stuff
  resetLEDs();      // Clears old stuff
  LEDOn(mode);      // Shows mode (since the rest of the lights were just cleared)
  showHighscore();  // Shows accompanying highscore and updates display

  // Reset highscores?
  while (!(digitalRead(cycleButton) || digitalRead(selectButton))) {
    // Both pressed
    delay(100);
  }
  if (cyclePressed && selectPressed && ((millis() - start) > 15000)) {
    // Both were pressed for longer than 15 seconds, reset high scores
    resetHighscores();
  }

  // Enter sleep or cycle mode
  while (!(digitalRead(cycleButton)) && digitalRead(selectButton)) {
    // Only cycle pressed
    delay(10);
  }
  if (cyclePressed && !(selectPressed) && ((millis() - start) > 5000) && ((millis() - start) < 10000)) {
    // Cycle pressed for between 5 and 10 seconds
    enterSleep(); // Enter sleep
  }
  else if (cyclePressed && !(selectPressed) && (millis() - start) < 5000) {
    // Short press, increment mode selection
    mode++;
    if (mode == numberOfModes) mode = 0; // Loops back around at the end
  }

  // Select mode?
  while (!(digitalRead(selectButton)) && digitalRead(cycleButton)) {
    // Select pressed
    delay(10);
  }
  if (!(cyclePressed) && selectPressed && ((millis() - start) > 10)) {
    // Select pressed
    selectMode();
  }

  delay(10); // Delay for even when no buttons are pressed
}

void selectMode() {
  runningGame = true; // Time to play...
  score = 0;          // Resets score
  resetLEDs();        // Clears menu
  firstFrame = true;  // Notes it's going to be the first frame of the game
  period = maxPeriod; // Sets default period
  
  // Sets default cursor parameters
  cursorPosition = 0;
  cursorDirection = true;
}

void enterSleep() {
  // Enter lowpower mode
  disableOutput(); // Start by reseting and turning off lights
  attachInterrupt(digitalPinToInterrupt(cycleButton),wakeUp, FALLING);  // Wake up interupt

  // Go to sleep until awoken by cycle button
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  detachInterrupt(digitalPinToInterrupt(cycleButton)); // Remove interupt
  
  enableOutput(); // Re-enable lights and go back to menu 
}

void wakeUp() {
  // Just exsists to awaken board
}
