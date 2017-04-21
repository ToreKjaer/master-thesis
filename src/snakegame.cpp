#include "snakegame.h"

SnakeGame::SnakeGame()
{
  // Empty constructor.
}

void SnakeGame::startGame(std::list<Player> &players)
{
  // Set an initial value for the lastUpdate variable:
  lastUpdate = millis();
  lastVelocityIncrease = millis();

  // Reset values:
  currentPosition = 0;
  currentEdgePosition = 0;
  velocity = 1000;
  velocityIncreasements = 0;

  // Set the gameEnded bool:
  gameEnded = false;

  // For debug:
  Serial.println("SnakeGame started");

  // Generate random values for each player and reset their scores:
  for (int i = 0; i < 4; i++)
  {
    candyPositions[i] = getRandomPosition();
    playerScores[i] = 0;
  }

  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    // Turn off old pixels for good measure:
    (*iterator).turnOffPixels();

    // Enable the LED and click functionality:
    (*iterator).enablePlayerInput(true);
  }
}

void SnakeGame::update(std::list<Player> &players)
{
  // Only update if the game has not ended:
  if (!gameEnded)
  {
    if (millis() - lastUpdate >= velocity)
    {
      // Switch direction if edges are met:
      if (currentPosition == 5)
      {
        // Go backwards:
        currentEdgePosition = 5;
      }
      else if (currentPosition == 0)
      {
        // Go forwards:
        currentEdgePosition = 0;
      }

      // Go the chosen direction:
      if (currentEdgePosition == 0)
      {
        // Go forwards:
        currentPosition++;
      }
      else if (currentEdgePosition == 5)
      {
        // Go backwards:
        currentPosition--;
      }

      int currentPlayerIndex = 0;
      for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
      {
        // Light the current LED:
        (*iterator).turnOffPixels();
        (*iterator).setSecretPosition(currentPosition + (*iterator).getOffset());

        // Check if a player has pressed the button when on the right "candy" position:
        if ((*iterator).getCurrentPosition() == candyPositions[currentPlayerIndex])
        {
          // Increment the given players score:
          playerScores[currentPlayerIndex]++;

          // Generate a new location for the candy:
          candyPositions[currentPlayerIndex] = getRandomPosition();
        }

        // Light the "candy" LED:
        (*iterator).setPixel(candyPositions[currentPlayerIndex]);

        Serial.print("Player ");
        Serial.print(currentPlayerIndex);
        Serial.print(": ");
        Serial.print(playerScores[currentPlayerIndex]);
        Serial.println("");

        // Increment the current player position:
        currentPlayerIndex++;
      }

      // Update the lastUpdate variable:
      lastUpdate = millis();
    }
  }

  // Increment the velocity by 40% every 6 seconds or so:
  if (millis() - lastVelocityIncrease >= 6000)
  {
    velocity = velocity * 0.6;
    lastVelocityIncrease = millis();
    velocityIncreasements++;
  }

  // After 6 increasements of velocity, find a winner:
  if (velocityIncreasements >= 6)
  {
    // Set the game has ended boolean:
    gameEnded = true;

    // Find the player with the highest score:
    int currentHighestPlayerIndex;
    int currentHighestScore;
    int currentPlayerIndex = 0;
    for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
    {
      if (currentPlayerIndex == 0)
      {
        // This is the first index. Set initial values for comparison and move on:
        currentHighestPlayerIndex = currentPlayerIndex;
        currentHighestScore = playerScores[currentPlayerIndex];
      }

      if (playerScores[currentPlayerIndex] - currentHighestScore > 0)
      {
        // An other player has a higher score than the last:
        currentHighestPlayerIndex = currentPlayerIndex;
        currentHighestScore = playerScores[currentPlayerIndex];
      }

      currentPlayerIndex++;
    }

    // Light the green fanfare for the winner and the red for the losers:
    currentPlayerIndex = 0;
    for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
    {
      if (currentPlayerIndex == currentHighestPlayerIndex)
      {
        // The winner:
        (*iterator).setLightStrategy(FANFARE, (*iterator).getUint32Color(0, 255, 0));
      }
      else
      {
        // The loser:
        (*iterator).setLightStrategy(FANFARE, (*iterator).getUint32Color(255, 0, 0));
      }

      currentPlayerIndex++;
    }
  }
}

int SnakeGame::getRandomPosition()
{
  // Set a random color for this player:
  randomSeed(analogRead(0));
  return random(0, 5);
}
