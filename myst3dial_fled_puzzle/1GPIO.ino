


portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void update_pulseGPIOCount(int p, int v) {
  pulseGPIOCount[p] = v;
}


// Method to get the current state of a digital input pin
bool getDigitalInputStateGPIO(int inputPin) {
  int state = digitalRead(inputPin);
  return (state == HIGH) ? HIGH : LOW;  // Return "HIGH" or "LOW"
}

// Method to toggle the output pin state
void toggleOutputStateGPIOA(int outputNumber) {
  if (outputNumber >= 0 && outputNumber < NUM_DIGITAL_OUTPUTSA) {
    int pin = outputPinsA[outputNumber];
    bool currentState = digitalRead(pin);  // Read the current state before toggling
    bool newState = !currentState;         // Toggle state
    digitalWrite(pin, newState);          // Set the new state

    printSerial("Output ");
    printSerial(String(outputNumber));
    printSerial(": Toggled to ");
    printSerialln(newState ? "HIGH" : "LOW", 0);
    sendMessageMQTTPayload("Toggled Output", "UpdateUI");
  } else {
    printSerialln("Error: Invalid output number.", 0);
  }
}

// Method to toggle the output pin state
void toggleOutputStateGPIOB(int outputNumber) {
  if (outputNumber >= 0 && outputNumber < NUM_DIGITAL_OUTPUTSB) {
    int pin = outputPinsB[outputNumber];
    bool currentState = digitalRead(pin);  // Read the current state before toggling
    bool newState = !currentState;         // Toggle state
    digitalWrite(pin, newState);          // Set the new state

    printSerial("Output ");
    printSerial(String(outputNumber));
    printSerial(": Toggled to ");
    printSerialln(newState ? "HIGH" : "LOW", 0);
  } else {
    printSerialln("Error: Invalid output number.", 0);
  }
}

// Method to handle digital input changes
void handleDigitalInputChangeA(int pinIndex) {
  // Check for debounce and pin state change
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTimeA[pinIndex] > debounceDelay) {
    lastDebounceTimeA[pinIndex] = currentTime; // Update debounce time
    int currentState = digitalRead(inputDigitalPinsA[pinIndex]);

    if (currentState != lastInputDigitalPinStateA[pinIndex]) {
      lastInputDigitalPinStateA[pinIndex] = currentState;  // Update the last known state

      // Trigger action only if the pin goes LOW (e.g., a button press)
      if (currentState == LOW) {
        if (pinIndex == 0) {
          //executeFUNCBatchGPIOPin1();
        } else if (pinIndex == 1) {
          //executeFUNCBatchGPIOPin2();
        } else if (pinIndex == 2) {
          //executeFUNCBatchGPIOPin3();
        }
      }
    }
  }
}

// Method to handle digital input changes
void handleDigitalInputChangeB(int pinIndex) {
  // Check for debounce and pin state change
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTimeB[pinIndex] > debounceDelay) {
    lastDebounceTimeB[pinIndex] = currentTime; // Update debounce time
    int currentState = digitalRead(inputDigitalPinsB[pinIndex]);

    if (currentState != lastInputDigitalPinStateB[pinIndex]) {
      lastInputDigitalPinStateB[pinIndex] = currentState;  // Update the last known state

      // Trigger action only if the pin goes LOW (e.g., a button press)
      if (currentState == LOW) {
        if (pinIndex == 0) {
          //executeFUNCBatchGPIOPin1();
        } else if (pinIndex == 1) {
          //executeFUNCBatchGPIOPin2();
        } else if (pinIndex == 2) {
          //executeFUNCBatchGPIOPin3();
        }
      }
    }
  }
}


// Method to handle analog input state changes
void handleAnalogInputPairsChange(int pinIndex) {
  int stateA = digitalRead(analogInputPinsA[pinIndex]);
  int stateB = digitalRead(analogInputPinsB[pinIndex]);

  portENTER_CRITICAL(&mux);
  if (stateA != lastStateAnalogInputs[pinIndex]) {
    pulseGPIOCount[pinIndex] += (stateA != stateB) ? 1 : -1;
    pulseGPIOUpdated[pinIndex] = true;
  }
  lastStateAnalogInputs[pinIndex] = stateA;
  portEXIT_CRITICAL(&mux);
}

