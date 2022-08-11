#pragma once

#include <array>

// LED Config
constexpr int LedPinRed    {13};
constexpr int LedPinYellow {2};
constexpr int LedPinGreen  {15};

constexpr std::array<int, 3> LedPins {
    LedPinRed, LedPinYellow , LedPinGreen
};

// Motor Config
constexpr int MotorPin1 {33};
constexpr int MotorPin2 {32};
constexpr int MotorEnable {13};

constexpr int MaxPWMMotor {100};
constexpr bool LeftRotation {1};
constexpr bool RightRotation {0};

enum class WIFIMode : uint32_t {
  AP_STA,
  AP
};

constexpr WIFIMode currentWIFIMode = WIFIMode::AP;
