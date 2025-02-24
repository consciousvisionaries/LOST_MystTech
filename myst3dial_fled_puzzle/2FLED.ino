#include <FastLED.h>

#define FASTLED_VERSION V1.1

CRGB leds[NUM_FLED_ADDLEDS + NUM_FLED_ADDLEDS2];

void setupFASTLED_GPIO() {
  
  if (NUM_FLED_OUTPUTS >= 1) {
    FastLED.addLeds<WS2812, FLED_PIN1, RGB>(leds, NUM_FLED_ADDLEDS);
    FastLED.clear();
    FastLED.show();
    Serial.println("<end> FAST LED Initialized!");
  }
  if (NUM_FLED_OUTPUTS >= 2) {
    FastLED.addLeds<WS2812, FLED_PIN2, RGB>(leds, NUM_FLED_ADDLEDS2);
    FastLED.clear();
    FastLED.show();
    Serial.println("<end> FAST LED 2 Initialized!");
  }
  rainbowCycle(50);
}

void updateFLED_address(int address, String color) {
  if (color == "red") {
    leds[address] = CRGB::Red;
  } else if (color == "blue") {
    leds[address] = CRGB::Blue;
  } else if (color == "green") {
    leds[address] = CRGB::Green;
  } else if (color == "orange") {
    leds[address] = CRGB::Orange;
  } else if (color == "yellow") {
    leds[address] = CRGB::Yellow;
  } else if (color == "purple") {
    leds[address] = CRGB::Purple;
  } else if (color == "cyan") {
    leds[address] = CRGB::Cyan;
  } else if (color == "magenta") {
    leds[address] = CRGB::Magenta;
  } else if (color == "white") {
    leds[address] = CRGB::White;
  } else if (color == "black") {
    leds[address] = CRGB::Black;
  } else if (color == "pink") {
    leds[address] = CRGB::Pink;
  } else if (color == "lime") {
    leds[address] = CRGB::Lime;
  } else if (color == "teal") {
    leds[address] = CRGB::Teal;
  } else if (color == "violet") {
    leds[address] = CRGB::Violet;
  } else if (color == "gold") {
    leds[address] = CRGB::Gold;
  } else if (color == "silver") {
    leds[address] = CRGB::Silver;
  }
}

void resetFLED() {
  for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
    lastLedCount[i] = -1;
    ledCount[i] = -1;
  }
}

void updateFLED_clearAll() {
  fill_solid(leds, NUM_FLED_ADDLEDS, CRGB::Black);
}

void updateFLED_show() {
  FastLED.show();
}

void loopFASTLED() {
}


void rainbowCycle(int wait) {
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < NUM_FLED_ADDLEDS; i++) {
      leds[i] = CHSV((i * 256 / NUM_FLED_ADDLEDS + j) & 255, 255, 255);
    }
    FastLED.show();
    delay(wait);
  }
}

void twinkle(int count, int delayMs) {
  for (int i = 0; i < count; i++) {
    int pos = random(NUM_FLED_ADDLEDS);
    leds[pos] = CHSV(random(0, 255), 255, 255);
    FastLED.show();
    delay(delayMs);
    leds[pos] = CRGB::Black;
  }
  FastLED.show();
}
