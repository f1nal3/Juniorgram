#pragma once

#include "StyleScale.hpp"
#include "StyleTypes.hpp"

namespace Style
{
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
