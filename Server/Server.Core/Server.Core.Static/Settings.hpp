#pragma once

namespace Server::Builder
{
struct ParamType
{
    constexpr static char* ServerPort  = "--serverport";
    constexpr static char* HostAddress = "--hostaddr";
    constexpr static char* DBName      = "--dbname";
    constexpr static char* DBPort      = "--port";
    constexpr static char* DBUser      = "--user";
    constexpr static char* DBPassword  = "--password";
};

class Settings
{
public:
    Settings() = default;

    inline Settings& SetValue(std::pair<std::string, std::string> value)
    {
        _settings.emplace(value);
        return *this;
    }

    const std::string& GetValue(std::string key) const { return _settings.at(key); }

private:
    std::map<std::string, std::string> _settings;
};
}  /// namespace Server::Builder