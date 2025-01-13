#include <FastLED.h>

#define MYST2FLED_VERSION V1.2

CRGB leds[NUM_FLED_ADDLEDS + NUM_FLED_ADDLEDS2];

// Function to map pulse counts to the number of LEDs
int getFLEDCount_pulseGPIO(int maxcount, int analoginput, int analogmaxrange) {
    // Ensure pulseCount is within the valid range before using it
    int totalGPIORange = (analogmaxrange * (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS));
   int clampedPulseGPIOCount = constrain(pulseGPIOCount[analoginput], 0, totalGPIORange);

    pulseGPIOCount[analoginput] = clampedPulseGPIOCount;

    // Return the mapped value to the number of LEDs based on the constrained pulse count
    return map(clampedPulseGPIOCount, 0, totalGPIORange, 0, maxcount);
}

void setupFASTLED_GPIO() {
  
    if (NUM_FLED_OUTPUTS >= 1) {
        FastLED.addLeds<WS2812, GPIO12_ADC2_CH5_HSPI_MOS0_TOUCH5_MTDI, RGB>(leds, NUM_FLED_ADDLEDS);
        FastLED.clear();
        FastLED.show();
        printSerialln("<end> FAST LED Initialized!",500);
    }
    if (NUM_FLED_OUTPUTS >=2) {
        FastLED.addLeds<WS2812, GPIO13_ADC2_CH4_HSPI_MOS1_TOUCH4, RGB>(leds, NUM_FLED_ADDLEDS2);
        FastLED.clear();
        FastLED.show();
        printSerialln("<end> FAST LED 2 Initialized!",500);
    }
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
  }
}

void resetFLEDCounts() {
  for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
                lastFLEDCount[i] = -1;
                FLEDCount[i] = -1;
            }

       //send3D_ROTARY_PULSEMQTTData(FLEDCount[0], FLEDCount[1], FLEDCount[2]);
}

void updateFLED_clearAll() {
  fill_solid(leds, NUM_FLED_ADDLEDS, CRGB::Black);
}

void updateFLED_show() {
  FastLED.show();
}

void loopFASTLED() {
  
}
