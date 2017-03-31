#include "sequencegame.h"

SequenceGame::SequenceGame()
{
  // Set field variables:
  lastShownIndex = 0;
}

void SequenceGame::startGame()
{
  // Set an initial value for the lastUpdate variable:
  lastUpdate = millis();

  // Populate the sequence with 4 random numbers:
  Serial.println("Sequence:");
  randomSeed(analogRead(0));
  for (int i = 0; i < 5; i++)
  {
    // Create random number:
    int randomNumber = random(0, 5);

    // If the random number is the same as before
    // do some magic to it:
    if (randomNumber == sequence[i - 1])
    {
      randomNumber = (randomNumber * i) % i;
    }

    // If the value is still the same, just add 1:
    if (randomNumber == sequence[i - 1])
    {
      randomNumber++;
    }

    sequence[i] = randomNumber;
    Serial.print((String) randomNumber + ", ");
  }
  Serial.println("");

  // TODO: Create start signal.

  // Set the current status of the game:
  currentStatus = SHOW_SEQUENCE;
}

void SequenceGame::update(Player& p1)
{
  switch (currentStatus)
  {
    case SHOW_SEQUENCE:
      updateShowSequence(p1);
      break;

    case PLAYING:
      updatePlaying(p1);
      break;

    default:
      break;
  }
}

void SequenceGame::updateShowSequence(Player& p1)
{
  unsigned long interval = 1000; // ms

  // Show the next pixel in the sequence if the interval has passed
  // and increment the last shown index accordingly:
  if (millis() - lastUpdate >= interval)
  {
    // Stop showing the sequence if all of the sequence has been displayed
    // and turn off the shown sequence:
    if (lastShownIndex == 5)
    {
      currentStatus = PLAYING;
      p1.turnOffPixels();
      return;
    }

    p1.turnOffPixels();
    p1.setPixel(sequence[lastShownIndex]);
    lastShownIndex++;

    // Update the last updated variable:
    lastUpdate = millis();
  }
}

void SequenceGame::updatePlaying(Player& p1)
{
  // TODO: Iterate through all four players.

  // Enable player input:
  p1.enablePlayerInput(true);

  // Iterate through all entries in the array:
  for (int i = 0; i < 5; i++)
  {
    // Keep track of which number we have already remembered correctly:
    if (sequence_PL_1[i] < 0)
    {
      // Only accept a number if it is in consecutive order:
      if (p1.getCurrentPosition() == sequence[i])
      {
        sequence_PL_1[i] = p1.getCurrentPosition();
        for (int j = 0; j < 5; j++) {
          Serial.println(sequence_PL_1[j]);
        }

        // Blink green if the player chose the right number:
        p1.setLightStrategy(BLINK, p1.getUint32Color(0, 255, 0));
      }
      else
      {
        // TODO: Blink red when bad choice.
        break;
      }
    }
  }

  // Check if a player has won (check to see if the last entry is > 0):
  if (sequence_PL_1[4] >= 0) {
    p1.setLightStrategy(FANFARE, p1.getUint32Color(0, 255, 0));
    currentStatus = DONE;
  }
}
