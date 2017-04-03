#include "Arduino.h" // Must have include in order to use Arduino specific functions.
#include "player.h"
#include "sequencegame.h"
#include <StandardCplusplus.h>
#include <list>

using namespace std;

// Players:
std::list<Player> players;

// Games:
SequenceGame sequenceGame;

void setup()
{
  // Initialize the serial communication:
  Serial.begin(9600);
  Serial.println("Serial started.");

  Player p1(2, 11, 12, 4);
  Player p2(8, 13, 14, 5);

  // Add players to the list:
  players.push_back(p1);
  players.push_back(p2);

  // Initialize players (set pin modes etc.):
  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    (*iterator).initialize();
  }

  // TODO: Start the game from the Serial.
  sequenceGame.startGame();

  // TODO: Look into if we need to delete objects after using them.. To prevent memory leaks. Could be done through a delete() callback passed to the given game object.
}

void loop()
{
  // Update players:
  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    (*iterator).update();
  }

  // Update the game:
  sequenceGame.update(players);
}
