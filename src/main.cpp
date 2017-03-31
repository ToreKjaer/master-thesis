#include "Arduino.h" // Must have include in order to use Arduino specific functions.
#include "player.h"
#include "sequencegame.h"

// Players:
Player player1(2, 11, 12, 4);
Player players[4] = {
  Player(2, 11, 12, 4),
  Player(3, 13, 14, 7),
  Player(5, 15, 16, 8),
  Player(6, 17, 18, 9),
};

// Games:
SequenceGame sequenceGame;

void setup()
{
  // Initialize the serial communication:
  Serial.begin(9600);
  Serial.println("Serial started.");

  player1.initialize();

  // TODO: Start the game from the Serial.
  sequenceGame.startGame();

  // TODO: Look into if we need to delete objects after using them.. To prevent memory leaks. Could be done through a delete() callback passed to the given game object.
}

void loop()
{
  // Update players:
  player1.update();

  // Update the game:
  sequenceGame.update(player1);
}
