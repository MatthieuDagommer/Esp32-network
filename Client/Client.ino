#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress2[] = {0x08, 0x3A, 0xF2, 0xB9, 0x18, 0x38};//server2
uint8_t broadcastAddress1[] = {0x08, 0x3A, 0xF2, 0xBA, 0x81, 0xD8};//server1
//client {0x10, 0x97, 0xBD, 0xD4, 0x0D, 0x20}
//uint8_t broadcastAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};// REPLACE WITH RECEIVER MAC ADDRESS



// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
 int numero;
 char message[32];
} struct_message;

int numPkt=0;
char* nameServeur;

struct_message myData;
struct_message dataRcv;

uint8_t* getMacAddress(){ 
  if (getNameRSSI("serveur1_G2", "serveur2_G2") == "serveur1_G2"){
    return broadcastAddress1;
  } else{
    return broadcastAddress2;
  }
  }

const char* getNameRSSI(const char* nameA,const char* nameB){
  int n = WiFi.scanNetworks();
  int rssiA=0;
  int rssiB=0;
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        for (int i = 0; i < n; ++i) {;
            if (WiFi.SSID(i)==nameA){
              rssiA=WiFi.RSSI(i);
            }
            if (WiFi.SSID(i)==nameB){
              rssiB=WiFi.RSSI(i);
            }
        }
    }
    if (rssiA>rssiB){
      return nameA;
    }
    else {
      return nameB;
    }
}

// callbacks for sending and receiving data
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 Serial.print("\r\nMaster packet sent:\t");
 Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
 memcpy(&dataRcv, incomingData, sizeof(dataRcv));
 Serial.print("\r\nBytes received: ");
 Serial.println(len);
 Serial.print("From slave: ");
}


void setup() {
 // Init Serial Monitor
 Serial.begin(115200);
 // Set device as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 Serial.println(WiFi.macAddress());
 // Init ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println(F("Error initializing ESP-NOW"));
   return;
 }
 Serial.print(F("Reciever initialized : "));
 
 // Define callback functions
 esp_now_register_send_cb(OnDataSent);
 esp_now_register_recv_cb(OnDataRecv);
 // Register peer
 esp_now_peer_info_t peerInfo;
 memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // Add peer
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println(F("Failed to add peer"));
   return;
 }
 memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // Add peer
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println(F("Failed to add peer"));
   return;
 }
 
  
}


void loop() {
  numPkt++;
 // Set values to send
 strcpy(myData.message, "Salut toi");
 myData.numero = numPkt;
 // Send message via ESP-NOW
 esp_err_t result = esp_now_send(getMacAddress(), (uint8_t *) &myData, sizeof(myData));
 delay(1000);
}
