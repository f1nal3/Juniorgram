#pragma once

#include "StyleScale.hpp"
#include "StyleTypes.hpp"

namespace Style
{
/**
 * @brief Starts all modules with scale
 * @param scale DPI factor 100%(96dpi)
 */
void startManager(int scale);

namespace internal
{
/**
 * @class ModuleBase
 * @brief Module for styles
 */
class ModuleBase
{
public:
    /**
     * @brief Start a module
     * @param scale Scale factor 100%(96dpi)
     */
    virtual void start(int scale) = 0;
    /**
     * @brief Destructor for a module
     */
    virtual ~ModuleBase() = default;
};

/**
 * @brief Register a module to a manager
 * @param module Module for styles
 */
void registerModule(ModuleBase* module);

}  // namespace internal

}  // namespace Style
