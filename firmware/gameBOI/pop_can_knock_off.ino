/* Pop-can knock off

    Cursor scans and the goal is to hit targets. Several targets
    appear at once and they must all be hit before the next round
    There is no time limit or penalty for passing targets, but
    missing will end the game.

    Each round will have the same amount of targets, however the
    cursor speed will gradually increase. There is a point bonus
    for each complete round finished
*/

void popCanKnockOff() {
  const byte targetsPerRound = 4;   // Targets present in each round
  const byte periodChange = 5;      // Adjustment to period each round
  static byte targetsPresent = 0;   // Running tally of targets

  const byte targetPoints = 1;      // Points from target hit
  const byte roundBonus = 10;       // Bonus after each completed round
  if (firstFrame) {
    // Setup
    cursorBounce = true;    // Scanning cursor
    LEDOn(cursorPosition);  // Display Cursor

    // Adds targets
    targetsPresent = addTargets(targetsPerRound, 0);
    updateLEDs();
  }
  switch (targetStatus()) {
    case 1:
      gameover(); // Premptive shot
      break;
    case 3:
      // Successful hit
      score += targetPoints;
      targetsPresent--;             // Decreases count
      LEDOff(31 - cursorPosition);  // Removes target

      // Checks if the screen has been cleared
      if (targetsPresent == 0) {
        targetsPresent = addTargets(targetsPerRound, 0); // Updates target count
        score += roundBonus; // Round bonus

        // Updates period
        decreasePeriod(periodChange);
      }
      break;
  }
  moveCursor();
}
