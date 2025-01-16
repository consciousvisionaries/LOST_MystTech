#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Update.h>
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include <string>


#define FIRMWARE_VERSION "V1.x"

WiFiClient espClient;
PubSubClient client(espClient);

// Declare the StaticJsonDocument
DynamicJsonDocument doc(MQTT_MAX_PACKET_SIZE);


String comment = "";
unsigned long loggedMessages = 0;
String recordLog[SIZE_LOG] = {};
String tmp = "";

bool mp3PlayerInitialized = false;

// Function to print a simple statement (appends to comment)
void printSerial(String statement) {

  comment = comment + statement;
}

// Function to print a formatted statement (like printf)
void printSerialf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  Serial.vprintf(format, args);  // Print formatted output
  va_end(args);
}

// Function to log and print a statement with a delay
void printSerialln(String statement, int msdelay) {

  // Only add "[R] " at the start if it's not already present
  if (!comment.startsWith("[R]   ")) {
    comment = "[R]   " + comment + statement;  // Prepend "[R] " only if not already present
  } else {
    comment = comment + statement;  // Just append the statement if "[R]" is already there
  }
  if (loggedMessages <= 9) {
    comment = "  " + comment;
  } else if (loggedMessages <= 99) {
    comment = " " + comment;
  }
  comment = String(loggedMessages) + comment;

  if (comment != recordLog[(loggedMessages - 1)]) {

    if (SHOW_LOG) {
      Serial.println(comment);
    }

    if (RECORD_LOG) {

      if (loggedMessages < SIZE_LOG) {
        recordLog[loggedMessages++] = comment;  // Store the log entry
      }
    }
  }
  comment = "";
  delay(msdelay);  // Delay for the specified milliseconds
}

void loadAllSettings() {
  loadNodeSettings();
  loadWiFiSettings();
  loadMQTTSettings();
  loadGlobalSettings();
  loadGlobalHyperlinks();
}

void saveAllSettings() {
  saveNodeSettings();
  saveWiFiSettings();
  saveMQTTSettings();
  saveGlobalSettings();
  saveGlobalHyperlinks();
}


void setup() {

  Serial.begin(115200);

  delay(1000);

  loadAllSettings();
  printSerialln(".credentials call completed", 1000);

  connectWiFi();
  printSerialln(".wifi call completed", 1000);

  checkForUpdates();
  printSerialln(".firmware update call completed", 1000);

  if (!SHOW_LOG && RECORD_LOG) {
    Serial.println(" => display serial log not available...");
  } else if (!RECORD_LOG) {
    Serial.println(" => serial log is not being tracked by developer. Contact +1 604-819-1324 for assistance and leave a voice message.");
  } else {
    Serial.println("");
  }

  connect_designatedMQTT();
  printSerialln(".mqtt call completed", 1000);

  setupGPIO();
  printSerialln(".gpio call completed", 1000);

  setupGPIOKeyboard();
  printSerialln(".keyboard call completed", 1000);

  setupFASTLED_GPIO();
  printSerialln(".fled call completed", 1000);

  setupMP3Player();
  printSerialln(".mp3 call completed", 1000);

  setupDashboard();
  printSerialln(".dashboard call completed", 1000);

  mystTechBMP();
  printSerialln("All rights reserved. (c) 2025 Appsations, Inc. dba. MystTech Creations", 1000);
  printSerialln("   for more info email: paul@consciousvisionaries || dial 604-819-1324.", 0);
  printSerialln("                        https://consciousvisionaries.ca", 0);

  printSerialln("             Local DNS: " + String(DNSLOCAL), 0);
  printSerialln("            IP Address: " + wifiSettings.ipaddress + "/", 0);
  printSerialln("                Broker: " + mqttSettings.mqttBroker, 0);
  printSerialln("                 Topic: " + String(mqttSettings.mqttTopic), 0);
  printSerialln("                        ", 0);
  printSerialln("      Firmware Version: " + nodeSettings.storedVersion, 0);
  printSerialln(" MystTECH DEVICE READY: " + String(MYSTTECH_MODEL) + "", 1000);
  printSerialln("", 1000);
  if (MP3_PLAYER && mp3PlayerInitialized) {
    printSerialln("   MP3 Player: Active, Initialized", 1000);
  } else if (MP3_PLAYER) {
    printSerialln("   MP3 Player: Active, Not Initialized", 1000);
  } else {
    printSerialln("   MP3 Player: Not Active", 1000);
  }

  sendMessageMQTTPayload("New Device updating MystTECH UI ...", "FirmwareStatus-->");

  printSerialln(".device added to dashboard", 1000);

  sendADDMEMessageMQTT();
  printSerialln(".addme has been requested on MQTT", 1000);

}

