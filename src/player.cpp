#include "player.h"

// Constructor.
Player::Player(int offset, int encoderPinA, int encoderPinB, int switchPin, uint32_t color)
{
  // Set field variables:
  this->offset = offset;
  this->secretPosition = offset;
  this->encoderPinA = encoderPinA;
  this->encoderPinB = encoderPinB;
  this->switchPin = switchPin;
  this->color = color;
}

void Player::initialize()
{
  // Initialize the rotary encoder:
  pinMode(this->encoderPinA, INPUT);
  pinMode(this->encoderPinB, INPUT);
  pinMode(this->switchPin, INPUT);
  digitalWrite(this->switchPin, HIGH);
  this->aLastState = digitalRead(this->encoderPinA);

  // Disable player input per default:
  this->playerInput = false;

  // Initialize NeoPixel strip:
  pixels = Adafruit_NeoPixel(24, 2);
  pixels.begin();

  // Turn off all LEDs initially:
  turnOffPixels();
}

int Player::getCurrentPosition()
{
  return this->currentPosition;
}

void Player::setSecretPosition(int position)
{
  secretPosition = position;
}

int Player::getOffset()
{
  return offset;
}

// Called from the main loop of the Arduino sketch.
void Player::update()
{
  // Only update if we want user input:
  if (rotaryEncoderInput)
  {
    updateRotaryEncoder();
  }

  if (playerInput)
  {
    delay(1); // NB! Is somehow needed, otherwise the LEDs will flicker randomly!!
    updateNeoPixels();
    checkClick();
  }
}

// Set player enabled:
void Player::enablePlayerInput(bool enable)
{
  playerInput = enable;
}

// Set the rotary encoder input:
void Player::enableRotaryEncoder(bool enable)
{
  rotaryEncoderInput = enable;
}

bool Player::isEnabled()
{
  return playerInput;
}

// Update the state of the rotary encoder if af pulse has occured.
void Player::updateRotaryEncoder()
{
  this->aState = digitalRead(this->encoderPinA);
  if (this->aState != this->aLastState)
  {
    // Are we rotating clockwise:
    if (digitalRead(this->encoderPinB) != this->aState)
    {
      if (this->secretPosition < offset + 6 - 1)
      {
        this->secretPosition++;
        turnOffPixels();
      }
    } else
    // Or are we rotating anti-clockwise:
    {
      if (this->secretPosition > offset)
      {
        this->secretPosition--;
        turnOffPixels();
      }
    }
  }
  this->aLastState = this->aState;
}

// Update which LED should light up according to the current
// position of the rotary encoder.
void Player::updateNeoPixels() {
  switch (currentColorStrategy)
  {
    case OFF:
      currentColorStrategy = NORMAL;
      currentPosition = -1; // Reset "click" position.
      break;

    case NORMAL:
      pixels.setPixelColor(secretPosition, color);
      pixels.show();
      break;

    case BLINK:
      updateBlink();
      break;

    case FANFARE:
      updateFanfare();
      break;
  }
}

// Check to see if the button on the rotary encoder has been pressed.
void Player::checkClick()
{
  if (digitalRead(this->switchPin))
  {
    if (btnPressed)
    {
      // Button not pressed.
      btnPressed = false;
    }
  } else
  {
    if (!btnPressed)
    {
      // Button pressed.
      btnPressed = true;
      currentPosition = secretPosition;
    }
  }
}

// =======================================
// =========== PIXEL FUNCTIONS ===========
// =======================================

// Turn all LEDs off on the NeoPixel strip.
void Player::turnOffPixels() {
  for (unsigned int i = offset; i < offset + 6; i++)
  {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
}

// Turn on the LED on position to a green'ish color.
void Player::setPixel(int position)
{
  pixels.setPixelColor(position + offset, 60, 212, 49);
  pixels.show();
}

// Set the current light strategy:
void Player::setLightStrategy(Colorstrategy strategy, uint32_t color)
{
  currentColorStrategy = strategy;
  strategyColor = color;
}

Colorstrategy Player::getLightStrategy()
{
  return currentColorStrategy;
}

// Set blinking mode with a respective color.
void Player::updateBlink()
{
  stopLightStrategy(8, NORMAL);

  unsigned int interval = 50; // ms

  if (millis() - lastUpdate >= interval)
  {
    if (pixelIsOn)
    {
      // Turn off the LED:
      turnOffPixels();
      pixelIsOn = false;
    }
    else
    {
      pixels.setPixelColor(secretPosition, strategyColor);
      pixels.show();
      pixelIsOn = true;
    }

    lastUpdate = millis();
    numOfTimes++;
  }
}

void Player::updateFanfare()
{
  stopLightStrategy(40, OFF);

  unsigned int interval = 100; // ms

  if (millis() - lastUpdate >= interval)
  {
    // Skip the first iteration:
    if (numOfTimes % 4 == 0)
    {
      turnOffPixels();
    }
    else
    {
      int startIndex = offset + 3 - (numOfTimes % 4);
      int endIndex = offset + 2 + (numOfTimes % 4);

      turnOffPixels();
      for (int i = startIndex; i <= endIndex; i++)
      {
        pixels.setPixelColor(i, strategyColor);
        pixels.show();
      }
    }

    lastUpdate = millis();
    numOfTimes++;
  }
}

// Stop blinking if we have done it X times and reset values:
void Player::stopLightStrategy(int blinkNumOfTimes, Colorstrategy strategy)
{
  if (numOfTimes == blinkNumOfTimes)
  {
    currentColorStrategy = strategy;
    lastUpdate = 0;
    pixelIsOn = false;
    numOfTimes = 0;
    return;
  }
}

// Return a color in uint32_t from RGB values.
uint32_t Player::getUint32Color(int r, int g, int b)
{
  return pixels.Color(r, g, b);
}
