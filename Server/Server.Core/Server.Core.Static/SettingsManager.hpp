#pragma once

#include "Settings.hpp"

namespace Server::Builder
{
class SettingsManager
{
public:
    SettingsManager() = default;

    explicit SettingsManager(std::unique_ptr<Settings> settings) : _settings(std::move(settings)) {}
    
    void Reset(std::unique_ptr<Settings> settings) { _settings.reset(settings.release()); }

    /**
    * @brief Generates connection string
    * @details In this case, for now, connection string to Postgre DB only
    */
    std::string GetConnectionOptions() const
    {
        return std::string("dbname="    + _settings->GetValue(ParamType::DBName) +
                           " hostaddr=" + _settings->GetValue(ParamType::HostAddress) +
                           " password=" + _settings->GetValue(ParamType::DBPassword) +
                           " port="     + _settings->GetValue(ParamType::DBPort) +
                           " user="     + _settings->GetValue(ParamType::DBUser));
    }
    
    uint16_t GetServerPort() const
    {
        return static_cast<uint16_t>(std::stoul(_settings->GetValue(ParamType::ServerPort)));
    }

private:
    std::unique_ptr<Settings> _settings;
};
}  /// namespace Server::Builder
