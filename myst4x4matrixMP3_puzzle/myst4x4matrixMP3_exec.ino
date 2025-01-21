
void exec_4x4matrixMP3_puzzle() {


  if (gameOnFlag || restartFlag) {

    if (restartFlag && !gameOnFlag) {
      generateFUNCRandomSolution(NUM_OF_SOLUTIONS, (NUM_DIGITAL_IOMATRIXPAIRS*NUM_DIGITAL_IOMATRIXPAIRS));
      gameOnFlag = true;
      restartFlag = false;
      gameOverFlag = false;

      resetFLED();
      resetMATRIXIO();

      Serial.println("Game is set!");
    }

    // check buttons against solution
    
      if (solutionWin[solutionSeq] == lastMatrixButtonPressed && lastMatrixButtonPressed > 0) {
        solutionSeq++;
        printSerial("Solution position: ");
        printSerialln(String(solutionSeq), 0);
        playMP3(lastMatrixButtonPressed);
        lastMatrixButtonPressed = 0;

        if (solutionSeq == NUM_OF_SOLUTIONS) {
          solutionFound = true;
          playMP3(MP3_WIN_FILENO);
        }

      } else if (lastMatrixButtonPressed > 0 ) {
        
        printSerialln("Wrong button pressed! Restart sequence.",0);
        solutionSeq = 0;
        lastMatrixButtonPressed = 0;
        playMP3(MP3_FAIL_FILENO);
      } 

    checkForWin();

    // Update LED states only if there's a change

  } else if (!restartFlag && !gameOverFlag) {
    gameOverFlag = true;

    // Set final LED counts to solution

    Serial.println("Game is over!");
  } else {

    // No action
  }
}
