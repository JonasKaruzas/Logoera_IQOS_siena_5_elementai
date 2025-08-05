#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define LED_PIN      7
#define BUTTON_PIN   13
#define NUM_LEDS     92

#define RED_PIN      A0
#define GREEN_PIN    A1
#define BLUE_PIN     A2
#define BRIGHT_PIN   A3
#define SPEED_PIN    A6

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// LED Groups
const uint8_t groupStarts[] = {0, 42, 52, 62, 72, 82}; // 0-based indexing
const uint8_t groupEnds[]   = {41, 51, 61, 71, 81, 91}; // inclusive

#define NUM_MODES 9
/*
  Modes:
  0 - Blink groups 2–6 (Group 1 always on)
  1 - Fade groups 2–6 (Group 1 always on)
  2 - Fade all groups (including Group 1)
  3 - Blink all groups (including Group 1)
  4 - Crossfade all groups
  5 - Crossfade groups 2–6 (Group 1 always on)
  6 - Random blink (like Mode 0)
  7 - Random fade (like Mode 1)
  8 - Random crossfade (like Mode 5)
*/

uint8_t currentMode = 0;
unsigned long lastButtonPress = 0;
bool lastButtonState = HIGH;

unsigned long lastUpdate = 0;
int animationStep = 0;

int randomOrder[6];

void setup() {
  strip.begin();
  strip.show();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  EEPROM.get(0, currentMode);
  if (currentMode >= NUM_MODES) currentMode = 0;

  randomSeed(analogRead(A5));
  shuffleOrder(1, 5);
  Serial.begin(9600);
}

void loop() {
  handleButton();
  updateAnimation();
}

void handleButton() {
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastButtonPress > 300) {
    currentMode = (currentMode + 1) % NUM_MODES;
    EEPROM.put(0, currentMode);
    lastButtonPress = millis();
    animationStep = 0;
  }
  lastButtonState = buttonState;
}

void shuffleOrder(int startIdx, int endIdx) {
  int size = endIdx - startIdx + 1;
  for (int i = 0; i < size; i++) {
    randomOrder[i] = startIdx + i;
  }
  for (int i = size - 1; i > 0; i--) {
    int j = random(i + 1);
    int temp = randomOrder[i];
    randomOrder[i] = randomOrder[j];
    randomOrder[j] = temp;
  }
}

void updateAnimation() {
  uint8_t red = analogRead(RED_PIN) / 4;
  uint8_t green = analogRead(GREEN_PIN) / 4;
  uint8_t blue = analogRead(BLUE_PIN) / 4;
  float brightness = analogRead(BRIGHT_PIN) / 1023.0;
  int speed = map(analogRead(SPEED_PIN), 0, 1023, 100, 1000);

  unsigned long now = millis();
  if (now - lastUpdate < speed) return;
  lastUpdate = now;

  bool fade = (currentMode == 1 || currentMode == 2 || currentMode == 7);
  bool crossfade = (currentMode == 4 || currentMode == 5 || currentMode == 8);
  bool includeGroup1 = (currentMode >= 2 && currentMode <= 4) || currentMode == 7;
  bool isRandomStep = (currentMode >= 6);

  int startIndex = (includeGroup1 || crossfade) ? 0 : 1;
  int endIndex = 5;

  if (animationStep == 0 && isRandomStep) {
    shuffleOrder(startIndex, endIndex);
  }

  int groupIndex = isRandomStep ? randomOrder[animationStep] : (startIndex + animationStep);
  int nextGroup = isRandomStep ? randomOrder[(animationStep + 1 > (endIndex - startIndex)) ? 0 : animationStep + 1]
                               : ((groupIndex + 1 > endIndex) ? startIndex : groupIndex + 1);

  if (crossfade) {
    if (currentMode == 5 || currentMode == 8) fillGroup(0, red, green, blue, brightness);  // Group 1 always on

    for (float t = 0; t <= 1.0; t += 0.02) {
      strip.clear();
      if (currentMode == 5 || currentMode == 8) fillGroup(0, red, green, blue, brightness);

      float tOut = 1.0 - t;
      float tIn = t;

      fillGroup(groupIndex, red * tOut, green * tOut, blue * tOut, brightness * tOut);
      fillGroup(nextGroup, red * tIn, green * tIn, blue * tIn, brightness * tIn);
      strip.show();
      delay(15);
    }
  }
  else if (fade) {
    if (!includeGroup1) fillGroup(0, red, green, blue, brightness);
    for (float t = 0; t <= 1.0; t += 0.05) {
      strip.clear();
      if (!includeGroup1) fillGroup(0, red, green, blue, brightness);
      fillGroup(groupIndex, red * t, green * t, blue * t, brightness * t);
      strip.show();
      delay(15);
    }
    for (float t = 1.0; t >= 0.0; t -= 0.05) {
      strip.clear();
      if (!includeGroup1) fillGroup(0, red, green, blue, brightness);
      fillGroup(groupIndex, red * t, green * t, blue * t, brightness * t);
      strip.show();
      delay(15);
    }
  }
  else {  // Blink
    strip.clear();
    if (!includeGroup1) fillGroup(0, red, green, blue, brightness);
    fillGroup(groupIndex, red, green, blue, brightness);
    strip.show();
    delay(speed);
  }

  animationStep++;
  if (animationStep > (endIndex - startIndex)) {
    animationStep = 0;
    if (isRandomStep) shuffleOrder(startIndex, endIndex);
  }
}

void fillGroup(uint8_t groupIndex, float r, float g, float b, float brightness) {
  r *= brightness;
  g *= brightness;
  b *= brightness;
  for (int i = groupStarts[groupIndex]; i <= groupEnds[groupIndex]; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}
