#pragma once

#include <array>

constexpr int LedPinRed    {13};
constexpr int LedPinYellow {2};
constexpr int LedPinGreen  {15};

constexpr std::array<int, 3> LedPins {
    LedPinRed, LedPinYellow , LedPinGreen
};
