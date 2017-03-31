// This define prevents multiple inclusion on compilation.
#ifndef sequencegame
#define sequencegame

#include "Arduino.h"
#include "player.h"
#include <StandardCplusplus.h>
#include <vector>

enum Gamestatus { SHOW_SEQUENCE, PLAYING, DONE };

class SequenceGame {

  int sequence[5];
  int sequence_PL_1[5] = {-1, -1, -1, -1, -1};

  Gamestatus currentStatus;

  unsigned long lastUpdate;

  int lastShownIndex;

  // Private methods:
  void updateShowSequence(Player& player);
  void updatePlaying(Player& player);

public:
  SequenceGame();

  void startGame();
  void update(std::vector<Player>& players);
};

#endif
