// This define prevents multiple inclusion on compilation.
#ifndef sequencegame
#define sequencegame

#include "Arduino.h"
#include "player.h"
#include <StandardCplusplus.h>
#include <list>

using namespace std;

enum Gamestatus { SHOW_SEQUENCE, PLAYING, DONE };

class SequenceGame {

  int sequence[5];
  int playerSequences[4][5];

  // Temp array to keep track of the last position in order to display the red
  // blink animation when pressed wrongly:
  int lastPressedPosition[4] = {-1, -1, -1, -1};

  Gamestatus currentStatus;

  unsigned long lastUpdate;

  int lastShownIndex;

  // Private methods:
  void updateShowSequence(std::list<Player>& players);
  void updatePlaying(std::list<Player>& players);

public:
  SequenceGame();

  void startGame();
  void update(std::list<Player>& players);

  Gamestatus getCurrentGameStatus();
};

#endif
