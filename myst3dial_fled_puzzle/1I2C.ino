#include <Adafruit_PWMServoDriver.h>;

Adafruit_PWMServoDriver ledArray = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver ledButtonArray = Adafruit_PWMServoDriver(0x40);


void setupI2C() {
  
  initializeI2CCom();

  setupPVM();
}

void loopI2C() {

  loopPVM();
}



void setupPVM() {

  printSerialln("Begin PVM...",0);
  
// Initialize the PCA9685 with default MODE1 and MODE2 settings
  if (!ledArray.begin()) {
    Serial.println("Error initializing LED Display PCA9685. Check your wiring.");
    scannerLoop();
    while (1);  // Halt execution if initialization fails
  } else { 
    printSerialln("Address: 0x40 now occupied",0);
  }
  delay(1000);
// Initialize the PCA9685 with default MODE1 and MODE2 settings
  if (!ledButtonArray.begin()) {
    Serial.println("Error initializing LED Button PCA9685. Check your wiring.");
    scannerLoop();
    //while (1);  // Halt execution if initialization fails
  }  else { 
    printSerialln("Address: 0x70 now occupied",0);
  }
  // Optionally configure mode settings (e.g., enable autoincrement and all-call address)
  // ledArray.configure(PCA9685_MODE1_AUTOINCR | PCA9685_MODE1_ALLCALL, PCA9685_MODE2_TOTEMPOLE);

  // Set the PWM frequency to 1000 Hz (example)
  ledArray.setPWMFreq(1000);
  ledButtonArray.setPWMFreq(1000);
  Serial.println("PCA9685 initialization complete.");
}

  
// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

void scannerLoop() {

  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);  // Wait 5 seconds before scanning again
}

unsigned long previousMillis = 0;  // will store last time the LED was updated
unsigned long ledDelay = 200;      // delay time for LED pattern
int currentLED = 0;

void loopPVM() {
  
    
  unsigned long currentMillis = millis();  // get the current time

  // Check if it's time to update the LED state
  if (currentMillis - previousMillis >= ledDelay) {
    previousMillis = currentMillis;  // save the last time LED was updated

    // Turn on the current LED at half brightness
    ledArray.setPWM(currentLED, 0, 4095);  // 2048 is 50% brightness
    ledButtonArray.setPWM(currentLED, 0, 4095);  // 2048 is 50% brightness

    // Turn off the previous LED (this creates the "animation" effect)
    if (currentLED > 0) {
      ledArray.setPWM(currentLED - 1, 0, 0);  // Turn off the previous LED
      ledButtonArray.setPWM(currentLED - 1, 0, 0);  // Turn off the previous LED

    } else {
      ledArray.setPWM(15, 0, 0);  // Turn off the last LED when cycling back to the start
      ledButtonArray.setPWM(15, 0, 0);  // Turn off the last LED when cycling back to the start

    }

    // Move to the next LED
    currentLED = (currentLED + 1) % 16;  // Cycle through all 16 LEDs

  }
}
