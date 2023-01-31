
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <ESP8266WiFi.h>
#include <espnow.h>

#include <FastLED.h>

// REPLACE WITH RECEIVER MAC Address
// uint8_t broadcastAddress[] = { 0xBC, 0xFF, 0x4D, 0x49, 0xC7, 0x62 };
uint8_t broadcastAddress[] = { 0x94, 0x3C, 0xC6, 0x38, 0x2E, 0xA4 };


// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 2

const int lightsaber_sound_threshold = 14;
float acceleration_magnitude;

struct __attribute__((packed)) dataPacket {
  int unsigned id;
  int unsigned Right_Score;
  int unsigned Left_Score;
  int unsigned Seconds_Remaining;
  int unsigned Minutes_Remaining;
  bool Green_Light;
  bool Red_Light;
  bool White_Green_Light;
  bool White_Red_Light;
  bool Yellow_Green_Light;
  bool Yellow_Red_Light;
  bool Yellow_Card_Green;
  bool Yellow_Card_Red;
  bool Red_Card_Green;
  bool Red_Card_Red;
  bool Priority_Left;
  bool Priority_Right;
  bool Light_Saber_Sound;
};

// Create a struct_message called test to store variables to be sent
dataPacket packet;

unsigned long lastTime = 0;
unsigned long timerDelay = 100;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println();
  Serial.println("Commencing Setup:");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  packet.id = BOARD_ID;
  packet.Light_Saber_Sound = true;
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if ((millis() - lastTime) > timerDelay) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  }
  acceleration_magnitude = abs(pow((a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z), (0.5)) - 9.8);
  if (acceleration_magnitude > lightsaber_sound_threshold) {

    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *)&packet, sizeof(packet));
    lastTime = millis();
    Serial.println("Sent Lightsaber Sound.");
  }

  EVERY_N_MILLISECONDS(2000) {
    Serial.print("The acceleration is: ");
    Serial.println(acceleration_magnitude);
  }
}