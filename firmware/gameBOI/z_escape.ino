/* Escape

    The objective is to find holes in the wall to jump through.
    The cursor scans and you must select any of the openings. If
    all openings are passed in a round, gameover.

    The cursor will speed up and the will be fewer openings as
    time progresses.
*/

void escape() {
  const byte defaultOpenings = 10;       // Stores
  static byte openings;                 // Openings available in the round
  const byte openingChangePeriod = 6;   // How often to decrease exits
  static byte stepCount;                // Used to keep track of when to decrease openings
  const byte minOpenings = 4;           // Minimum openings

  const byte marginWidth = 3;           // Margins at each end
  const byte periodChange = 2;          // Adjustment to period each round
  static byte openingsAvailable = 0;    // Running tally of openings

  static bool overOpening = false;      // Used to alert when a passover occurs
  static byte lastPosition = 0;         // Stores last position of 
  
  if (firstFrame) {
    // Setup
    cursorBounce = true;    // Scanning cursor
    LEDOn(cursorPosition);  // Display Cursor

    // Default values
    openings = defaultOpenings;
    stepCount = openingChangePeriod;
    
    // Adds openings
    addMargins(marginWidth);
    openingsAvailable = 16 - addTargets(16 - (openings + marginWidth), marginWidth); // Leaves just the right amount of openings
    updateLEDs();
  }
  switch (targetStatus()) {
    case 0:
      // Check it is on a new position
      if (lastPosition != cursorPosition) {
        if (overOpening) openingsAvailable--;   // Reduces count if needed (last one was an opening)
        else overOpening = true;                // Updates that it is passing a gap now
      }
      break;
    case 1:
      // Selected an opening
      score++;
      overOpening = false; // Clears potential status

      // Moves cursor to ends
      LEDOff(cursorPosition);
      if (cursorDirection) cursorPosition = 15;
      else cursorPosition = 0;
      LEDOn(cursorPosition);  // Display Cursor
      
      addMargins(marginWidth); // Adds margin
      
      // Updates period
      decreasePeriod(periodChange);

      // Reduces exits periodically
      stepCount--;
      if ((stepCount == 0) && (openings > minOpenings)) {
        stepCount = openingChangePeriod; // Reset count
        openings--;
      }

      // Render the new screen for a few moments for the user to process
      openingsAvailable = 16 - addTargets(16 - (openings + marginWidth), marginWidth); // Leaves just the right amount of openings
      updateLEDs();
      
      delay(period * 4); // Delay to process the setup
      break;
    case 2:
      // Potenetially passed an opening
      if (overOpening == true) {
        // If there just was an opening
        openingsAvailable--; // Reduces count
        overOpening = false; // Do not repeat this check
      }
      break;
    case 3:
      // Hit a wall
      gameover();
      break;
  }
  
  lastPosition = cursorPosition; // Updates record
  moveCursor();
  
  if (openingsAvailable == 0) gameover(); // Gameover if no more openings available
}

void addMargins(byte width) {
  // Clears bottom row and adds margins
  setLowerRow(0);

  byte start = 0;
  if (cursorPosition == 15) {
    start = 16 - width;
  }

  for (byte i = 0; i < width; i++) {
    LEDOn(31- (i + start)); // Adds margins where needed
  }
}
