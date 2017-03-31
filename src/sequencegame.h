// This define prevents multiple inclusion on compilation.
#ifndef sequencegame
#define sequencegame

#include "Arduino.h"
#include "player.h"
#include <StandardCplusplus.h>
#include <vector>

using namespace std;

enum Gamestatus { SHOW_SEQUENCE, PLAYING, DONE };

class SequenceGame {

  int sequence[5];
  int sequence_PL_1[5] = {-1, -1, -1, -1, -1};

  Gamestatus currentStatus;

  unsigned long lastUpdate;

  int lastShownIndex;

  // Private methods:
  void updateShowSequence(std::vector<Player>& players);
  void updatePlaying(std::vector<Player>& players);

public:
  SequenceGame();

  void startGame();
  void update(std::vector<Player>& players);
};

#endif
