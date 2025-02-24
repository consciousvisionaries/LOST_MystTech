int getCount(int maxcount, int analoginputpin) {
  // Ensure pulseCount is within the valid range before using it
  int clampedPulseCount = constrain(pulseCount[analoginputpin], 0, PULSE_MAX_RANGE[analoginputpin]);

  pulseCount[analoginputpin] = clampedPulseCount;

  // Return the mapped value to the number of LEDs based on the constrained pulse count
  return map(clampedPulseCount, 0, PULSE_MAX_RANGE[analoginputpin], 0, maxcount);
}




void funcRotaryDialPuzzle() {

  int maxLEDCount = (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS);


  if (gameOnFlag || restartFlag) {

    if (restartFlag && !gameOnFlag) {
      gameOnFlag = true;
      restartFlag = false;
      gameOverFlag = false;
      solutionFound = false;
      solutionStable = false;

      // Reset last LED counts
      for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
        lastLedCount[i] = -1;
        ledCount[i] = -1;
        pulseCount[i] = -1;

      }

      send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);
      sendMessageMQTTPayload("Game is set!", "Status");
      Serial.println("Game is set!");
    }


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
        updateFLED_address(-solutionWin[i] + ((i + 1) * maxLEDCount) - 1, "orange");
      }

      // Update LEDs for each dial dynamically based on NUM_FLED_CHANNELS
      for (int c = 0; c < NUM_FLED_CHANNELS; c++) {
        // Calculate the start and end LED indices for the current channel
        int startIdx = ((c + 1) * maxLEDCount) - 1;
        int endIdx = startIdx - ledCount[c];
        int warningIdx = startIdx + 10;
        // Update LEDs for the current channel
        for (int i = startIdx; i > endIdx; i--) {
          if (i < warningIdx) {
                      updateFLED_address(i, "white");  // Use the color for the current channel

          } else {
          updateFLED_address(i, globalSettings.colorsFLEDChannels[c]);  // Use the color for the current channel
          }
        }
      }

      updateFLED_show();

      Serial.println("Change in last dial detected! MQTT message to follow....");
      send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);
      sendMessageMQTTPayload("Change in last dial detected! MQTT message to follow....", "Status");
      // Debug output
      for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
        Serial.println("LED Strip " + String(i + 1) + ": " + String(ledCount[i]));
      }
    }
  } else if (!restartFlag && !gameOverFlag) {
    gameOverFlag = true;

    // Set final LED counts to solution
    for (int i = 0; i < NUM_FLED_CHANNELS; i++) {
      ledCount[i] = solutionWin[i];
    }

    // Update LEDs for each dial dynamically based on NUM_FLED_CHANNELS
    for (int c = 0; c < NUM_FLED_CHANNELS; c++) {
      // Calculate the start and end LED indices for the current channel
      int startIdx = (c + 1) * maxLEDCount - 1;
      int endIdx = startIdx - ledCount[c];
      int warningIdx = startIdx + 10;
      // Update LEDs for the current channel
      for (int i = startIdx; i > endIdx; i--) {

        updateFLED_address(i, globalSettings.colorsFLEDChannels[c]);  // Use the color for the current channel

      }
    }

    send3D_ROTARY_PULSEMQTTData(ledCount[0], ledCount[1], ledCount[2]);
    Serial.println("Game is over!");
  } else {
    // No action
  }
}
