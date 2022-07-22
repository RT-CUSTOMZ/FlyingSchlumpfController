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


void notFound(AsyncWebServerRequest *request);

void InitWiFi();


bool extractDataFromFrame (const uint8_t *incomingData);