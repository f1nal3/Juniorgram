#pragma once
namespace Style
{
inline constexpr auto minDPI = 100;
inline constexpr auto maxDPI = 300;
inline constexpr auto defDPI = 100;

[[nodiscard]] int   getDpiScale();
[[nodiscard]] float devicePixelRatio();

void setDpiScale(int factor);
void setDevicePixelRatio(float factor);

int valueDPIScale(int value);
int ConvertScale(int value, int scale);
}  // namespace Style