void loop() {


  loopFIRMWARE();
  checkForWin();
  checkVersionWinScenario();
  clientMQTTConnected();
  loopGPIO();
  loopFASTLED();
  mp3Loop();

  if (String(NR_TYPE) == "4X4_MATRIX_MP3" && NUM_DIGITAL_INPUTSB == 8) {
    funcMatrixMP3Puzzle();
  }
}

void logSerialPort(bool record) {

}


void delayESPTask(int d) {
  delay(d);
}

void connectWiFi() {
  bool connected = false;
  for (int i = 0; i < 2; i++) {
    printSerialln("Attempting WIFI Loop #: " + String(i), 0);
    if (i != -1) {
      wifiSettings.ssid = wifiSettings.bup_ssid[i];
      wifiSettings.password = wifiSettings.bup_password[i];
      printSerialln("Trying backup credentials...", 0);
    } else {
      printSerialln("Trying stored credentials...", 0);
    }

    printSerialf("Connecting to WiFi: %s", wifiSettings.ssid.c_str());
    printSerialf("WiFi Password: %s", wifiSettings.password.c_str());
    WiFi.begin(wifiSettings.ssid.c_str(), wifiSettings.password.c_str());

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {

      printSerial(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      printSerialln("WiFi connected.", 0);
      printSerial("IP Address: ");
      printSerialln(WiFi.localIP().toString(), 0);
      wifiSettings.ipaddress = WiFi.localIP().toString();
      connected = true;
      break;
    } else {
      printSerialln("WiFi connection failed. Trying next network...", 500);

    }
  }

  if (!connected) {
    printSerialln("No Wi-Fi connection established. Starting Access Point...", 500);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    printSerial("Access Point IP Address: ");
    printSerialln(WiFi.softAPIP().toString(), 0);
  }


  // Set mDNS hostname
  if (MDNS.begin(MYSTTECH_MODEL)) {
    printSerialln("mDNS responder started: " + String(MYSTTECH_MODEL), 500);
  } else {
    printSerialln("Error starting mDNS", 500);
  }
}

unsigned long lastUpdateCheck = 0; // Global variable to track the last update check time
const unsigned long updateInterval = 3600000; // 1 hour in milliseconds

void loopFIRMWARE() {
  // Check if it's time to perform a firmware update check
  if (millis() - lastUpdateCheck >= updateInterval) {
    printSerialln("Checking for firmware updates...", 0);
    checkForUpdates();
    lastUpdateCheck = millis(); // Reset the timer
  }
}

void checkForUpdates() {
  HTTPClient http;
  String versionURL = String("https://raw.githubusercontent.com/") +
                      String(GITHUB_USER) + "/" +
                      String(GITHUB_REPO) + "/" +
                      String(GITHUB_BRANCH) + "/" +
                      "version.txt";

  printSerialln("Fetching version from URL: " + versionURL, 0);
  http.begin(versionURL);
  int httpCode = http.GET();
  if (httpCode == 200) {
    String newVersion = http.getString();
    newVersion.trim();

    if (newVersion != nodeSettings.storedVersion) {
      printSerialln("New version available: " + newVersion, 0);
      if (updateFirmware(newVersion)) {
        nodeSettings.storedVersion = newVersion;
        saveNodeSettings();
        printSerialln("Update complete. Rebooting...", 5000);
        ESP.restart();
      } else {
        printSerialln("Firmware update failed.", 0);
      }
    } else {
      printSerialln("Device firmware is up-to-date.", 0);
    }
  } else {
    printSerialln("Failed to fetch version info. HTTP code: " + String(httpCode), 0);
  }
  http.end();
}

bool updateFirmware(const String& newVersion) {
  HTTPClient http;
  String firmwareURL = String("https://raw.githubusercontent.com/") +
                       String(GITHUB_USER) + "/" +
                       String(GITHUB_REPO) + "/" +
                       String(GITHUB_BRANCH) + "/" +
                       String(GITHUB_BIN);

  printSerialln("Fetching firmware from URL: " + firmwareURL, 0);
  http.begin(firmwareURL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    int contentLength = http.getSize();
    if (Update.begin(contentLength)) {
      WiFiClient* client = http.getStreamPtr();
      size_t written = Update.writeStream(*client);

      if (written == contentLength) {
        printSerialln("Firmware written successfully.", 0);
        if (Update.end() && Update.isFinished()) {
          return true;
        } else {
          printSerialln("Update not finished.", 0);
        }
      } else {
        printSerialln("Firmware write failed.", 0);
      }
    } else {
      printSerialln("Not enough space for update.", 0);
    }
  } else {
    printSerialln("Failed to download firmware. HTTP code: " + String(httpCode), 0);
  }

  http.end();
  return false;
}



// Define the 7x7 bitmap for each character in "MystTech" using * and space
byte mystTechBMPArray[8][7] = {
  // 'M' (7x7) - Needs a diagonal shape
  {0b1000001, 0b1100011, 0b1010101, 0b1001101, 0b1000001, 0b1000001, 0b1000001},  // 'M'

  // 'y' (7x7) - Has a diagonal at the bottom
  {0b1000001, 0b1000001, 0b1000001, 0b0111111, 0b0000001, 0b0000001, 0b0000001},  // 'y'

  // 's' (7x7) - Standard S shape
  {0b0111111, 0b0100000, 0b0111111, 0b0000001, 0b0000001, 0b0111111, 0b0000000},  // 's'

  // 't' (lowercase, improved) (7x7) - Better horizontal bar and vertical stroke
  {0b0010000, 0b0010000, 0b0010000, 0b1111111, 0b0010000, 0b0010000, 0b0010000},  // 't'

  // 'T' (7x7) - Uppercase T with a wider top bar
  {0b1111111, 0b0010000, 0b0010000, 0b0010000, 0b0010000, 0b0010000, 0b0010000},  // 'T'

  // 'e' (7x7) - Standard e shape
  {0b0111111, 0b0100000, 0b0111111, 0b0100000, 0b0100000, 0b0111111, 0b0000000},  // 'e'

  // 'c' (7x7) - Standard c shape
  {0b0111111, 0b0100000, 0b0100000, 0b0100000, 0b0100000, 0b0100000, 0b0111111},  // 'c'

  // 'h' (7x7) - Standard h shape
  {0b1000001, 0b1000001, 0b1111111, 0b1000001, 0b1000001, 0b1000001, 0b1000001}   // 'h'
};


// Function to print the 7x7 bitmap characters for "MystTech" using * for blocks
void mystTechBMP() {

  for (int row = 0; row < 7; row++) {  // Loop over 7 rows for each character
    // Print the rows of all characters one by one
    for (int charIndex = 0; charIndex < 8; charIndex++) {
      for (int col = 0; col < 7; col++) {  // Loop over the 7 columns
        // Print '*' for '1' bit and space ' ' for '0' bit
        delay(3);

        if ((mystTechBMPArray[charIndex][row] >> (6 - col)) & 1) {
          printSerial("█");  // Print '*' for 'on' pixel
        } else {
          printSerial(" ");  // Print space for 'off' pixel
        }
      }
      printSerial("  ");  // Add space between characters
    }
    printSerialln("", 0); // Move to the next line after printing all characters for this row
  }
}
