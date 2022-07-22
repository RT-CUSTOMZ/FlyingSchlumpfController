#pragma once

#include "SPIFFS.h"

void InitSPIFFS(void){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}