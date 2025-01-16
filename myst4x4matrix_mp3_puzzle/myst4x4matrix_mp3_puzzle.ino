

// access point
#define AP_SSID "LOST 4x4 Matrix MP3"
#define AP_PASSWORD "MystTech" // 'localhost:1880/admin'

#define PUZZLE_SOLUTION { 1, 3, 5, 7, 9, 11, 13, 15 }
#define RECORD_LOG true
#define SHOW_LOG true
#define SIZE_LOG 250

#define EXEC_BATCH1_PIN -1
#define EXEC_BATCH2_PIN -1

#define PUZZLE_NAME "LOST 4x4 Matrix MP3"
#define DESIGNER_NAME "Beyond Belief"
#define TECH_NAME "MYSTTECH Creations"
#define MYSTTECH_MODEL "MYST4X4MATRIX_MP3_PUZZLE"
#define DNSLOCAL "MYST4X4MATRIX_MP3_PUZZLE.LOCAL"

#define NR_TYPE "4X4_MATRIX_MP3"
#define NR_TAB "LOST"
#define NR_GROUP "Stage 3"

#define PULSE_MAX_RANGE 20 // NR_TYPE "3D_ROTARY_PULSE, 15000" "Arduino Dial, 20"
#define MP3_PLAYER true

#define NUM_DIGITAL_INPUTSA 0
#define NUM_DIGITAL_INPUTSB 0
#define NUM_ANALOG_INPUTPAIRS 0
#define NUM_DIGITAL_INPUTSMATRIX 4 // 4x4 (8)
#define NUM_DIGITAL_OUTPUTSA 8
#define NUM_DIGITAL_OUTPUT_ROWSA 2
#define NUM_DIGITAL_OUTPUTSB 6
#define NUM_DIGITAL_OUTPUT_ROWSB 1
#define NUM_FLED_OUTPUTS 0
#define NUM_FLED_CHANNELS 0  // 30/3
#define NUM_FLED_ADDLEDS 0  // Total number of LEDs (10 per dial)
#define NUM_FLED_ADDLEDS2 0 // total for around the room.
#define NUM_RXTX_PORTS 1 //mp3

// github
const char* GITHUB_USER = "consciousvisionaries";
const char* GITHUB_REPO = "LOST_MystTech";
const char* GITHUB_BRANCH = "main/myst4x4matrix_mp3_puzzle";
const char* GITHUB_BIN = "myst4x4matrix_mp3_puzzle.ino.esp32da.bin"; // esp32 wroom

bool allServicesActive = false;

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
