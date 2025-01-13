

void generateFUNCRandomSolution(int numbersolutions, int rangemax) {

  // Seed the random number generator
  randomSeed(analogRead(0));
  printSerial("Solution: ");

  // Create an array to track which numbers have already been used
  bool usedNumbers[numbersolutions] = {false};

  // Populate the solution array with random values between 0 and (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS) without repetition
  for (int i = 0; i < numbersolutions; i++) {
    int randNum;
    do {
      randNum = random(0, rangemax);
    } while (usedNumbers[randNum]); // Keep generating a new random number until we find one that hasn't been used

    solutionWin[i] = randNum;
    usedNumbers[randNum] = true;  // Mark this number as used

    printSerial(String(solutionWin[i]));
    printSerial(", ");
  }

  printSerialln("<end>", 0);
}

void checkForWin() {

  if (solutionFound) {
    if (!solutionStable) {
      solutionCheckStart = millis();
    }
    if (millis() - solutionCheckStart >= 2000) {
      Serial.println("");
      printSerialln("Solution Found!", 0);
      solutionStable = true;
    }
  } else {
    solutionStable = false;
  }
}

// Batch operation functions
void executeFUNCBatchButton1_PIN(int outputPin) {

  printSerialln("Solution detected!",0);
      solutionFound = true;
      solutionStable = true;

  for (int i = 0; i < 3; i++) {
    update_pulseCount(i, 0);
  }
  
  gameOnFlag = false;
  restartFlag = true;

  if (outputPin != -1) {
    printSerialln("Executing GPIO1 batch 1 operation...", 0);
    digitalWrite(outputPin, LOW);
    delayESPTask(5000);

    digitalWrite(outputPin, HIGH);
    printSerialln("Batch 1 completed... held for 5 secs.", 0);
  }
}

void executeFUNCBatchButton2() {
  printSerial("Batch 2 requested. Executing game start batch 2 operation...");

  gameOnFlag = false;
  restartFlag = true;


 
  printSerialln("Batch 2 completed... restarted game!", 0);
}

void executeFUNCBatchButton3_PIN(int outputPin) {
  printSerial("Batch 3 requested. Executing auxillary batch 3 operation...");

  if (outputPin != -1) {
    digitalWrite(outputPin, LOW);
    delayESPTask(5000);
    digitalWrite(outputPin, HIGH);
  }
  printSerialln("Batch 3 completed... held for 5 secs.", 0);
}
