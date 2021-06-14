#include "Style.hpp"

namespace Style
{
std::vector<internal::ModuleBase*>& StyleModules()
{
    static auto result = std::vector<internal::ModuleBase*>();
    return result;
}

void startModules(int scale)
{
    for (const auto module : StyleModules())
    {
        module->start(scale);
    }
}

namespace
{
int ScaleValue = defDPI;

}

namespace internal
{
void registerModule(ModuleBase* module) { StyleModules().push_back(module); }


}  // namespace internal

void setDpiScale(int factor)
{
    if (factor < minDPI) factor = minDPI;
    if (factor > maxDPI) factor = maxDPI;
    ScaleValue = factor;
}

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
void startManager(int scale)
{
    setDpiScale(scale);
    startModules(scale);
}

}  // namespace Style
