#include "player.h"

// Constructor.
Player::Player(int ledPin, int encoderPinA, int encoderPinB, int switchPin)
{
  // Set field variables:
  this->ledPin = ledPin;
  this->encoderPinA = encoderPinA;
  this->encoderPinB = encoderPinB;
  this->switchPin = switchPin;
}

void Player::initialize()
{
  // Initialize the rotary encoder:
  pinMode(this->encoderPinA, INPUT);
  pinMode(this->encoderPinB, INPUT);
  pinMode(this->switchPin, INPUT);
  digitalWrite(this->switchPin, HIGH);
  this->aLastState = digitalRead(this->encoderPinA);

  // Set a random color for this player:
  randomSeed(analogRead(0));
  this->color = pixels.Color(random(0, 255), random(0, 255), random(0, 255));

  // Disable player input per default:
  this->playerInput = false;

  // Initialize NeoPixel strip:
  pixels = Adafruit_NeoPixel(6, ledPin, NEO_GRB + NEO_KHZ800);
  pixels.begin();

  // Turn off all LEDs initially:
  turnOffPixels();
}

int Player::getCurrentPosition()
{
  return this->currentPosition;
}

// Called from the main loop of the Arduino sketch.
void Player::update()
{
  // Only update if we want user input:
  if (this->playerInput)
  {
    updateRotaryEncoder();
    updateNeoPixels();
    checkClick();
  }
}

// Set player enabled:
void Player::enablePlayerInput(bool enable)
{
  this->playerInput = enable;
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
      if (this->secretPosition < pixels.numPixels() - 1)
      {
        this->secretPosition++;
        turnOffPixels();
      }
    } else
    // Or are we rotating anti-clockwise:
    {
      if (this->secretPosition > 0)
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
      Serial.println(currentPosition);
    }
  }
}

// =======================================
// =========== PIXEL FUNCTIONS ===========
// =======================================

// Turn all LEDs off on the NeoPixel strip.
void Player::turnOffPixels() {
  for (unsigned int i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
}

// Turn on the LED on position to blue.
void Player::setPixel(int position)
{
  pixels.setPixelColor(position, 0, 0, 255);
  pixels.show();
}

// Set the current light strategy:
void Player::setLightStrategy(Colorstrategy strategy, uint32_t color)
{
  currentColorStrategy = strategy;
  strategyColor = color;
}

// Set blinking mode with a respective color.
void Player::updateBlink()
{
  stopLightStrategy(8);

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
  stopLightStrategy(40);

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
      int startIndex = 3 - (numOfTimes % 4);
      int endIndex = 2 + (numOfTimes % 4);

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
void Player::stopLightStrategy(int blinkNumOfTimes)
{
  if (numOfTimes == blinkNumOfTimes)
  {
    currentColorStrategy = NORMAL;
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
