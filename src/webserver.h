#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>
#include "protocol.h"

JSONVar board;
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Replace with your network credentials (STATION)
const char* ssid = "ReplaceSSID";
const char* password = "ReplacePassword";

DevelopmentDemoMessage myDemoMessage;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

ButtonMessage incomingButtonPressed;
DevelopmentDemoMessage myDemoMessage;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .button { max-width: 300px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.speed { color: #fd7e14; }
    .card.angle { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card speed">
        <h4><i class="fas fa-speed"></i>Current Speed</h4><p><span class="reading"><span id="speed"></span> <sup>m</sup>&frasl;<sub>s</sub></span></p><p class="packet">Reading ID: <span id="rt1"></span></p>
        <form action="/sendSpeed" target="hidden-form">
          <input type="submit" class="card button" value="SubmitMessage"></button>
        </form><br>
      </div>
      <div class="card angle">
        <h4><i class="fas fa-angle"></i>Current Angle</h4><p><span class="reading"><span id="angle"></span> &deg;</span></p><p class="packet">Reading ID: <span id="rh1"></span></p>
        <form action="/sendAngle" target="hidden-form">
          <input type="submit" class="card button" value="SubmitMessage"></button>
        </form><br>
      </div>
      <div class="card lastpressedby">
        <h4><i class="fas fa-last-pressed-by"></i> Last Button pressed by Player</h4><p><span class="lastpressed"><span id="t2"></span> #</span></p><p class="packet">Reading ID: <span id="rt2"></span></p>
      </div>
      <div class="card numberOfButtons">
        <h4><i class="fas fa-number-of-buttons"></i> Number of Buttons</h4><p><span class="reading"><span id="numberofbuttons"></span> #</span></p><p class="packet">Reading ID: <span id="rh2"></span></p>
        <form action="/sendMessage" target="hidden-form">
          <input type="submit" class="card button" value="Enter Peering Mode"></button>
        </form><br>
      </div>
    </div>
    <form action="/sendMessage" target="hidden-form">
      <input type="submit" class="card button" value="SubmitMessage"></button>
    </form><br>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  document.getElementById("speed").innerHTML = obj.speed.toFixed(2);
  document.getElementById("angle").innerHTML = obj.angle.toFixed(2);
  document.getElementById("lastpressed").innerHTML = obj.lastpressed;
  document.getElementById("numberofbuttons").innerHTML = obj.numberofbuttons;
 }, false);
}
</script>
</body>
</html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void InitWiFi(){
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  
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
  server.onNotFound(notFound);
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

bool extractDataFromFrame (const uint8_t *incomingData){
  memcpy(&incomingButtonPressed, incomingData, sizeof(incomingButtonPressed));
  
  // board["CurrentBatteryVoltage"] = incomingButtonPressed.value;
  String jsonString = JSON.stringify(board);
  events.send(jsonString.c_str(), "new_readings", millis());
}