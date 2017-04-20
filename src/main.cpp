#include "Arduino.h" // Must have include in order to use Arduino specific functions.
#include "player.h"
#include "sequencegame.h"
#include "snakegame.h"
#include <StandardCplusplus.h>
#include <list>

using namespace std;

// Game enums:
enum GameTypes { SEQUENCEGAME, SNAKEGAME };
GameTypes currentGame;

// Players:
std::list<Player> players;

// Games:
SequenceGame sequenceGame;
SnakeGame snakeGame;

bool gameStarted = false;

void setup()
{
  // Initialize the serial communication:
  Serial.begin(9600);
  Serial.println("Serial started.");

  Player p1(0, 11, 12, 4);
  Player p2(6, 13, 14, 5);
  Player p3(12, 15, 16, 7);
  Player p4(18, 17, 18, 8);

  // Add players to the list:
  players.push_back(p1);
  players.push_back(p2);
  players.push_back(p3);
  players.push_back(p4);

  // Initialize players (set pin modes etc.):
  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    (*iterator).initialize();
  }

  // TODO: Look into if we need to delete objects after using them.. To prevent memory leaks. Could be done through a delete() callback passed to the given game object.
}

void loop()
{
  // Start sequence game with command: "seq" in Serial input:
  if (Serial.available() > 0)
  {
    String serialReadTemp = Serial.readString();
    if (serialReadTemp == "seq")
    {
        sequenceGame.startGame();
        gameStarted = true;
        currentGame = SEQUENCEGAME;
    }
    else if (serialReadTemp == "snake")
    {
        snakeGame.startGame(players);
        gameStarted = true;
        currentGame = SNAKEGAME;
    }
  }

  // Update players:
  for(std::list<Player>::iterator iterator = players.begin(); iterator != players.end(); iterator++)
  {
    (*iterator).update();

    // Turn off LEDs when the fanfare is done displaying:
    if ((*iterator).getLightStrategy() == OFF && (*iterator).isEnabled())
    {
      Serial.println("Stopped");
      (*iterator).enablePlayerInput(false);
      gameStarted = false;
    }
  }

  // Update the current game playing:
  if (gameStarted)
  {
    switch (currentGame) {
      case SEQUENCEGAME:
        sequenceGame.update(players);
        break;

      case SNAKEGAME:
        snakeGame.update(players);
        break;
    }

  }
}
