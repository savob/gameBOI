/* Survival

    Cursor scans edge to edge, the goal is to keep knocking down targets
    as they pop up and prevent more than a certain amount being present
    being onscreen at once. There is no time limit nor penalty for misses.
    However, the cursor will speed up over time to compensate the faster
    spawn rate of targets
*/

void survival() {
  const byte targetLimit = 10;    // Targets allowed before loss
  const byte baseTargetCount = 2; // Number of targets at the start
  static byte targetsPresent = 0;
  
  const byte spawnPeriodChange = 5;     // How much the period changes each spawn
  const byte spawnPeriodMinimum = 200;  // The minimum spawn period
  const word spawnPeriodDefault = 700;  // Starting spawn period

  const byte periodChange = 1;          // How much the cursor period changes with each target
  const byte defaultPeriod = 80;        // Default period to start at
  
  static word spawnPeriod;
  static unsigned long nextSpawn = 0;
  
  const byte scoreHit = 1;  // Point awarded for a successful hit

  if (firstFrame) {
    // Setup
    cursorBounce = true;    // Scanning cursor
    LEDOn(cursorPosition);  // Display Cursor
    period = defaultPeriod; // Period
    
    // Adds targets
    targetsPresent = addTargets(baseTargetCount, 0);
    updateLEDs();

    spawnPeriod = spawnPeriodDefault;
    nextSpawn = millis() + spawnPeriod;
  }
  
  if (targetStatus() == 3) {
      // Successful hit
      score += scoreHit;
      LEDOff(31 - cursorPosition);  // Removes target
  }

  if (millis() > nextSpawn) {
    targetsPresent = addTargets(1,0); // Spawns target

    // Gets next spawn period
    if (spawnPeriod > (spawnPeriodMinimum + spawnPeriodChange)) spawnPeriod -= spawnPeriodChange;
    nextSpawn = millis() + spawnPeriod;

    decreasePeriod(periodChange); // Adjust cursor period 
  }

  if (targetsPresent > targetLimit) gameover(); // Checks for gameover
  
  moveCursor();
}
