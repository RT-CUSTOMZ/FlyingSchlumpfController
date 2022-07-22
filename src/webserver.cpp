#include "webserver.h"
#include "config.h"
#include <esp_now.h>
#include "SPIFFS.h"
#include "Credentials.h"

JSONVar board;
AsyncWebServer server(80);
AsyncEventSource events("/events");

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

ButtonMessage incomingButtonPressed;
DevelopmentDemoMessage myDemoMessage;


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void WaitUntilConnected(){

  if(currentWIFIMode == WIFIMode::AP_STA){
    // Set device as a Wi-Fi AP and Station
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(STAssid, STApassword);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }

  } else if (currentWIFIMode == WIFIMode::AP){
    WiFi.mode(WIFI_AP);
    
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)…");

    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(APssid, APpassword);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  }


}

void GetConnectionStatusInfo(){
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
}

void HandleFileRequests(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css","text/css");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico","image/x-icon");
  });
  server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.png","image/png");
  });
}

void HandleFormRequests(){
  // Send a GET request to <ESP_IP>/sendMessage?input1=<inputMessage>
  server.on("/sendMessage", HTTP_PUT, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/sendMessage?input1=<inputMessage>
    
      static bool led_state = digitalRead(LedPinGreen);
      led_state = !led_state;
      digitalWrite(LedPinGreen, led_state);
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myDemoMessage, sizeof(myDemoMessage));

      inputMessage = "No message sent";
      inputParam = "none";
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });

}
void HandleBrowserClientConnection(){
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
}


void InitWiFi(){
  WaitUntilConnected();
  GetConnectionStatusInfo();
  HandleFileRequests();
  HandleFormRequests();
  HandleBrowserClientConnection();
  
  server.onNotFound(notFound);
  server.addHandler(&events);
  server.begin();
}

bool extractDataFromFrame (const uint8_t *incomingData){
  memcpy(&incomingButtonPressed, incomingData, sizeof(incomingButtonPressed));
  
  // board["CurrentBatteryVoltage"] = incomingButtonPressed.value;
  String jsonString = JSON.stringify(board);
  events.send(jsonString.c_str(), "new_readings", millis());
  return true;
}