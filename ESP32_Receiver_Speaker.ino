#define USE_HELIX

#include <SPI.h>
#include <SD.h>

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioLibs/AudioSourceSD.h"

#include <FastLED.h>

#include <esp_now.h>
#include <WiFi.h>

using namespace audio_tools;

bool play_sound = false;

// Favero Parser Data Structure
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

// Create a struct_message called packet
dataPacket packet;

float start_time;
float end_time;
bool running_song;

// Favero Settings
bool last_touch = false;
bool victory_song = false;
bool yellow_card = false;

// Create a structure to hold the readings from each board
// Board1 is from the Favero Parser
dataPacket board1;
// Board2 is from the Accelerometer
dataPacket board2;
// Board3 is from the Accelerometer
dataPacket board3;

// Create an array with all the structures
dataPacket boardsStruct[3] = { board1, board2, board3 };

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&packet, incomingData, sizeof(packet));
  Serial.print("Received data from board: ");
  Serial.print(packet.id - 1);
  play_sound = true;
}

// Button
const int BUTTON_PIN = 21;
// RandomPin
const int random_pin = 4;

// const int chipSelect=10;
const int chipSelect = 5;   //CS
#define PIN_SPI_SD_MOSI 23  //DI
#define PIN_SPI_SD_MISO 19  //DO
#define PIN_SPI_SD_CLK 18   //CLK


const char *urls[] = {
  // "http://centralcharts.ice.infomaniak.ch/centralcharts-128.mp3",
  // "http://centraljazz.ice.infomaniak.ch/centraljazz-128.mp3",
  // "http://centralrock.ice.infomaniak.ch/centralrock-128.mp3",
  // "http://centralcountry.ice.infomaniak.ch/centralcountry-128.mp3",
  // "http://centralfunk.ice.infomaniak.ch/centralfunk-128.mp3"
  "https://www.soundhelix.com/examples/mp3/SoundHelix-Song-1.mp3"
};

// Uses an SD Card for an audio source
const char *startFilePath = "/";
const char *ext = ".mp3";
AudioSourceSD source(startFilePath, ext, chipSelect);

I2SStream i2s;
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);

void setup() {
  Serial.begin(115200);

  randomSeed(analogRead(random_pin));  // randomize using noise from random_pin

  // Prints the MAC Address
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // ESPNOW Connections
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // // Once ESPNow is successfully Init, we will register for recv CB to
  // Acceleration CallBack
  esp_now_register_recv_cb(OnDataRecv);

  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // Set PinMode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // setup output
  i2s.begin(i2s.defaultConfig(TX_MODE));

  // setup player
  player.setVolume(1);
  player.begin();
  // Turns off auto next
  player.setAutoNext(false);
  // Adjust the buffer size from 1024 bytes to 262144
  player.setBufferSize(1024);
}

void loop() {
  player.copy();

  // Creates a datapacket
  dataPacket packet;

  // if ((acceleration_magnitude1 > 10) and (!running_song)) {
  if ((play_sound) and (!player.isActive())) {

    // Serial.println(acceleration_magnitude1);
    start_time = millis();
    end_time = start_time + 2000;
    // player.setVolume(0.0);
    // player.play();
    // Sets Song to LightSaber
    int track_to_play = int(random(0, 10));
    player.setIndex(track_to_play);  // it generate random numbers from min to max)
    Serial.print("Playing Track: ");
    Serial.print(track_to_play);
    Serial.println(".");

    Serial.println("Running the LightSaber Sound");
    // running_song = true;
    play_sound = false;
  }

  if ((end_time < millis()) and (running_song)) {
    player.stop();
    running_song = false;
    // Resets Play Song
    play_sound = false;
    Serial.println("Stopping the Song");
  }
}
