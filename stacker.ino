/* Stacker

    Classic stacking game. A block is the foundation and another
    block scrolls above it. The user tries to place the moving
    block on top of the other minimizing overhang, any overhang
    is lost and the newly placed block becomes the new base. The
    new block will be of the same width as the base.

    The user is to try and climb as high as possible, before
    they place a block that entirely misses and ends the game.
*/

void stacker() {
  const byte startingTargetWidth = 8; // Stores default target width
  static byte targetWidth;            // Stores current target width

  const byte periodChange = 2;  // Stores the amount the period changes by
  const byte scoreAward = 1;    // The points awarded every successful round

  const word pause = 250;   // Pause period between keyframes during cycle


  if (firstFrame) {
    // Setup
    cursorBounce = true; // Box will bounce side to side

    // Set starting target
    setLowerRow(0); // Clear target row
    targetWidth = startingTargetWidth;
    for (byte i = (8 - (targetWidth / 2)); i < ((8 - (targetWidth / 2)) + targetWidth); i++) {
      LEDOn(31 - i); // Set starting target centered in bottom row
    }
  }

  if (bottonPressed()) {
    // Time to check if it stacked good
    delay(pause);       // Lets user see their placement
    targetWidth = 0;    // Resets target width

    // Checks where block rest
    for (byte i = 0; i < 16; i++) {
      if (bitRead(LEDStatus, i)) {
        if (bitRead(LEDStatus, 31 - i)) targetWidth++;  // Increments width, if there are blocks underneith
        else bitWrite(LEDStatus, i, 0);                 // Removes overhanging blocks
      }
    }
    updateLEDs();

    // Check the width left successfully
    if (targetWidth == 0) gameover(); // Checks if there are none left (gameover)
    else {
      // The game may continue if there are blocks left

      // Moves box from cursor to target level
      delay(pause);
      for (byte i = 0; i < 16; i++) {
        bitWrite(LEDStatus, (31 - i), bitRead(LEDStatus, i));
      }
      setUpperRow(0); // Clears old target
      updateLEDs();
      delay(pause / 2);

      // Admin stuff
      score += scoreAward;
      decreasePeriod(periodChange);
      cursorPosition = 0; // Resets box start
    }
  }

  moveBox(targetWidth); // Renders/moves initial box
  delayMicroseconds(500); // Half a millisecond delay
}

void moveBox(byte width) {
  // Similar to moveCursor, however it works with boxes
  // Uses cursorPosition to record the left side of the box

  if (cursorMoveTime < millis()) {
    cursorMoveTime = millis() + period;   // Records next move time
    setUpperRow(0);         // Clears old box

    // Time to move cursor
    if (cursorDirection) {
      // Moves to the right
      if (cursorPosition == (15 - width)) {
        // Hitting right edge
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
    for (byte i = cursorPosition; i < (cursorPosition + width); i++) {
      LEDOn(i);
    }
    updateLEDs();                   // Redraws screen
  }
}
