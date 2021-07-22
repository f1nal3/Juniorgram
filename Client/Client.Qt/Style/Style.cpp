#include "Style.hpp"

#include <cmath>

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

namespace internal
{
void registerModule(ModuleBase* module) { StyleModules().push_back(module); }

}  // namespace internal

void startManager(int scale)
{
    setDpiScale(scale);
    startModules(scale);
}

}  // namespace Style
