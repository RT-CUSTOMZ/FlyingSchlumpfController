/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include "statemachine.h"

#include <Arduino.h>

#include <WiFi.h>

#include <string_view>

#include "webserver.h"

#include "FileSystem.h"

MainFSM mainFSM;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  mainFSM.process_event(Trigger{
      std::string_view{reinterpret_cast<const char *>(mac), 6},
      std::string_view{reinterpret_cast<const char *>(incomingData), static_cast<size_t>(len)}});

  // extractDataFromFrame(incomingData);
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize SPIFFS
  InitSPIFFS();

  // Set device as a Wi-Fi Station
  InitWiFi();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println(WiFi.macAddress());

  for (auto pin : LedPins)
  {
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
  }

  for (auto pin : ButtonPins)
  {
    pinMode(pin, INPUT_PULLUP);
  }

  InitMotor();

  mainFSM.PeeringMode = true;
  mainFSM.start();
}

void loop()
{
}
