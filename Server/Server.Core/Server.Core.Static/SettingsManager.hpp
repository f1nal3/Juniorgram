#pragma once

//#include "Settings.hpp"

class SettingsManager
{
public:
    SettingsManager() = default;

    SettingsManager(const SettingsManager&)             = default;
    SettingsManager(SettingsManager&&)                  = default;
    SettingsManager& operator=(const SettingsManager&)  = default;
    SettingsManager& operator=(SettingsManager&&)       = default;

    inline void setValue(std::pair<std::string, std::string> keyValue)
    {
        _settings.insert(keyValue);
    }

    std::string GetConnectionOptions() const
    {
        std::string dbOptions = "";
        for (const auto& [configName, configValue] : _settings)
        {
            if (configName != "--serverport")
            {
                for (size_t i = 0; i < configName.length(); ++i)
                {
                    if (configName[i] != '-')
                    {
                        dbOptions += configName[i];
                    }
                }
                dbOptions += "=";
                dbOptions += configValue;
                dbOptions += " ";
            }
        }

        return dbOptions;
    }

    std::string GetServerPort() const { return _settings.find("--serverport")->second; }

private:
    std::map<std::string, std::string> _settings;
};
