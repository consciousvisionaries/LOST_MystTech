void exec_4x4matrixMP3_puzzle() {


    if (gameOnFlag || restartFlag) {
      
        if (restartFlag && !gameOnFlag) {
            generateFUNCRandomSolution(NUM_OF_SOLUTIONS, 16);
            gameOnFlag = true;
            restartFlag = false;
            gameOverFlag = false;
            
            resetFLED();
            //resetMATRIXIO();
    

            Serial.println("Game is set!");
        }

        // check buttons against solution

        
        

        
        // Check if the solution is found
        // if so, solutionFound = true;
        

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