// Method to set up GPIO pins (outputs, inputs, analog)
void setupGPIO() {
  Serial.begin(115200);

  // Array to track used pins
  bool usedPins[50] = {false}; // Assuming a maximum of 50 GPIO pins on the microcontroller

  // Lambda function to check and track pin usage
  auto usePin = [&](int pin) {
    if (usedPins[pin]) {
      printSerial("Error: Pin conflict detected on pin ");
      printSerialln(String(pin), 60000);
      return;  // Exit if a conflict is found
    }
    usedPins[pin] = true;
    printSerial("Pin ");
    printSerial(String(pin));
    printSerial(" is available => ");
  };

  // Initialize output pins (A pins)
  for (int i = 0; i < NUM_DIGITAL_OUTPUTSA; i++) {
    if (outputPinsA[i] == GPIO17_U2TXD) {
      printSerial(" => Setting up GPIO17_U2TXD as an output...");
      Serial2.end();  // Disable UART2
      delay(10);  // Allow time for UART2 to fully disable

      pinMode(outputPinsA[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsA[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO17_U2TXD disabled and set as digital output.", 0);

    } else if (outputPinsA[i] == GPIO16_U2RXD_WS2812_16) {
      printSerial(" => Setting up GPIO16_U2RXD_WS2812_16 as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsA[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsA[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO16_U2RXD_WS2812_16 disabled and set as digital output.", 0);

    } else if (outputPinsA[i] == GPIO01_U0TXD_LED_TX) {
      printSerial(" => Setting up GPIO01_U0TXD_LED_TX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsA[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsA[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO01_U0TXD_LED_TX disabled and set as digital output.", 0);

    } else if (outputPinsA[i] == GPIO03_U0RXD_LED_RX) {
      printSerial(" => Setting up GPIO03_U0RXD_LED_RX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsA[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsA[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO03_U0RXD_LED_RX disabled and set as digital output.", 0);
    }

    usePin(outputPinsA[i]);  // Check for conflicts
    pinMode(outputPinsA[i], OUTPUT);
    delay(10);  // Allow time for pinMode to take effect
    digitalWrite(outputPinsA[i], outputPins_initStateA[i] ? HIGH : LOW); // Initialize with state array
    delay(10);  // Small delay after setting the output state
    printSerial("Output pin ");
    printSerial(String(outputPinsA[i]));
    printSerial(" set to ");
    printSerialln(outputPins_initStateA[i] ? "HIGH" : "LOW", 0);

  }
  printSerialln("<end> ." + String(NUM_DIGITAL_OUTPUTSA) + " A-Outputs Initialized.", 0);
  delay(1000);


  // Initialize output pins (B pins)
  for (int i = 0; i < NUM_DIGITAL_OUTPUTSB; i++) {
    if (outputPinsB[i] == GPIO17_U2TXD) {
      printSerial(" => Setting up GPIO17_U2TXD as an output...");
      Serial2.end();  // Disable UART2
      delay(10);  // Allow time for UART2 to fully disable

      pinMode(outputPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO17_U2TXD disabled and set as digital output.", 0);

    } else if (outputPinsB[i] == GPIO16_U2RXD_WS2812_16) {
      printSerial(" => Setting up GPIO16_U2RXD_WS2812_16 as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO16_U2RXD_WS2812_16 disabled and set as digital output.", 0);

    } else if (outputPinsB[i] == GPIO01_U0TXD_LED_TX) {
      printSerial(" => Setting up GPIO01_U0TXD_LED_TX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO01_U0TXD_LED_TX disabled and set as digital output.", 0);

    } else if (outputPinsB[i] == GPIO03_U0RXD_LED_RX) {
      printSerial(" => Setting up GPIO03_U0RXD_LED_RX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(outputPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(outputPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO03_U0RXD_LED_RX disabled and set as digital output.", 0);
    }
    usePin(outputPinsB[i]); // Check for conflicts
    pinMode(outputPinsB[i], OUTPUT);
    delay(10);  // Allow time for pinMode to take effect
    digitalWrite(outputPinsB[i], outputPins_initStateB[i] ? HIGH : LOW); // Initialize with state array
    delay(10);  // Small delay after setting the output state
    printSerial("Output pin ");
    printSerial(String(outputPinsB[i]));
    printSerial(" set to ");
    printSerialln(outputPins_initStateB[i] ? "HIGH" : "LOW", 0);
  }

  printSerialln("<end> ." + String(NUM_DIGITAL_OUTPUTSB) + " B-Outputs Initialized.", 0);
  delay(1000);


  // Initialize input pins (A pins)
  for (int i = 0; i < NUM_DIGITAL_INPUTSA; i++) {
    if (outputPinsA[i] == GPIO17_U2TXD) {
      printSerial(" => Setting up GPIO17_U2TXD as an output...");
      Serial2.end();  // Disable UART2
      delay(10);  // Allow time for UART2 to fully disable

      pinMode(inputDigitalPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(inputDigitalPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO17_U2TXD disabled and set as digital output.", 0);

    } else if (inputDigitalPinsB[i] == GPIO16_U2RXD_WS2812_16) {
      printSerial(" => Setting up GPIO16_U2RXD_WS2812_16 as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(inputDigitalPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO16_U2RXD_WS2812_16 disabled and set as digital output.", 0);

    } else if (inputDigitalPinsB[i] == GPIO01_U0TXD_LED_TX) {
      printSerial(" => Setting up GPIO01_U0TXD_LED_TX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(inputDigitalPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO01_U0TXD_LED_TX disabled and set as digital output.", 0);

    } else if (inputDigitalPinsB[i] == GPIO03_U0RXD_LED_RX) {
      printSerial(" => Setting up GPIO03_U0RXD_LED_RX as an output...");
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], OUTPUT); // Set pin as OUTPUT
      delay(10);  // Small delay to ensure pinMode change is stable
      digitalWrite(inputDigitalPinsB[i], LOW); // Initialize to LOW
      delay(10);  // Small delay to stabilize the LOW state

      printSerialln("GPIO03_U0RXD_LED_RX disabled and set as digital output.", 0);
    }
    usePin(inputDigitalPinsA[i]); // Check for conflicts
    pinMode(inputDigitalPinsA[i], INPUT_PULLUP); // General configuration for other pins
    delay(10);  // Small delay after setting pin mode
    printSerial("Input pin ");
    printSerial(String(inputDigitalPinsA[i]));
    printSerialln(" set to INPUT_PULLUP.", 0);

  }
  printSerialln("<end> ." + String(NUM_DIGITAL_INPUTSA) + " A-Inputs Initialized.", 1000);

  // Initialize input pins (B pins)
  for (int i = 0; i < NUM_DIGITAL_INPUTSB; i++) {
    if (inputDigitalPinsB[i] == GPIO16_U2RXD_WS2812_16) {
      Serial2.end();  // Disable UART2
      delay(10);  // Allow time for UART2 to fully disable

      pinMode(inputDigitalPinsB[i], INPUT_PULLUP); // Set RX pin to INPUT_PULLUP
      delay(10);  // Small delay after setting pin mode
      printSerialln("GPIO16_U2TXD disabled and set as digital input.", 0);

    } else if (inputDigitalPinsB[i] == GPIO17_U2TXD) {
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], INPUT_PULLUP); // Set RX pin to INPUT_PULLUP
      delay(10);  // Small delay after setting pin mode
      printSerialln("RX_UART1 disabled and set as digital input.", 0);

    }  else if (inputDigitalPinsB[i] == GPIO03_U0RXD_LED_RX) {
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], INPUT_PULLUP); // Set RX pin to INPUT_PULLUP
      delay(10);  // Small delay after setting pin mode
      printSerialln("RX_UART1 disabled and set as digital input.", 0);

    } else if (inputDigitalPinsB[i] == GPIO01_U0TXD_LED_TX) {
      Serial1.end();  // Disable UART1
      delay(10);  // Allow time for UART1 to fully disable

      pinMode(inputDigitalPinsB[i], INPUT_PULLUP); // Set RX pin to INPUT_PULLUP
      delay(10);  // Small delay after setting pin mode
      printSerialln("RX_UART1 disabled and set as digital input.", 0);
    }
    usePin(inputDigitalPinsB[i]); // Check for conflicts
    pinMode(inputDigitalPinsB[i], INPUT_PULLUP); // General configuration for other pins
    delay(10);  // Small delay after setting pin mode
    printSerial("Input pin ");
    printSerial(String(inputDigitalPinsB[i]));
    printSerialln(" set to INPUT_PULLUP.", 0);

  }
  printSerialln("<end> ." + String(NUM_DIGITAL_INPUTSB) + " B-Inputs Initialized.", 0);
  delay(1000);

  // Initialize analog input pairs (A and B)
  for (int i = 0; i < NUM_ANALOG_INPUTPAIRS; i++) {
    printSerial("Initializing analog input pair: ");
    printSerial(String(i));
    printSerialln("; ", 0);

    usePin(analogInputPinsA[i]); // Check for conflicts
    usePin(analogInputPinsB[i]); // Check for conflicts

    pinMode(analogInputPinsA[i], INPUT);  // Configure analog pin A as INPUT
    delay(10);  // Small delay after setting pin mode
    pinMode(analogInputPinsB[i], INPUT);  // Configure analog pin B as INPUT
    delay(10);  // Small delay after setting pin mode
    printSerial("Analog input pair ");
    printSerial(String(i));
    printSerialln(" initialized.", 0);
  }
  printSerialln("<end> ." + String(NUM_ANALOG_INPUTPAIRS) + " Analog Input Pairs Initialized!", 0);

  // Initialize FLED addressable outputs (if applicable)
  for (int i = 0; i < NUM_FLED_OUTPUTS; i++) {
    usePin(outputFLEDPins[i]);
    printSerialln("Initializing FLED addressable output at Pin " + String(outputFLEDPins[i]), 0);

  }
  printSerialln("<end> ." + String(NUM_FLED_OUTPUTS) + " FLED Outputs Initialized!", 0);

  // Initialize RXTX addressable outputs (if applicable)
  for (int i = 0; i < NUM_RXTX_PORTS; i++) {
    for (int t = 0; t < 2; t++) {
      usePin(RXTX_Pins[i][t]);
      printSerialln("Initializing RXTX addressable I/O at Pin " + String(RXTX_Pins[i][t]), 0);
    }
  }
  printSerialln("<end> ." + String(NUM_RXTX_PORTS) + " RXTX Ports Initialized!", 0);
}




