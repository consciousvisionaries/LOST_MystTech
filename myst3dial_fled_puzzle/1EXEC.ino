
void checkVersionWinScenario() {

  
  if (solutionFound && solutionStable) {
    
    sendMessageMQTTPayload("Activate relay 2 in Stage 1 (3 Dials MP3 Puzzle)", "Stage 1 Button 3 Pressed");

    twinkle(500, 100);

    for (int i = 0; i < 3; i++) {
      update_pulseGPIOCount(i, 0);
    }
    
    gameOnFlag = false;
    restartFlag = false;
    solutionStable = false;
  }
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

// Batch operation functions
void executeFUNCBatchButton1_PIN(int pin) {
    Serial.println("Solution stable for 5 seconds. Executing GPIO1 batch 1 operation...");
    digitalWrite(pin, LOW);
    
     twinkle(50, 100);

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
