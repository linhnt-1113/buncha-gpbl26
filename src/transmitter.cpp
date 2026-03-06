//Include Libraries
#include "esp_now.h"
#include "esp_wifi.h"
#include "WiFi.h"
#include "Wire.h"
#include "MPU6050_light.h"

// Transmitter MAC: 20:E7:C8:B2:7B:F8
// Receiver MAC: 68:FE:71:FD:79:C8

//Receiver MAC Address
uint8_t broadcastAddress[] = {0x68, 0xFE, 0x71, 0xFD, 0x79, 0xC8};

//Struct to send data
typedef struct __attribute__((packed)) {
  bool f;
  bool b;
  bool l;
  bool r;
} message;
message data;


//Create an object of predefined class MPU6050 
MPU6050 mpu(Wire);


//Initialize global variables
unsigned long timer = 0;
float x, y;
bool front = false, back = false, left = false, right = false;


//Initialize a peer
esp_now_peer_info_t peerInfo;

//function to be called upon callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  int channel = 1; // Pick a channel
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  
  
//initialize esp-now
  if (esp_now_init() != 0){
    Serial.println("Error Initializing ESP NOW");
    return;
  }
  
  //Register callback function
  esp_now_register_send_cb(OnDataSent);

  //Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  //Add peer
  if (esp_now_add_peer(&peerInfo) != 0){
    Serial.println("Failed to add peer");
    return;
  }

  // code for mpu6050 setup
  Wire.setPins(21, 22);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}

void loop() {
  //get data from mpu6050
  mpu.update();
  
  if((millis()-timer)>10){ //get data every 10ms
	x = mpu.getAngleX();
	y = mpu.getAngleY();

  //process data
	if(x >= 30)
    back = true;
  else
    back = false;
  if(x <= -30)
    front = true;
  else
    front = false;
  if(y >= 30)
    right = true;
  else
    right = false;
  if(y <= -30)
    left = true;
  else
    left = false;
	timer = millis();  

  data.f = front;
  data.b = back;
  data.l = left;
  data.r = right;
  //Send message
  esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  } 
}