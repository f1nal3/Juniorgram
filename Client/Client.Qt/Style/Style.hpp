#pragma once

#include "StyleTypes.hpp"

namespace Style
{
inline constexpr auto minDPI = 100;
inline constexpr auto maxDPI = 300;
inline constexpr auto defDPI = 100;

[[nodiscard]] int getDpiScale();

void setDpiScale(int factor);

int valueDPIScale(int value);
int ConvertScale(int value, int scale);

class palette;

void startManager(int scale);

namespace internal
{
class ModuleBase
{
public:
    virtual void start(int scale) = 0;
    virtual ~ModuleBase()         = default;
};

void registerModule(ModuleBase* module);



}  // namespace internal

}  // namespace Style
