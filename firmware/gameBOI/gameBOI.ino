/* gameBOI Code

    This is the code for a project of mine, aiming to show off my
    ability to design circuits and code by using an ATmega328P to
    control a series of LEDs using shift registers to make a game
    console that plays a set of one-button games.

    The games also store their highscors in the EEPROM for them
    to make through power-off. There is also the intent to have a
    low power sleep mode so the console doesn't have to have the
    battery constantly connected/disconnected.

    The game list is as follows, they are detailed in their tabs:
       1 - Escape
       2 - Pop can knock off
       3 - Skip rope
       4 - Stacker
       5 - Survival
       6 - Swing
       7 - Whack a mole
*/

#include <EEPROM.h>
#include "LowPower.h"

const byte serialDataPin = 0;     // Data to shift registers
const byte serialClockPin = 4;    // Clock for data (rising edge)
const byte outputEnablePin = 1;   // Enables output of shift registers when LOW
const byte resetDataPin = 5;      // Resets data block of shift registers on LOW - NOT THE OUTPUTS
const byte storageClockPin = 7;   // Moves data from storage to output on rising edge.

const byte selectButton = 2;      // Pin connected to select button (pulled down when pressed)
const byte cycleButton = 3;       // Pin connected to cycle button (pulled down when pressed)

long LEDStatus = 0;               // Stores all LEDs in its bits
// LEDs are counted from 0 in the topleft to 31 going clockwise

// Game related
byte mode = 0;                    // Stores game mode
const byte numberOfModes = 7;     // Stores the number of programmed modes
bool runningGame = false;         // Is a game running or not
unsigned int score = 0;           // Keeps track of score
unsigned int highscores[16];      // Keeps track of high scores
bool firstFrame = true;           // First 'rendered frame' of game? (Used to give people chance to see game starting

const long maxPeriod = 200;       // Maximum / default starting period
const long minPeriod = 50;        // Minimum period between position changes
long period = 50;                 // Stores scrolling period
bool buttonHeld = false;          // Stores whether the select button is being held or not

unsigned long cursorMoveTime = 0; // Next time to move cursor
byte cursorPosition = 0;          // Position of cursor
bool cursorDirection = false;     // Direction of scrolling, right is true
bool cursorBounce = false;        // Does the cursor 'bounce' off edges ('scan')?

void setup() {
  // Output pins
  pinMode(serialDataPin, OUTPUT);
  pinMode(serialClockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);
  pinMode(resetDataPin, OUTPUT);
  pinMode(storageClockPin, OUTPUT);

  // Buttons (they get pulled down on presses
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(cycleButton, INPUT_PULLUP);

  resetLEDs(); // Resets the shift registers
  enableOutput();

  // Reads all highscores from memory to minimize read/write cycles
  for (byte i = 0; i < 16; i++) {
    highscores[i] = EEPROM.get((i * 2), score);

    // After reprogramming over SPI, EEPROM seems to be set to 0xFFFF everywhere.
    // If this is detected, the system will reset highscores
    if (highscores[i] > 10000) {
      resetHighscores();
      break;
    }
  }
}

void loop() {
  if (runningGame) {
    // Run a game
    switch (mode) {
      case 0:
        escape();
        break;
      case 1:
        popCanKnockOff();
        break;
      case 2:
        skipRope();
        break;
      case 3:
        stacker();
        break;
      case 4:
        survival();
        break;
      case 5:
        swing();
        break;
      case 6:
        whackAMole();
        break;
    }
    if (firstFrame) {
      delay(500);               // Let's player process first screen
      randomSeed(millis());     // Randomizes everything
      firstFrame = false;       // No longer first frame
    }
    else delayMicroseconds(500); // Artificial frame rate
  }
  else {
    mainMenu(); // If not playing, it's in the main menu
  }
}

void resetHighscores() {
  // Clears all highscores
  word zero = 0;

  for (int i = 0; i < 16; i++) {
    EEPROM.put(i * 2, zero);
    highscores[i] = zero;
  }
}
