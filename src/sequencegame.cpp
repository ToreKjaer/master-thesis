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
    // Stop showing the sequence if all of the sequence has been displayed
    // and turn off the shown sequence:
    if (lastShownIndex == 5)
    {
      for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
      {
        (*iterator).turnOffPixels();
      }
      currentStatus = PLAYING;
      return;
    }

    for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
    {
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
  bool foundWinner = false;
  int currentPlayerSequence = 0;

  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    // Enable player input:
    (*iterator).enablePlayerInput(true);

    // Iterate through all entries in the array:
    for (int i = 0; i < 5; i++)
    {
      // Keep track of which number we have already remembered correctly:
      if (playerSequences[currentPlayerSequence][i] < 0)
      {
        // Only accept a number if it is in consecutive order:
        if ((*iterator).getCurrentPosition() == sequence[i] + (*iterator).getOffset())
        {
          playerSequences[currentPlayerSequence][i] = (*iterator).getCurrentPosition();
          for (int j = 0; j < 5; j++) {
            Serial.println(playerSequences[currentPlayerSequence][j]);
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

    // Check if a player has won (check to see if the last entry in the individual player sequence array is > 0):
    if (playerSequences[currentPlayerSequence][4] >= 0) {
      foundWinner = true;
      currentStatus = DONE;
    }

    // Increment the player sequence counter:
    currentPlayerSequence++;
  }

  // If a winner is found, iterate through the players again to display winner/looser fanfare:
  if (foundWinner)
  {
    currentPlayerSequence = 0;
    for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
    {
      if (playerSequences[currentPlayerSequence][4] >= 0) {
        (*iterator).setLightStrategy(FANFARE, (*iterator).getUint32Color(0, 255, 0));
      }
      else if (playerSequences[currentPlayerSequence][4] < 0 && foundWinner)
      {
        (*iterator).setLightStrategy(FANFARE, (*iterator).getUint32Color(255, 0, 0));
      }
      currentPlayerSequence++;
    }
  }
}
