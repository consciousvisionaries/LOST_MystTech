#include <Preferences.h>
Preferences preferences;

bool gameOnFlag = false;
bool gameOverFlag = false;
bool restartFlag = true;

bool solutionFound = false;
bool solutionStable = false;
int solutionWin[8] = PUZZLE_SOLUTION;
unsigned long solutionCheckStart = 0;

String MQTT_CLIENT_ID = "";

int FLEDCount[8] = {0, 0, 0, 0, 0, 0 , 0 , 0};
int lastFLEDCount[8] = { -1, -1, -1, -1, -1, -1, -1, -1};

void saveNodeSettings() {
  preferences.begin("node", false);
  preferences.putString("puzzleName", nodeSettings.puzzleName);
  preferences.putString("designer", nodeSettings.designer);
  preferences.putString("tech", nodeSettings.tech);
  preferences.putString("mDNS", nodeSettings.mDNS);
  preferences.putString("nrTab", nodeSettings.nrTab);
  preferences.putString("nrGroup", nodeSettings.nrGroup);
  preferences.putString("nrType", nodeSettings.nrType);
  preferences.putString("storedVersion", nodeSettings.storedVersion);
  preferences.end();
}

void loadNodeSettings() {
  preferences.begin("node", true);
  nodeSettings.puzzleName = preferences.getString("puzzleName", PUZZLE_NAME);
  nodeSettings.designer = preferences.getString("designer", DESIGNER_NAME);
  nodeSettings.tech = preferences.getString("tech", TECH_NAME);
  nodeSettings.mDNS = preferences.getString("mDNS", DNSLOCAL);
  nodeSettings.nrTab = preferences.getString("nrTab", NR_TAB);
  nodeSettings.nrGroup = preferences.getString("nrGroup", NR_GROUP);
  nodeSettings.nrType = preferences.getString("nrType", NR_TYPE);
  nodeSettings.storedVersion = preferences.getString("storedVersion", "");
  preferences.end();
}

void saveWiFiSettings() {
  preferences.begin("wifi", false);
  preferences.putString("ssid", wifiSettings.ssid);
  preferences.putString("password", wifiSettings.password);
  preferences.putString("ipaddress", wifiSettings.ipaddress);
  preferences.putString("mac", wifiSettings.mac);
  for (int i = 0; i < 2; i++) {
    String temp = ("bup_ssid" + String(i));  // Create the key string for bup_ssid
    preferences.putString(temp.c_str(), wifiSettings.bup_ssid[i]);

    temp = ("bup_password" + String(i));  // Create the key string for bup_password
    preferences.putString(temp.c_str(), wifiSettings.bup_password[i]);
  }
  preferences.end();
}

void loadWiFiSettings() {
  preferences.begin("wifi", true);
  wifiSettings.ssid = preferences.getString("ssid", "");
  wifiSettings.password = preferences.getString("password", "");
  wifiSettings.ipaddress = preferences.getString("ipaddress", "");
  wifiSettings.mac = preferences.getString("mac", "");
  for (int i = 0; i < 2; i++) {
    String temp = ("bup_ssid" + String(i));  // Create the key string for bup_ssid
    wifiSettings.bup_ssid[i] = preferences.getString(temp.c_str(), wifiSettings.bup_ssid[i]);

    temp = ("bup_password" + String(i));  // Create the key string for bup_password
    wifiSettings.bup_password[i] = preferences.getString(temp.c_str(), wifiSettings.bup_password[i]);
  }
  preferences.end();
}

void saveMQTTSettings() {
  preferences.begin("mqtt", false);
  preferences.putString("mqttUsername", mqttSettings.mqttUsername);
  preferences.putString("mqttPassword", mqttSettings.mqttPassword);
  preferences.putString("mqttBroker", mqttSettings.mqttBroker);
  for (int i = 0; i < 3; i++) {
    String temp = ("bup_mqttBroker" + String(i));  // Create the key string for bup_mqttBroker
    preferences.putString(temp.c_str(), mqttSettings.bup_mqttBroker[i]);

    temp = ("bup_mqttUser" + String(i));  // Create the key string for bup_mqttUser
    preferences.putString(temp.c_str(), mqttSettings.bup_mqttUser[i]);

    temp = ("bup_mqttPassword" + String(i));  // Create the key string for bup_mqttPassword
    preferences.putString(temp.c_str(), mqttSettings.bup_mqttPassword[i]);
  }
  preferences.end();
}

void loadMQTTSettings() {
  preferences.begin("mqtt", true);
  mqttSettings.mqttUsername = preferences.getString("mqttUsername", mqttSettings.mqttUsername);
  mqttSettings.mqttPassword = preferences.getString("mqttPassword", mqttSettings.mqttPassword);
  mqttSettings.mqttBroker = preferences.getString("mqttBroker", mqttSettings.mqttBroker);
  for (int i = 0; i < 3; i++) {
    String temp = ("bup_mqttBroker" + String(i));  // Create the key string for bup_mqttBroker
    mqttSettings.bup_mqttBroker[i] = preferences.getString(temp.c_str(), mqttSettings.bup_mqttBroker[i]);

    temp = ("bup_mqttUser" + String(i));  // Create the key string for bup_mqttUser
    mqttSettings.bup_mqttUser[i] = preferences.getString(temp.c_str(), mqttSettings.bup_mqttUser[i]);

    temp = ("bup_mqttPassword" + String(i));  // Create the key string for bup_mqttPassword
    mqttSettings.bup_mqttPassword[i] = preferences.getString(temp.c_str(), mqttSettings.bup_mqttPassword[i]);
  }
  preferences.end();
}

