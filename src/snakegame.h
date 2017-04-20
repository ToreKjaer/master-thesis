// This define prevents multiple inclusion on compilation.
#ifndef snakegame
#define snakegame

#include "Arduino.h"
#include "player.h"
#include <StandardCplusplus.h>
#include <list>

class SnakeGame
{
  unsigned long lastUpdate, lastVelocityIncrease;
  unsigned int velocityIncreasements = 0;

  // Positions used to determine direction:
  int currentPosition;
  int currentEdgePosition;

  // Player velocity:
  unsigned long velocity = 1000; // ms

  // Player "candy" positions:
  int candyPositions[4];

  int playerScores[4];

  bool gameEnded;

public:
  SnakeGame();

  void startGame(std::list<Player> &players);
  void update(std::list<Player>& players);

  int getRandomPosition();
};

#endif
