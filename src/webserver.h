#pragma once

#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>
#include "protocol.h"

extern JSONVar board;
extern AsyncWebServer server;
extern AsyncEventSource events;

// Replace with your network credentials (STATION)
extern const char* ssid;
extern const char* password;

// REPLACE WITH YOUR RECEIVER MAC Address
extern uint8_t broadcastAddress[];

extern ButtonMessage incomingButtonPressed;
extern DevelopmentDemoMessage myDemoMessage;

void notFound(AsyncWebServerRequest *request);

void InitWiFi();

bool extractDataFromFrame (const uint8_t *incomingData);