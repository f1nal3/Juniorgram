#pragma once

#include "Settings.hpp"

class SettingsManager
{
public:
    SettingsManager() = default;

    SettingsManager(const SettingsManager&)             = default;
    SettingsManager(SettingsManager&&)                  = default;
    SettingsManager& operator=(const SettingsManager&)  = default;
    SettingsManager& operator=(SettingsManager&&)       = default;

    explicit SettingsManager(const Settings& settings)
        : _settings(settings)
    {}

    std::string GetConnectionOptions() const
    {
        return std::string("dbname="      + _settings.DBName() +
                           " hostaddr="   + _settings.HostAddress() +
                           " password="   + _settings.DBPassword() +
                           " port="       + _settings.DBPort() +
                           " user="       + _settings.DBUser());
    }

    std::string GetServerPort() const { return _settings.ServerPort(); }

    DataAccess::IRepositoryManager* GetRepoManager() const { return _settings.RepoManager(); }

private:
    Settings _settings;
};
