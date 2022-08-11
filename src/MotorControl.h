#pragma once

#include "config.h"
#include <Arduino.h>

void InitMotor();
void MotorLeftRotation();
void MotorRightRotation();
void MotorBreak();
void MotorFreeWheeling();
void RampUpMotor(uint8_t MotorSpeed = MaxPWMMotor);
void RampDownMotor(uint8_t MotorSpeed = MaxPWMMotor);