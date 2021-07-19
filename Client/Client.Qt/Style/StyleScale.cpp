#include "StyleScale.hpp"

#include <cmath>

namespace Style
{
namespace
{
int   ScaleValue = defDPI;
float pixelRatio = 1.0f;
}  // namespace

void setDpiScale(int factor)
{
    if (factor < minDPI) factor = minDPI;
    if (factor > maxDPI) factor = maxDPI / devicePixelRatio();
    ScaleValue = factor;
}

void setDevicePixelRatio(float factor) { pixelRatio = factor; }

float devicePixelRatio() { return pixelRatio; }

int getDpiScale() { return ScaleValue; }

int valueDPIScale(int value)
{
    double res = static_cast<double>(value) * static_cast<double>(ScaleValue) / 100.0;
    return static_cast<int>(res + (fmod(res, 10.0) >= 5.0 ? 0.5 : 0.0));
}
int ConvertScale(int value, int scale)
{
    double res = static_cast<double>(value) * static_cast<double>(scale) / 100.0;
    return static_cast<int>(res + (fmod(res, 10.0) >= 5.0 ? 0.5 : 0.0));
}
}  // namespace Style