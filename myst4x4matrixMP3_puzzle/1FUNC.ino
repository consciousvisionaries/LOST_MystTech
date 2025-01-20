


void checkVersionWinScenario() {

  
  if (solutionFound && solutionStable) {
    
    sendMessageMQTTPayload("Activate relay 2 in Stage 1 (3 Dials MP3 Puzzle)", "Stage 1 Button 3 Pressed");

    delay(50);

    for (int i = 0; i < 3; i++) {
      update_pulseGPIOCount(i, 0);
    }
    
    gameOnFlag = false;
    restartFlag = false;
    solutionStable = false;
    solutionFound = false;
  }
}

void generateFUNCRandomSolution() {
    // Seed the random number generator
    randomSeed(analogRead(0));
    Serial.print("Solution: ");
    // Populate the solution array with random values between 0 and 9
    for (int i = 0; i < NUM_OF_SOLUTIONS; i++) {
        solutionWin[i] = random(5, (NUM_FLED_ADDLEDS / NUM_OF_SOLUTIONS)); 
        Serial.print(solutionWin[i]);
        Serial.print(", ");
    }
    Serial.println("<end>");
}

void checkForWin() {
    if (solutionFound) {
        if (!solutionStable) {
            solutionCheckStart = millis();
            solutionStable = true;
        }

        if (millis() - solutionCheckStart >= 2000) {
            executeFUNCBatchButton1_PIN(EXEC_BATCH1_PIN);
            solutionStable = false;
        }
    } else {
        solutionStable = false;
    }
}

// Function to map pulse counts to the number of LEDs
int getCount(int maxcount, int analoginput) {
    // Ensure pulseGPIOCount is within the valid range before using it
    int TOT_RANGE = (PULSE_MAX_RANGE * (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS));
    int clampedpulseGPIOCount = constrain(pulseGPIOCount[analoginput], 0, TOT_RANGE);

    pulseGPIOCount[analoginput] = clampedpulseGPIOCount;

    // Return the mapped value to the number of LEDs based on the constrained pulse count
    return map(clampedpulseGPIOCount, 0, TOT_RANGE, 0, maxcount);
}





// Batch operation functions
void executeFUNCBatchButton1_PIN(int pin) {
    Serial.println("Solution stable for 5 seconds. Executing GPIO1 batch 1 operation...");
    digitalWrite(pin, LOW);
    delayESPTask(5000);

    for (int i = 0; i < 3; i++) {
        pulseGPIOCount[i] = 0;
    }
    gameOnFlag = false;
    restartFlag = false;

    digitalWrite(pin, HIGH);
    Serial.println("Batch 1 completed... Relay 1 trigger; forced 'Game Win'");
}

void executeFUNCBatchButton2() {
    Serial.print("Batch 2 requested. Executing game start batch 2 operation...");
    delayESPTask(5000);
    gameOnFlag = false;
    restartFlag = true;
    Serial.println("Batch 2 completed... restarted game!");
}

void executeFUNCBatchButton3_PIN(int pin) {
    Serial.print("Batch 3 requested. Executing GPIO2 batch 3 operation...");
    digitalWrite(pin, LOW);
    delayESPTask(5000);


    digitalWrite(pin, HIGH);
    Serial.println("Batch 3 completed... Relay two trigger; held for 5 secs.");
}

void executeFUNCBatchMQTT(String activity) {
    Serial.println("Key 'activity': " + activity);
}
