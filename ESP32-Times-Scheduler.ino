/*
          ⠀ ⠀⠀⠀ ⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠊⠈⠑⣄⠀⠀⠀⡰⠋⠀⠙⢆⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⡰⠃⠀⠀⠀⠸⣶⣶⣶⡇⠀⠀⠀⠈⢦⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠒⠒⡶⠣⢤⠀⠀⣀⣀⠉⠉⡄⠀⢀⣶⣦⠀⠈⢷⠉⣉⡉⠀
          ⠀⠀⠀⠀⠠⠖⣾⡃⠒⠚⠀⠀⠻⠟⠀⠦⠷⠖⠀⠙⠉⠀⠀⠈⣏⠀⠉⠀
          ⠀⠀⠀⠀⢀⣾⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡆⠀⠀
          ⠀⠀⠀⢀⣞⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡀⠀
          ⠀⠀⢠⣿⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢧⠀
          ⠀⢀⠿⠿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡀
          ⠀⡜⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇
          ⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃
          ⠀⠹⡄⠀⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠏⠀
          ⠀⠀⠘⢦⣾⣿⡷⢲⣤⣄⣀⡀⠀⠀⠀⢠⠀⠀⣆⢰⠀⠘⡆⣀⠔⠁⠀⠀
          ⠀⠀⠀⠀⠉⠻⠅⣾⣿⡿⢀⣿⣿⣦⣀⣸⡀⠀⣸⠼⣆⢀⡏⠁⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠀⠀⠀⠑⠒⠁⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/

// ===== Include required libraries =====
#include <WiFi.h>             // WiFi functions
#include <WiFiManager.h>      // Auto WiFi configuration
#include <PubSubClient.h>     // MQTT client library
#include <ArduinoJson.h>      // For JSON parsing
#include <Preferences.h>      // Non-volatile storage in ESP32

// ===== MQTT configuration =====
#define mqtt_server "192.168.1.100"       // MQTT broker IP
#define request_add_time "esp32/add"      // Topic to receive time JSON (Add time)
#define request_rm_time "esp32/remove"    // Topic to receive time JSON (Remove time)
#define status "esp32/status"             // Topic to publish status messages


// ===== Global objects =====
WiFiClient espClient;              // TCP client used by MQTT
PubSubClient client(espClient);    // MQTT client using espClient
Preferences PREFS;                 // ESP32 flash storage

// ===== Global variables =====
String SlotTime[8];
int SizeOfSlot = 0;

// =======================================================================================
//                                  CONNECT WIFI AND SETTING MQTT SEVER
// =======================================================================================

void connectWiFi() {
  WiFiManager wm; // Create Oject WiFiManager

  // Try to connect to a saved WiFi network
  // If no credentials are saved, it starts an AP named "ESP32-Config"
  if (!wm.autoConnect("ESP32-Config")) {
    Serial.println("WiFi connect failed, restart ESP32");
    ESP.restart(); // restart for connect
  }

  // Debug Connected
  Serial.println("WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {     // Loop until successfully connected
    Serial.print("Connecting to MQTT...");

    // try to connect to MQTT broker with client ID "ESP32Client"
    if (client.connect("ESP32Client")) {

      // Connected successfully
      Serial.println("connected");
      client.subscribe(request_add_time);        // Subscribe to the topic stored in 'request_add_time'
      client.subscribe(request_rm_time);         // Subscribe to the topic stored in 'request_rm_time'
    } else {

      // Connection fails
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(", retrying in 2 seconds");
      delay(2000); // Wait 2 seconds before retrying
    }
  }
}

// =======================================================================================
//                                        PREFERENCES
// =======================================================================================

// Load Data From EEPROM
void loadTimes() {
  PREFS.begin("Time", true); // true -> Read Only

  SizeOfSlot = PREFS.getInt("Size", 0);                                              // Get Size of Slot (When dont have data set 0)
  Serial.printf("In Slot -> [%d/8]:\n", SizeOfSlot);                                    
  for (int i = 0; i < SizeOfSlot; i++) {
    SlotTime[i] = PREFS.getString(("slot" + String(i)).c_str(), "");                 // Get String Time At slot time in Flash Memory
    Serial.printf("%d.) %s\n", i + 1, SlotTime[i].c_str());
  }

  PREFS.end();
}

// Check Duplicate time
bool isDuplicate(const String &timeStr) {
  for (int i = 0; i < SizeOfSlot; i++) {
    if (SlotTime[i] == timeStr) return true;        // Check if current slot matches the target time 
  }

  return false;                                     // Not Found Duplicate (Dont matches the target time)
}

void SaveTimes(const String &timeStr) {
  // SizeOfSlot Empty
  if (SizeOfSlot >= 8) {
    Serial.println("Empty Slot Time");            // Debug if Fully Slot of time
    client.publish(status, "Empty Slot Time");    // Callback to Node Red
    return;
  } 

  if (!isDuplicate(timeStr)) {
    // Add time in Slot
    PREFS.begin("Time", false);                                              // (namespace, write) false -> Write/read 
    SlotTime[SizeOfSlot] = timeStr;
    PREFS.putString(("slot" + String(SizeOfSlot)).c_str(), timeStr);         // Save Time to Flash Memory
    SizeOfSlot++;                                                            
    PREFS.putInt("Size", SizeOfSlot);                                        // Update Size of Slot
    Serial.printf("Save [%d/8] : %s\n", SizeOfSlot, timeStr.c_str());      
    PREFS.end();         
    client.publish(status, "Add time Success");                              // send msg (Add time Success) -> node red
  } else {
    // if Duplicate time -> Fail
    Serial.println("Duplicate time, ignore"); 
    client.publish(status, "Duplicate time, ignore");                        // send msg (Duplicate time) -> node red
  }

  // Debug print all current slots
  Serial.println("Current Slots:");
  for (int i = 0; i < SizeOfSlot; i++) {
    Serial.printf("%d) %s\n", i + 1, SlotTime[i].c_str());
  }
}

void DeleteTimeSlot(int index) {
  if (index < 0 || index >= SizeOfSlot) return;

  PREFS.begin("Time", false);                                // (namespace, write) false -> Write/read 
  PREFS.remove(("slot" + String(index)).c_str());            // remove slot time at index position (in flash memory)

  // Shift remaining slots in EEPROM
  for (int i = index + 1; i < SizeOfSlot; i++) { 
    String oldKey = "slot" + String(i);                      // Get the current slot key in EEPROM 
    String newKey = "slot" + String(i - 1);                  // Prepare the new key where the value will be moved
    
    String value = PREFS.getString(oldKey.c_str(), "");      // Get String from oldKey
    PREFS.putString(newKey.c_str(), value);                  // And put String at newKey
  }

  PREFS.remove((String("slot") + (SizeOfSlot - 1)).c_str()); // Remove the last slot
  SizeOfSlot--;                                              
  PREFS.putInt("Size", SizeOfSlot);                          // Update Size of Slot
  PREFS.end();

  // Shift array in RAM
  for (int i = index; i < SizeOfSlot; i++) {
    SlotTime[i] = SlotTime[i + 1];                           // Move element down one index
  }

  Serial.printf("Deleted slot %d\n", index + 1);
  client.publish(status, "Delete Success");                  // send to node red
} 

void removeTimes(const String &timeStr) {
  int i = 0;
  while (i < SizeOfSlot) {                         // Loop through all slots 
    if (SlotTime[i] == timeStr) {                  // Check if current slot matches the target time 
      DeleteTimeSlot(i);
      continue;
    }

    i++;                                           // Move to next slot if no deletion
  }

  // Debug print all current slots
  Serial.println("Current Slots:");
  for (int j = 0; j < SizeOfSlot; j++) {
    Serial.printf("%d) %s\n", j + 1, SlotTime[j].c_str());
  }
}

// =======================================================================================
//                                        MQTT BROKER
// =======================================================================================

void callback(char* topic, byte* payload, unsigned int length) {
  // Change Payload -> String JSON 
  String JSON = "";
  for (unsigned int i = 0; i < length; i++) {
    JSON += (char)payload[i];
  }

  // Debug JSON
  Serial.println("JSON Time : " + JSON);
  // {
  // "hour":2,
  // "minute":26,
  // "second":null}"
  // }

  StaticJsonDocument<200> doc;                                 // Convert String JSON -> JsonDocument (JSON object)
  DeserializationError error = deserializeJson(doc, JSON);     // String JSON -> Subtime (Key, Value)

  if (error) {
    // When JSON Error
    Serial.println("JSON Parse Failed");
    client.publish(status, "JSON Parse Failed");

    return;
  } 

  // get time from JSON (Store with Key)
  int hour   = doc["hour"];
  int minute = doc["minute"];
  int second = doc["second"];

  // Convert SubTime -> String time
  char Buffer[9];
  sprintf(Buffer, "%02d:%02d:%02d", hour, minute, second);
  String timeStr = String(Buffer);

  // Debug timeString
  Serial.println("String Buffer : " + timeStr);

  // Check topic from mqtt sever
  if (String(topic) == request_add_time) {
    SaveTimes(timeStr);                        
  } else if (String(topic) == request_rm_time) {
    removeTimes(timeStr);
  }
}

// =======================================================================================
//                                          MAIN SETUP
// =======================================================================================

void setup() {
  Serial.begin(115200);
  
  connectWiFi();      // CONNECT WIFI
  loadTimes();        // LOAD DATA FROM Flash Memory

  // SETTING MQTT 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// =======================================================================================
//                                          MAIN LOOP
// =======================================================================================

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();
}