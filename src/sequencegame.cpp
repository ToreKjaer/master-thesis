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

void SequenceGame::update(std::list<Player>& players)
{
  switch (currentStatus)
  {
    case SHOW_SEQUENCE:
      updateShowSequence(players);
      break;

    case PLAYING:
      updatePlaying(players);
      break;

    default:
      break;
  }
}

void SequenceGame::updateShowSequence(std::list<Player>& players)
{
  unsigned long interval = 1000; // ms

  // Show the next pixel in the sequence if the interval has passed
  // and increment the last shown index accordingly:
  if (millis() - lastUpdate >= interval)
  {
    if (lastShownIndex == 5)
    {
      currentStatus = PLAYING;
    }

    for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
    {
      // Stop showing the sequence if all of the sequence has been displayed
      // and turn off the shown sequence:
      if (lastShownIndex == 5)
      {
        (*iterator).turnOffPixels();
        return;
      }

      // Set the next pixel to ON in the sequence:
      (*iterator).turnOffPixels();
      (*iterator).setPixel(sequence[lastShownIndex]);
    }

    // Increment index:
    lastShownIndex++;

    // Update the last updated variable:
    lastUpdate = millis();
  }
}

void SequenceGame::updatePlaying(std::list<Player>& players)
{
  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    // Enable player input:
    (*iterator).enablePlayerInput(true);

    // Iterate through all entries in the array:
    for (int i = 0; i < 5; i++)
    {
      // Keep track of which number we have already remembered correctly:
      if (sequence_PL_1[i] < 0)
      {
        // Only accept a number if it is in consecutive order:
        if ((*iterator).getCurrentPosition() == sequence[i])
        {
          sequence_PL_1[i] = (*iterator).getCurrentPosition();
          for (int j = 0; j < 5; j++) {
            Serial.println(sequence_PL_1[j]);
          }

          // Blink green if the player chose the right number:
          (*iterator).setLightStrategy(BLINK, (*iterator).getUint32Color(0, 255, 0));
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
      (*iterator).setLightStrategy(FANFARE, (*iterator).getUint32Color(0, 255, 0));
      currentStatus = DONE;
    }
  }
}
