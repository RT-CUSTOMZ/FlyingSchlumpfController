#pragma once

#include <array>
#include <experimental/array>

// LED Config
constexpr int LedPinRed    {3};

constexpr auto LedPins = std::experimental::make_array(
    LedPinRed
);

// Motor Config
constexpr int MotorPin1 {17};
constexpr int MotorPin2 {18};
constexpr int MotorDiv  {19};
constexpr int MotorEnable {15};

constexpr int MaxPWMMotor {100};
constexpr bool LeftRotation {1};
constexpr bool RightRotation {0};

enum class WIFIMode : uint32_t {
  AP_STA,
  AP
};

constexpr WIFIMode currentWIFIMode = WIFIMode::AP;

constexpr int Button0 {12};
constexpr int Button1 {13};
constexpr int Button2 {14};
constexpr int Button3 {16};

constexpr auto ButtonPins = std::experimental::make_array(
    Button0,
    Button1,
    Button2,
    Button3
);
