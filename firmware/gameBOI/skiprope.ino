/*  Skip Rope game

    The user has a constant target at the bottom and the cursor
    scrolls left to right. The goal is to hit the target repeatedly
    and the cursor speeds up everytime. This continues until they
    miss the target.
*/

void skipRope() {
  static bool overTarget = false;         // Used to monitor if the cursor has passed over a target
  static bool successfulSkip = false;     // Was there a succesfull skip this pass
  const float speedFactor = 0.95;          // Adjustment to period each few rounds

  if (firstFrame) {
    // Setup
    setLowerRow(0x0030);    // Puts targets close to right edge
    cursorBounce = false;   // Scrolling cursor
    LEDOn(cursorPosition);  // Display Cursor
    updateLEDs();
  }

  if (cursorPosition == 0) {
    // Start of a sweep, set sweep variables to default
    successfulSkip = false;
    overTarget = false;
  }
  if (!(successfulSkip)) {
    // If skip hasn't been performed yet
    switch (targetStatus()) {
      case 0:
        if (overTarget && !successfulSkip) gameover(); // If they passed the target without a hit
        break;
      case 1:
        gameover(); // Premptive shot
        break;
      case 2:
        overTarget = true; // Passing over target
        break;
      case 3:
        // Successful skip
        score++;
        successfulSkip = true;
        if (period > (minPeriod/speedFactor)) period *= speedFactor; // Decreases period every round
        break;
    }
  }
  moveCursor();
}
