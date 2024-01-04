// This tab stores functions shared by all the different games

unsigned int getHighscore() {
  return (highscores[mode]); // Returns highscore
}

void showHighscore() {
  // Shows highscore in highscore row
  setLowerRow(word(getHighscore()));
  updateLEDs();
}

void gameover() {
  // Game over procedure
  // Flash screen, then show score screen

  disableOutput();
  delay(300);
  enableOutput();
  delay(300);
  disableOutput();
  delay(300);
  enableOutput();
  delay(300);
  disableOutput();
  delay(300);
  enableOutput();
  delay(300);

  // Shows current score above highscore as well as updating highscore.

  // Shows score and previous highscore
  setUpperRow(score);
  showHighscore(); // This also updates display
  delay(1000);

  // Highscore updating
  if (score > getHighscore()) {
    EEPROM.put((mode * 2), score); // Updates highscore
    highscores[mode] = score;

    // Flash screen to congradulate
    disableOutput();
    delay(500);
    enableOutput();
    delay(500);
  }

  runningGame = false; // Back to the menu
}

bool bottonPressed() {
  // Returns true if the button has been recently pressed (not held)

  if (buttonHeld) {
    // Button was previously held
    buttonHeld = !digitalRead(selectButton); // Checks if button is still held
    return false;
  }
  else {
    if (!digitalRead(selectButton)) {
      // If the button is pressed now but previously wasn't
      buttonHeld = true; // Record it is being held
      return true;
    }
    else return false; // Button is not pressed
  }
}

bool targetPresent() {
  // Sees if the cursor is currently above a target
  return (bitRead(LEDStatus, 31 - cursorPosition));
}

byte targetStatus() {
  /* Checks if the user has pressed the button and if there is a target present
      0 - No press or target
      1 - Press but no target (Missed shot)
      2 - No press but target present (Potential miss)
      3 - Pressed button with target present (Hit)
  */
  return (2 * targetPresent() + bottonPressed());
}

void moveCursor() {
  // Moves cursor (assuming it's only one LED wide)
  if (cursorMoveTime < millis()) {
    cursorMoveTime = millis() + period;   // Records next move time
    LEDOff(cursorPosition);         // Clears old cursor spot

    // Time to move cursor
    if (cursorDirection) {
      // Moves to the right
      if (cursorPosition == 15) {
        // Edge
        if (cursorBounce) {
          cursorDirection = false;  // Reverse direction (bounce)
          cursorPosition--;         // Move in new direction
        }
        else cursorPosition = 0;    // Wrap around
      }
      else cursorPosition++;        // Move cursor to the right
    }
    else {
      // Moves to the left
      if (cursorPosition == 0) {
        // Edge
        if (cursorBounce) {
          cursorDirection = true;   // Reverse direction (bounce)
          cursorPosition++;         // Move in new direction
        }
        else cursorPosition = 15;   // Wrap around
      }
      else cursorPosition--;        // Move cursor to the right
    }

    // Updates display
    LEDOn(cursorPosition);          // Plots new cursor
    updateLEDs();                   // Redraws screen
  }
}

byte addTargets(byte number, byte minimumDistance) {
  /* Adds targets, returns the targets present at the end

      This will add new targets randomly, given two parameters
       - The quantitiy of targets needed
       - The minimum space from the current cursor position
      It will account for bouncing off the side or not, assumes direction is set

      Starts by defining a working range to generat numbers in, then
      it verifies the amount of open spots in that range and constrains
      the number of targets to be added to this. Then generates indexes
      until we have generated all the targets needed.
  */

  // Variables to store viable range (set to defaults)
  int minimumIndex = 0;
  int maximumIndex = 15;

  if (minimumDistance != 0) {
    // Do we need to worry about a minimum distance? (need to adjust range)
    if (cursorDirection) {
      // Going right
      if (cursorBounce) {
        // If the cursor is going to the edges we don't want to include the
        // return trip as part of the minimum distance so we don't put a target
        // in this margin and scare the user. TO avoid this we always try to keep
        // the target out of the margin.

        if ((15 - cursorPosition) < minimumDistance) {
          // Margin too small
          maximumIndex = cursorPosition - 1;
          minimumIndex = 0;
        }
        else {
          // Space in the margins
          minimumIndex = cursorPosition + minimumDistance;
          maximumIndex = cursorPosition + 15;
        }
      }
      else {
        // Scrolling cursor
        minimumIndex = cursorPosition + minimumDistance;
        maximumIndex = cursorPosition + 15;
      }
    }
    else {
      // Left
      if (cursorBounce) {
        // Same thought process as with bounce going right
        if (cursorPosition < minimumDistance) {
          // Margin too small
          maximumIndex = 15;
          minimumIndex = cursorPosition + 1;
        }
        else {
          // Space in the margins
          maximumIndex = cursorPosition - minimumDistance;
          minimumIndex = cursorPosition - 15;
        }
      }
      else {
        // Scrolling cursor
        maximumIndex = cursorPosition - minimumDistance;
        minimumIndex = cursorPosition - 15;
      }
    }
  }

  // Moves indexs out of negatives
  if (minimumIndex < 0) {
    minimumIndex += 16;
    maximumIndex += 16;
  }

  ////////////////////////////////////////
  // Verifies the amount of open spots in the range
  byte openingsPresent = 0; // Used to count exsisting targets

  for (int i = minimumIndex; i <= maximumIndex; i++) {
    if (!bitRead(LEDStatus, 31 - (i % 16))) openingsPresent++; // Adds 1 if LED is not present
  }
  number = min(number, openingsPresent);     // Limits the number if needed

  ////////////////////////////////////////
  // Generates the new targets needed
  for (byte i = 0; i < number; i++) {
    byte newTarget = 0;

    do {
      newTarget = random(minimumIndex, (maximumIndex + 1)); // Regenerate until a vacancy is found
      newTarget %= 16;                                      // Wraps values around for ranges that gor around limits
    } while (bitRead(LEDStatus, 31 - newTarget));

    LEDOn(31 - newTarget); // Adds target
  }

  byte targetsPresent = 0; // Resets count
  for (byte i = 16; i < 32; i++) {
    // Counts all targets (including those outside of generation range
    if (bitRead(LEDStatus, i)) targetsPresent++; // Adds 1 if LED is present, 0 if not
  }

  return targetsPresent;    // Returns targets present
}
void decreasePeriod(long increment) {
  // Decreases period by the increment if allowed
  if (period >= (minPeriod + increment)) period -= increment;
}
