#pragma once

#include <map>

namespace Server::Builder
{
struct ParamType
{
    constexpr static char const* ServerPort  = "--serverport";
    constexpr static char const* HostAddress = "--hostaddr";
    constexpr static char const* DBName      = "--dbname";
    constexpr static char const* DBPort      = "--port";
    constexpr static char const* DBUser      = "--user";
    constexpr static char const* DBPassword  = "--password";
};

class Settings
{
public:
    Settings() = default;

    inline Settings& SetValue(std::pair<std::string, std::string> value)
    {
        _settings.try_emplace(value.first, value.second);
        return *this;
    }

    std::string GetValue(const std::string& key) const
    {
        if (_settings.find(key) != _settings.end())
        {
            return _settings.at(key);
        }

        return std::string();
    }

private:
    std::map<std::string, std::string, std::less<>> _settings;
};
}  /// namespace Server::Builder