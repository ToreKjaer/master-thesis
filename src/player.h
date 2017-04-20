// This define prevents multiple inclusion on compilation.
#ifndef player
#define player

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

enum Colorstrategy { OFF, NORMAL, BLINK, FANFARE };

class Player
{
  // encoderPinA is the right most pin on the rotary encoder seen from above with the three pins pointing upwards.
  // encoderPinB is the left most pin on the rotary encoder seen from above with the three pins pointing upwards.
  int encoderPinA, encoderPinB, switchPin;

  // The current position of the LED:
  unsigned int secretPosition, currentPosition = -1, offset;
  Colorstrategy currentColorStrategy = NORMAL;

  // Variables to determine the rotary encoder's position:
  int aState;
  int aLastState;

  Adafruit_NeoPixel pixels;
  uint32_t color; // Color of the NeoPixels.

  bool playerInput, rotaryEncoderInput, btnPressed;

  // Pixel blink variables:
  unsigned long lastUpdate = 0;
  uint32_t strategyColor;
  bool pixelIsOn = false;
  int numOfTimes = 0;

  // Private methods:
  void updateRotaryEncoder();
  void updateNeoPixels();
  void checkClick();
  void stopLightStrategy(int blinkNumOfTimes, Colorstrategy strategy);

public:
  Player(int offset, int encoderPinA, int encoderPinB, int switchPin);

  void initialize();

  int getCurrentPosition();
  void setSecretPosition(int position);
  void update();
  void enablePlayerInput(bool enable);
  void enableRotaryEncoder(bool enable);
  bool isEnabled();
  int getOffset();

  // NeoPixel methods:
  void turnOffPixels();
  void setPixel(int position);
  void setLightStrategy(Colorstrategy strategy, uint32_t color);
  Colorstrategy getLightStrategy();
  void updateBlink();
  void updateFanfare();
  uint32_t getUint32Color(int r, int g, int b);
};

#endif
