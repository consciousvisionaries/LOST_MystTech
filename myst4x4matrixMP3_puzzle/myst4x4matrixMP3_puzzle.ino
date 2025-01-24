

// access point
#define AP_SSID "LOST Matrix 4x4 MP3 Puzzle"
#define AP_PASSWORD "MystTech" // 'localhost:1880/admin'

#define PUZZLE_SOLUTION { 1, 3, 5, 7, 9, 11, 13, 15 }
#define PUZZLE_DISPLAY_SORTORDER { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }//{ 16,11,10,9,5,6,7,8,4,3,2,1,13,12,14,15 }
#define NUM_OF_SOLUTIONS 8
#define SOLUTION_TYPE "RELAY" // or MQTT for remote trigger
#define SOLUTION_MQTT_MESSAGE ""
#define RECORD_LOG true
#define SHOW_LOG true
#define SIZE_LOG 250
#define RESET_PREF false

#define MP3_PLAYER false
#define MP3_FAIL_FILENO 17
#define MP3_WIN_FILENO 18

#define EXEC_BATCH1_PIN 27
#define EXEC_BATCH2_PIN 14
#define FLED_PIN1 12
#define FLED_PIN2 16

#define PUZZLE_NAME "LOST Matrix 4x4 MP3 Puzzle"
#define DESIGNER_NAME "Beyond Belief"
#define TECH_NAME "MYSTTECH Creations"
#define MYSTTECH_MODEL "MYST4X4MATRIX_MP3_PUZZLE"
#define DNSLOCAL "MYST4X4MATRIX_MP3_PUZZLE.LOCAL"

#define NR_TAB "LOST"
#define NR_GROUP "Stage 2"
#define NR_TYPE "4X4MATRIX"
#define PULSE_MAX_RANGE 20 // NR_TYPE "3D_ROTARY_PULSE"

#define NUM_DIGITAL_INPUTSA 0
#define NUM_DIGITAL_INPUTSB 0
#define NUM_ANALOG_INPUTPAIRS 0
#define NUM_DIGITAL_IOMATRIXPAIRS 4
#define NUM_DIGITAL_OUTPUTSA 0
#define NUM_DIGITAL_OUTPUT_ROWSA 0
#define NUM_DIGITAL_OUTPUTSB 2
#define NUM_DIGITAL_OUTPUT_ROWSB 1
#define NUM_FLED_OUTPUTS 0
#define NUM_FLED_CHANNELS 0  // 30/3
#define NUM_FLED_ADDLEDS 0 // Total number of LEDs (10 per dial)
#define NUM_FLED_ADDLEDS2 0 // total for around the room.
#define NUM_RXTX_PORTS 1
#define NUM_I2C_PORTS 1

String bodyInputTitle = PUZZLE_NAME;
String bodyOutputATitle = "Override";
String bodyOutputBTitle = "";
String buttonsHTMLTitle = "";

// github
const char* GITHUB_USER = "consciousvisionaries";
const char* GITHUB_REPO = "LOST_MystTech";
const char* GITHUB_BRANCH = "LOST_4X4MATRIXMP3/myst4x4matrixMP3_puzzle";
const char* GITHUB_BIN = "myst4x4matrixMP3_puzzle.ino.esp32da.bin"; // esp32 wroom


String MQTT_CLIENT_ID = "";           // Replace with MAC Address

struct NodeSettings {
  String puzzleName = PUZZLE_NAME;
  String designer = DESIGNER_NAME;
  String tech = TECH_NAME;
  String mDNS = DNSLOCAL;
  String nrTab = NR_TAB;
  String nrGroup = NR_GROUP;
  String nrType = NR_TYPE;
  String storedVersion = "";
  String ssid = "";
  String password = "";
  String ipaddress = "";
  String mac = "";
};

NodeSettings nodeSettings;

struct WiFiSettings {
  String ssid = "";
  String password = "";
  String ipaddress = "";
  String mac = "";
  String bup_ssid[2] = { "TELUSDE0875_2.4G", "Beyond Entertainment" };
  String bup_password[2] = { "3X3K22832E", "Gary2019" };
};

WiFiSettings wifiSettings;

struct MQTTSettings {
  String mqttUsername = "9grsvy8373";
  String mqttPassword = "8bdehprsuz";
  String mqttBroker = "b37.mqtt.one";
  const char* mqttTopic = "9grsvy8373/lost";
  String bup_mqttBroker[3] = {"b37.mqtt.one", "192.168.0.129", "broker.emqx.io"};
  String bup_mqttUser[3] = {"9grsvy8373", "pro1polaris", ""};
  String bup_mqttPassword[3] = {"8bdehprsuz", "CVr819P*!", ""};

};

MQTTSettings mqttSettings;

struct GlobalSettings {

  String inputNames[8] = { "Dial 1a", "Dial 1b", "Dial 2a", "Dial 2b", "Dial 3a", "Dial 3b", "not used", "not used" };
  String outputNames[8] = { "Dial 1a", "Dial 1b", "Dial 2a", "Dial 2b", "Dial 3a", "Dial 3b", "not used", "not used" };
  String colorsFLEDChannels[3] = {"red", "green", "blue"};

};

GlobalSettings globalSettings;

struct GlobalHyperlinks {
  String adminButtonHTML = "";
  String homeButtonHTML = "";
};

GlobalHyperlinks globalHyperlinks;
