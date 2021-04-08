#pragma once
#include <cmath>

namespace Style
{
inline constexpr auto minDPI = 75;
inline constexpr auto maxDPI = 300;
inline constexpr auto defDPI = 100;

[[nodiscard]] int getDpiScale();

void setDpiScale(int factor);

int valueDPIScale(int value);

}  // namespace Style
