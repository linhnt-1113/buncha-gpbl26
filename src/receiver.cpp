//Include Libraries
#include "esp_now.h"
#include "esp_wifi.h"
#include "WiFi.h"

//Set Motor PINs
#define ENA 2 //PWM Enabled
#define IN1 4
#define IN2 5
#define IN3 18
#define IN4 19
#define ENB 25 //PWM Enabled
 
//Initialize global variables
bool front = 0;
bool back = 0;
bool left = 0;
bool right = 0;

//Struct to receive data (same as on transmitter side)
typedef struct __attribute__((packed)) {
  bool f;
  bool b;
  bool l;
  bool r;
} message;
message data;

//Function to be called on callbacks
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
  memcpy(&data, incomingData, sizeof(data));
  front = data.f;
  back = data.b;
  left = data.l;
  right = data.r;

  Serial.print("Recv - F: "); Serial.print(front);
  Serial.print(" B: "); Serial.print(back);
  Serial.print(" L: "); Serial.print(left);
  Serial.print(" R: "); Serial.println(right);
}


//Functions for specific movements
void carforward() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() {
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(115200);
  //Initialize pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  WiFi.mode(WIFI_STA);
  int channel = 1; // Pick a channel
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  //Initialize esp-now
  if (esp_now_init() != ESP_OK){
    return;
  }

  //Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
  // front = data.f;
  // back = data.b;
  // left = data.l;
  // right = data.r;
void loop() {
  if (front == 1) {
    carforward();
  } 
  else if (back == 1) {
    carbackward();
  } 
  else if (left == 1) {
    carturnleft();
  } 
  else if (right == 1) {
    carturnright();
  } 
  else if (front == 0 && back == 0 && left == 0 && right == 0) {
    carStop();
  }
}
