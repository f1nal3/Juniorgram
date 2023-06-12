#pragma once

#include "Settings.hpp"
#include "ArgParser.hpp"

namespace Server::Builder
{
/**
* @class SettingsManager
* @brief ServerBuilder settings class provider.
* @details Contains the high-level logic to transformation of the base parameters.
*/
class SettingsManager
{
public:
    SettingsManager() = default;

    explicit SettingsManager(std::unique_ptr<Settings> settings) : _settings(std::move(settings)) {}
    
    explicit SettingsManager(const ArgParser& parser) : _settings(parser.getSettings()) {}

    /**
    * @brief Method to set new Settings.
    * @details Sets a new collection of the parameters.
    *          Note: Changes the owner of the argument resource
    * @param std::unique_ptr<Settings> settings
    */
    void reset(std::unique_ptr<Settings> settings) { _settings = std::move(settings); }

    /**
    * @brief Method to generate connection string
    * @details It generates string to connect to the DB.
    *          In this case, for now, connection string to Postgre DB only.
    *          In the future, this may be a method template for connecting to different DB.
    */
    std::string getConnectionOptions() const
    {
        return std::string("dbname="    + _settings->getValue(ParamType::DBName) +
                           " hostaddr=" + _settings->getValue(ParamType::HostAddress) +
                           " password=" + _settings->getValue(ParamType::DBPassword) +
                           " port="     + _settings->getValue(ParamType::DBPort) +
                           " user="     + _settings->getValue(ParamType::DBUser));
    }
    
    /**
    * @brief Method to get a server port
    * @details It provides a server port
    */
    uint16_t getServerPort() const
    {
        return static_cast<uint16_t>(std::stoul(_settings->getValue(ParamType::ServerPort)));
    }

private:
    std::unique_ptr<Settings> _settings;
};
}  /// namespace Server::Builder
