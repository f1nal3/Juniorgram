#pragma once
namespace Style
{
inline constexpr auto minDPI = 100;
inline constexpr auto maxDPI = 300;
inline constexpr auto defDPI = 100;

[[nodiscard]] int getDpiScale();

void setDpiScale(int factor);

int valueDPIScale(int value);
int ConvertScale(int value, int scale);
}  // namespace Style