#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "wave.h"
#include "still.h"

// C++ code for an interactive gingerbread house "village" using two ESP32s, LED lights, buttons, and motion sensors. 

// The project will emulate two neighbors "speaking" to each other over WiFi by transmitting simple greeting 
// messages when a neighbor "waves" and turning on the porch light when a doorbell is pressed.

// Define MAC Address of the other device (REPLACE with actual addresses)

// Find any device's unique MAC Address by running any program in Arduino, recording
// the MAC Address found in the Output (eg. MAC: a0:dd:6c:70:7e:c8), and converting
// it to the hexadecimal format needed for C++ code.

// Hari's Mac Address (Other)
uint8_t otherDeviceMAC[] = {0xA0, 0xDD, 0x6C, 0x6E, 0x93, 0x98}; 

// Rhea's Mac Address 
//uint8_t otherDeviceMAC[] = {0xA0, 0xDD, 0x6C, 0x70, 0x7E, 0xC8}; 

// Add your other device's MAC Address here and comment out line 14.

typedef struct struct_message {
  char a[32];
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;
TFT_eSPI tft = TFT_eSPI();

// Change to the GPIO pins you are using on your ESP-32
#define BUTTON_PIN 2
#define LED_PIN 17
#define MOTION_PIN 13

// State to avoid repeat sending
bool messageSent = false;

// Dimensions of still & waving images
// Correct dimensions for an ESP-32 screen
int waveW=105;
int waveH=135;
int stillW=89;
int stillH=135;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  Serial.print("Message: ");
  Serial.println(myData.a);

  // Display the wave image and message if "HELLO!" is received
  if (strcmp(myData.a, "HELLO!") == 0) {
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);
    tft.pushImage(20, 20, waveW, waveH, wave_resized); // Show wave image
    tft.setRotation(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);           // Set text color (foreground, background)
    tft.setTextSize(3);                               // Set text size
    tft.drawString("HELLO!", 20, 190);                // Display HELLO message

    Serial.println("Displayed HELLO! message and wave image.");
  }
}

unsigned long lastMotionTime = 0;   // Store the last time motion was detected
unsigned long debounceDelay = 50; 

void motionLed() {
  int motionState = digitalRead(MOTION_PIN); // Read the motion sensor state
  unsigned long currentTime = millis();     // Get current time

  // If motion is detected and enough time has passed since the last detection
  if (motionState == HIGH && (currentTime - lastMotionTime) > debounceDelay) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    Serial.println("Motion Detected!");
    lastMotionTime = currentTime; // Update last motion time
  } 
  // If no motion is detected
  else if (motionState == LOW) {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    Serial.println("No Motion.");
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pullup resistor
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT_PULLUP);

  // Initialize the TFT screen
  tft.init();
  tft.fillScreen(TFT_BLACK);

  // Set screen rotation to 180 degrees
  tft.setRotation(2);

  // Set default text properties
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);

  // Display the still image
  tft.pushImage(20, 20, stillW, stillH, still_resized);

  // Set button pin as input
  //pinMode(BUTTON_LEFT, INPUT_PULLUP);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  memcpy(peerInfo.peer_addr, otherDeviceMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

unsigned long messageDisplayStartTime = 0; // Timestamp when the message is displayed
bool messageCleared = true;  
bool isIdle = true; // Track if the screen is in the idle state

void loop() {

  if (digitalRead(BUTTON_PIN) && !messageSent) {
    // Display the wave image and HELLO message on the sender's screen
    tft.setRotation(2);
    tft.pushImage(20, 20, waveW, waveH, wave_resized); // Show wave image
    tft.setTextColor(TFT_WHITE, TFT_BLACK);           // Set text color (foreground, background)
    tft.setTextSize(3);                               // Set text size
    tft.drawString("HELLO!", 20, 190);                // Display HELLO message

    // Send the "HELLO!" message to the receiver
    strcpy(myData.a, "HELLO!");
    esp_err_t result = esp_now_send(otherDeviceMAC, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    } else {
      Serial.println("Error sending the data");
    }

    // Record the message display time and avoid repeat sending
    messageDisplayStartTime = millis();
    messageCleared = false;
    isIdle = false;

    messageSent = true;
  }

  // Reset messageSent when button is released
  if (!digitalRead(BUTTON_PIN)) {
    messageSent = false;
  }

  // Clear the wave and HELLO message after 2 seconds
  if (!messageCleared && millis() - messageDisplayStartTime >= 2000) {
    // Clear the screen and return to the "still" image only once
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);
    tft.pushImage(20, 20, stillW, stillH, still_resized); // Show still image

    messageCleared = true; // Mark message as cleared
    isIdle = true;         // Return to idle state
  }

  // Ensure the "still" image is displayed once when entering idle state
  if (isIdle && !messageSent && !messageCleared) {
    // Only execute once when transitioning back to idle
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);
    tft.pushImage(20, 20, stillW, stillH, still_resized); // Show still image
    messageCleared = true; // Prevent redundant drawing
  }
}