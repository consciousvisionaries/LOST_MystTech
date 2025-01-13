#define functionFile "0V15x"
#define functionVersion "V1.5.x"

void funcRotaryDialPuzzle() {

  if (gameOnFlag || restartFlag) {

    int maxLEDCount = (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS);

    if (restartFlag && !gameOnFlag) {
      
      generateFUNCRandomSolution(NUM_FLED_CHANNELS, maxLEDCount);
      gameOnFlag = true;
      restartFlag = false;
      gameOverFlag = false;

      resetFLEDCounts();

      printSerialln("Game is set!",0);
    }

    // Simulate fetching pulse counts
    for (int c = 0; c < NUM_FLED_CHANNELS; c++) {
      FLEDCount[c] = getFLEDCount_pulseGPIO(maxLEDCount, c, PULSE_MAX_RANGE);
    }

    // Check if the solution is found
    solutionFound = true;
    for (int s = 0; s < NUM_FLED_CHANNELS; s++) {
      if (FLEDCount[s] != solutionWin[s]) {
        solutionFound = false;
        break;
      }
    }

    checkForWin();

    // Update LED states only if there's a change
    bool ledChanged = false;
    for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
      if (FLEDCount[i] != lastFLEDCount[i]) {
        ledChanged = true;
        lastFLEDCount[i] = FLEDCount[i];
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
        int endIdx = startIdx + FLEDCount[c];

        // Update LEDs for the current channel
        for (int i = startIdx; i < endIdx; i++) {
          updateFLED_address(i, globalSettings.colorsFLEDChannels[c]);  // Use the color for the current channel
        }
      }



      updateFLED_show();

      printSerialln("Change in last dial detected! MQTT message to follow....",0);

      //send3D_ROTARY_PULSEMQTTData(FLEDCount[0], FLEDCount[1], FLEDCount[2]);

      // Debug output
      for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
        printSerialln("LED Strip " + String(i + 1) + ": " + String(FLEDCount[i]),0);
      }
    }
  } else if (!restartFlag && !gameOverFlag) {
    gameOverFlag = true;

    // Set final LED counts to solution
    for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
      FLEDCount[i] = solutionWin[i];
    }

    //send3D_ROTARY_PULSEMQTTData(FLEDCount[0], FLEDCount[1], FLEDCount[2]);
    printSerialln("Game is over!",0);
  } else {
    // No action
  }
}