void saveGlobalSettings() {
  preferences.begin("global", false);
  preferences.putString("nrTab", nodeSettings.nrTab);
  preferences.putString("nrGroup", nodeSettings.nrGroup);
  for (int i = 0; i < 8; i++) {
    String temp = ("inputName" + String(i));  // Create the key string for inputName
    preferences.putString(temp.c_str(), globalSettings.inputNames[i]);

    temp = ("outputName" + String(i));  // Create the key string for outputName
    preferences.putString(temp.c_str(), globalSettings.outputNames[i]);
  }
  for (int i = 0; i < 3; i++) {
    String temp = ("colorsFLEDChannels" + String(i));  // Create the key string for colorsFLEDChannels
    preferences.putString(temp.c_str(), globalSettings.colorsFLEDChannels[i]);
  }
  preferences.end();
}

void loadGlobalSettings() {
  preferences.begin("global", true);
  nodeSettings.nrTab = preferences.getString("nrTab", NR_TAB);
  nodeSettings.nrGroup = preferences.getString("nrGroup", NR_GROUP);
  for (int i = 0; i < 8; i++) {
    String temp = ("inputName" + String(i));  // Create the key string for inputName
    globalSettings.inputNames[i] = preferences.getString(temp.c_str(), globalSettings.inputNames[i]);

    temp = ("outputName" + String(i));  // Create the key string for outputName
    globalSettings.outputNames[i] = preferences.getString(temp.c_str(), globalSettings.outputNames[i]);
  }
  for (int i = 0; i < 3; i++) {
    String temp = ("colorsFLEDChannels" + String(i));  // Create the key string for colorsFLEDChannels
    globalSettings.colorsFLEDChannels[i] = preferences.getString(temp.c_str(), globalSettings.colorsFLEDChannels[i]);
  }
  preferences.end();
}

void saveGlobalHyperlinks() {
  preferences.begin("hyperlinks", false);
  preferences.putString("homeButtonHTML", globalHyperlinks.homeButtonHTML);
  preferences.putString("adminButtonHTML", globalHyperlinks.adminButtonHTML);
  preferences.end();
}

void loadGlobalHyperlinks() {
  preferences.begin("hyperlinks", true);
  globalHyperlinks.homeButtonHTML = preferences.getString("homeButtonHTML", "");
  globalHyperlinks.adminButtonHTML = preferences.getString("adminButtonHTML", "");
  preferences.end();
}


#define GPIO03_U0RXD_LED_RX 3
#define GPIO01_U0TXD_LED_TX 1
#define GPIO16_U2RXD_WS2812_16 16
#define GPIO17_U2TXD 17
#define GPIO23_VSPI_MOSI 23 // digital out 1 - win game
#define GPIO22_I2C_SCL 22
#define GPIO21_I2C_SDA 21
#define GPIO19_VSPI_MISO_CAM_Y5 19 // digital out 2 - other
#define GPIO18_VSPI_CLK_CAM_Y4 18
#define GPIO13_ADC2_CH4_HSPI_MOS1_TOUCH4 13 // fled 2
#define GPIO12_ADC2_CH5_HSPI_MOS0_TOUCH5_MTDI 12 // fled
#define GPIO02_ADC2_CH2_BOOT_TOUCH2_LEDIO2 2
#define GPIO04_ADC2_CH0_CAM_Y2_TOUCH0 4 // dial 1
#define GPIO05_VSPI_CS_CAM_Y3_SDIO 5 // dial 1
#define GPIO26_ADC2_CH9_CAM_SIOD_DAC2_I2S_BCK 26 // digital input 1 /// end game
#define GPIO32_ADC1_CH4_TOUCH9 32 // digital input 2 /// start game.... new sort
#define GPIO27_ADC2_CH7_CAM_SIOC_TOUCH7 27 // digital input 3 /// trigger relay 3
#define GPIO33_ADC1_CH5_TOUCH8 33 // dial 2
#define GPIO14_ADC2_CH6_HSPI_CLK_TOUCH6 14 // dial 2
#define GPIO25_ADC2_CH8_DAC1_I2S_LCK 25
#define GPIN36_ADC1_CH0_VP 36
#define GPIN39_ADC1_CH3_VN 39
#define GPIN34_ADC1_CH6_CAM_Y8 34 // dial 3
#define GPIN35_ADC1_CH7_CAM_Y9 35 // dial 3
#define GPIO15_ADC2_CH3_MTDO_TOUCH3_HSPI_CS 15
#define GPIO00_ADC2_CH1_BOOT_TOUCH1 0
