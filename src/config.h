#pragma once

#include <array>

// LED Config
constexpr int LedPinRed    {21};

constexpr std::array<int, 1> LedPins {
    LedPinRed
};

// Motor Config
constexpr int MotorPin1 {33};
constexpr int MotorPin2 {32};
constexpr int MotorEnable {13};

constexpr uint8_t MaxPWMMotor {150};
constexpr bool LeftRotation {1};
constexpr bool RightRotation {0};

constexpr int Button0 {19};

constexpr std::array<int, 1> ButtonPins {
    Button0
};
