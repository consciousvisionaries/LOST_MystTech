#define functionFile "0V16x"
#define functionVersion "V1.6.x"

void checkVersionWinScenario() {

  
  if (solutionFound && solutionStable) {
    
    sendMessageMQTTPayload("Activate relay 2 in Stage 1 (3 Dials MP3 Puzzle)", "Stage 1 Button 3 Pressed");

    delay(50);

    for (int i = 0; i < 3; i++) {
      update_pulseCount(i, 0);
    }
    
    gameOnFlag = false;
    restartFlag = false;
    solutionStable = false;
    solutionFound = false;
  }
}

void funcMatrixMP3Puzzle() {
}
