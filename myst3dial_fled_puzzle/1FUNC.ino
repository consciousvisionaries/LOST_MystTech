


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

void funcRotaryDialPuzzle() {
  
    if (gameOnFlag || restartFlag) {
      
        if (restartFlag && !gameOnFlag) {
            generateFUNCRandomSolution();
            gameOnFlag = true;
            restartFlag = false;
            gameOverFlag = false;
            
            // Reset last LED counts
            for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
                lastLedCount[i] = -1;
                ledCount[i] = -1;
            }

            send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);

            Serial.println("Game is set!");
        }

        int maxLEDCount = (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS);

        // Simulate fetching pulse counts
        for (int c = 0; c < NUM_FLED_CHANNELS; c++) {
          ledCount[c] = getCount(maxLEDCount, c);
        }
        
        // Check if the solution is found
        solutionFound = true;
        for (int s = 0; s < NUM_FLED_CHANNELS; s++) {
            if (ledCount[s] != solutionWin[s]) {
                solutionFound = false;
                break;
            }
        }

        checkForWin();

        // Update LED states only if there's a change
        bool ledChanged = false;
        for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
            if (ledCount[i] != lastLedCount[i]) {
                ledChanged = true;
                lastLedCount[i] = ledCount[i];
            }
        }

        if (ledChanged) {
            updateFLED_clearAll();

            // Update solution LEDs
            for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
                updateFLED_address(solutionWin[i] + (i * maxLEDCount), "orange");
            }
            
                     
            // Update LEDs for each dial dynamically based on NUM_FLED_CHANNELS
            for (int c = 0; c < NUM_FLED_CHANNELS; c++) {
                // Calculate the start and end LED indices for the current channel
                int startIdx = c * maxLEDCount;
                int endIdx = startIdx + ledCount[c];
            
                // Update LEDs for the current channel
                for (int i = startIdx; i < endIdx; i++) {
                    updateFLED_address(i, globalSettings.colorsFLEDChannels[c]);  // Use the color for the current channel
                }
            }
            
              

            updateFLED_show();

            Serial.println("Change in last dial detected! MQTT message to follow....");

            send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);

            // Debug output
            for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
                Serial.println("LED Strip " + String(i+1) + ": " + String(ledCount[i]));
            }
        }
    } else if (!restartFlag && !gameOverFlag) {
        gameOverFlag = true;

        // Set final LED counts to solution
        for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
            ledCount[i] = solutionWin[i];
        }

        send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);
        Serial.println("Game is over!");
    } else {
        // No action
    }
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
