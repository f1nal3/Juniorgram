#pragma once

#include "Settings.hpp"

namespace Server::Builder
{
class SettingsManager
{
public:
    SettingsManager() = default;

    explicit SettingsManager(const Settings& settings) : _settings(settings) {}
    
    void Reset(const Settings& settings) { _settings = settings; }

    /**
    * @brief Generates connection string
    * @details In this case, for now, connection string to Postgre DB only
    */
    std::string GetConnectionOptions() const
    {
        return std::string("dbname=" + _settings.GetValue(ParamType::DBName) +
                           " hostaddr=" + _settings.GetValue(ParamType::HostAddress) +
                           " password=" + _settings.GetValue(ParamType::DBPassword) +
                           " port="     + _settings.GetValue(ParamType::DBPort) +
                           " user="     + _settings.GetValue(ParamType::DBUser));
    }
    
    std::string GetServerPort() const
    {
        return _settings.GetValue(ParamType::ServerPort);
    }

private:
    Settings _settings;
};
}  /// namespace Server::Builder
