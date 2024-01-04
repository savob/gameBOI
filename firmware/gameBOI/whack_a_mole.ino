/* Whack a mole

    The user's cursor bouces side to side and they attempt to
    hit the multiple targets in the bottom row. The goal is to
    score as many points in the time allowed. Hits add points,
    and although there is no penalty for passing a target,
    points are deducted when a shoot is fired without a target.

    The cursor speed is constant for the duration of the game.
*/

void whackAMole() {
  const byte gamePeriod = 150;      // Period for game
  const byte targetsPresent = 6;    // Targets present in at any moment

  const byte scoreAward = 5;  // Point awarded for a successful hit
  const byte scorePenalty = 2;      // Points lost on a miss

  static unsigned long endTime = 0;         // Records time to end
  const unsigned long gameLength = 30000;   // Game time in ms

  if (firstFrame) {
    // Setup
    cursorBounce = true;    // Scanning cursor
    LEDOn(cursorPosition);  // Display Cursor

    endTime = millis() + gameLength;  // Records start time
    period = gamePeriod;              // Custom period
    
    // Adds targets
    addTargets(targetsPresent, 0);
    updateLEDs();
  }
  switch (targetStatus()) {
    case 1:
      // Penalty for a misfire
      if (score > scorePenalty) score -= scorePenalty; // Avoid going into negatives with unsigned variables
      else score = 0;
      break;
    case 3:
      // Successful hit
      score += scoreAward;
      LEDOff(31 - cursorPosition);  // Removes target
      addTargets(1,0);              // Adds a new target
      break;
  }

  if (millis() > endTime) gameover(); // Ends when time's up
  
  moveCursor();
}
