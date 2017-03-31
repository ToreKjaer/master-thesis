// This define prevents multiple inclusion on compilation.
#ifndef sequencegame
#define sequencegame

#include "Arduino.h"
#include "player.h"

enum Gamestatus { SHOW_SEQUENCE, PLAYING, DONE };

class SequenceGame {

  int sequence[5];
  int sequence_PL_1[5] = {-1, -1, -1, -1, -1};

  Gamestatus currentStatus;

  unsigned long lastUpdate;

  int lastShownIndex;

  // Private methods:
  void updateShowSequence(Player& p1);
  void updatePlaying(Player& p1);

public:
  SequenceGame();

  void startGame();
  void update(Player& p1);
};

#endif
