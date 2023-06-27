#pragma once

#include <map>
#include "FileLogger.hpp"

namespace Kernel::Builder
{
struct ParamType
{
    constexpr static char const* KernelPort  = "--kernelport";
    constexpr static char const* HostAddress = "--hostaddr";
    constexpr static char const* DBName      = "--dbname";
    constexpr static char const* DBPort      = "--port";
    constexpr static char const* DBUser      = "--user";
    constexpr static char const* DBPassword  = "--password";
};

/**
* @class Settings
* @brief Class container. Stores the required parameters for Kernel class configuration.
* @details Technically it's a wrapper over std::map.
*/
class Settings
{
public:
    Settings() = default;

    /**
    * @brief Setting up parameter method.
    * @details Takes a pair as an argument and stores it in the inner container.
    *          If the parameter is duplicated, skips the new value and logs this action.
    * @param const std::pair<std::string, std::string>& value /
    *        In other words: pair<key, value>
    */
    Settings& setValue(const std::pair<std::string, std::string>& value)
    {
        if (!_settings.try_emplace(value.first, value.second).second)
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[Settings::setValue]: Write error! Parameter with key \"" + value.first + "\" already exists.",
                Base::Logger::LogLevel::ERR
            );
        }
        return *this;
    }

    /**
    * @brief Getting up parameter method.
    * @details Gets a value with the <key> from the map \
    *          If a key does not exist in the map, it will return an empty std::string.
    * @param const std::string& key
    */
    std::string getValue(const std::string& key) const
    {
        if (auto it = _settings.find(key); it != _settings.end())
        {
            return it->second;
        }
        return {};
    }

private:
    std::map<std::string, std::string, std::less<>> _settings;
};
}  /// namespace Kernel::Builder
