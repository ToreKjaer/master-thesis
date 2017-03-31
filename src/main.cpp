#include "Arduino.h" // Must have include in order to use Arduino specific functions.
#include "player.h"
#include "sequencegame.h"
#include <StandardCplusplus.h>
#include <vector>

using namespace std;

// Players:
std::vector<Player> players;

// Games:
SequenceGame sequenceGame;

void setup()
{
  // Initialize the serial communication:
  Serial.begin(9600);
  Serial.println("Serial started.");

  // Add players to the vector:
  players.push_back(Player(2, 11, 12, 4));
  players.push_back(Player(3, 13, 14, 7));
  players.push_back(Player(5, 15, 16, 8));
  players.push_back(Player(6, 17, 18, 9));

  for (int i = 0; i < 4; i++)
  {
    players.at(i).initialize();
  }

  // TODO: Start the game from the Serial.
  sequenceGame.startGame();

  // TODO: Look into if we need to delete objects after using them.. To prevent memory leaks. Could be done through a delete() callback passed to the given game object.
}

void loop()
{
  // Update players:
  for (unsigned int i = 0; i < players.size(); i++)
  {
    players.at(i).update();
  }

  // Update the game:
  sequenceGame.update(players);
}
