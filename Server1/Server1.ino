#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0xB9, 0x18, 0x38};// Server2

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
 int numero;
 char message[32] ;
} struct_message;


struct_message myData;
struct_message dataRcv;

const char *soft_ap_ssid          = "serveur1_G2";
const char *soft_ap_password      = NULL;


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
 Serial.print(dataRcv.numero);
 Serial.print(dataRcv.message);
 
}


void setup() {
 // Init Serial Monitor
 Serial.begin(115200);
 // Set device as a Wi-Fi Station
 WiFi.mode(WIFI_AP_STA);
 Serial.println(WiFi.macAddress());

 Serial.println("\n[*] Creating ESP32 AP");
 WiFi.softAP(soft_ap_ssid, soft_ap_password);
 Serial.print("[+] AP Created with IP Gateway ");
 Serial.println(WiFi.softAPIP());
 
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
 memcpy(peerInfo.peer_addr, broadcastAddress, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // Add peer
 if (esp_now_add_peer(&peerInfo) != ESP_OK) {
   Serial.println(F("Failed to add peer"));
   return;
 }
}


void loop() {
 // Set values to send
 //strcpy(myData.message, "Salut toi");
 // Send message via ESP-NOW
 //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
 delay(1000);
}
