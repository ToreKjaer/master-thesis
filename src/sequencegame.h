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
  int sequence_PL_1[5] = {-1, -1, -1, -1, -1};
  // TODO: Have an array of arrays wrt. individual player sequences.

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
};

#endif
