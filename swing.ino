/* Swing

    The objective of this game is to hit every target as they
    appear on the bottom row (only one target will be present
    at any given moment). The cursor will bounce side to side
    gaining speed with every hit and randomly changing
    direction after hits.

    Should the user pass or miss a target, the game ends.
*/

void swing() {
  const byte periodChange = 3;      // Period change per hit
  static bool overTarget = false;   // Used to alert when a passover occurs
  const byte scoreAward = 1;        // Point awarded for a successful hit
  const byte clearance = 6;         // Clearance between targets

  if (firstFrame) {
    // Setup
    cursorBounce = true;    // Scanning cursor
    LEDOn(cursorPosition);  // Display Cursor

    // Adds targets
    addTargets(1, clearance);
    updateLEDs();
  }
  switch (targetStatus()) {
    case 0:
      if (overTarget) gameover(); // Missed the target
      break;
    case 1:
      gameover(); // Fired at nothing
      break;
    case 2:
      overTarget = true; // Passing a target
      break;
    case 3:
      // Successful hit
      score += scoreAward;
      
      cursorDirection = random(0,2);// Random direction change
      
      LEDOff(31 - cursorPosition);  // Removes target
      overTarget = false;           // Reset status
      
      addTargets(1, clearance);     // Adds a new target

      decreasePeriod(periodChange);
      break;
  }

  moveCursor();
}