// Main loop to handle GPIO activities
void loopGPIO() {

  // Poll and handle digital inputs (instead of relying on interrupts)
  for (int i = 0; i < NUM_DIGITAL_INPUTSA; i++) {
    handleDigitalInputChangeA(i);
  }

  // Poll and handle digital inputs (instead of relying on interrupts)
  for (int i = 0; i < NUM_DIGITAL_INPUTSB; i++) {
    handleDigitalInputChangeB(i);
  }


  // Poll and handle analog input changes
  for (int i = 0; i < NUM_ANALOG_INPUTPAIRS; i++) {
    handleAnalogInputPairsChange(i);
  }

  // Process pulse counts and updates for analog inputs
  for (int i = 0; i < NUM_ANALOG_INPUTPAIRS; i++) {
    portENTER_CRITICAL(&mux);
    int count = pulseGPIOCount[i];
    bool updated = pulseGPIOUpdated[i];
    pulseGPIOUpdated[i] = false;
    portEXIT_CRITICAL(&mux);

    count = constrain(count, 0, (NUM_FLED_ADDLEDS / NUM_FLED_CHANNELS));

    if (updated) {
      printSerial("Dial ");
      printSerial(String(i + 1));
      printSerial(" Count: ");
      printSerialln(String(count), 0);
    }
  }
}
