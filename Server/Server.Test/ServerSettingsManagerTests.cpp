#include <catch2/catch.hpp>

#include "SettingsManager.hpp"

using Server::Builder::SettingsManager;
using Server::Builder::Settings;
using Server::Builder::ParamType;

TEST_CASE("Constructor of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Constructing with parameters")
    {
        CHECK_NOTHROW(SettingsManager(std::make_unique<Settings>()));
    }
}

TEST_CASE("Reset method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Changing private Settings field")
    {
        std::unique_ptr<Settings> nullSettings = nullptr;
        auto someSettings = std::make_unique<Settings>();
        
        SettingsManager testSettingsManager(std::move(nullSettings));

        CHECK_NOTHROW(testSettingsManager.Reset(std::move(someSettings))); 
    }
}

TEST_CASE("GetConnectionOptions method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Checking GetConnectionOptions line")
    {
        auto defaultSettings = std::make_unique<Settings>();
        (*defaultSettings.get()).SetValue(std::make_pair(ParamType::DBPort, "DBPort"))
                                .SetValue(std::make_pair(ParamType::DBName, "DBName"))
                                .SetValue(std::make_pair(ParamType::HostAddress, "HostAddr"))
                                .SetValue(std::make_pair(ParamType::DBUser, "DBUser"))
                                .SetValue(std::make_pair(ParamType::DBPassword, "DBPass"));

        SettingsManager testSettingsManager(std::move(defaultSettings));
        std::string     checkString{"dbname=DBName hostaddr=HostAddr password=DBPass port=DBPort user=DBUser"};

        REQUIRE(testSettingsManager.GetConnectionOptions() == checkString);
    }
}

TEST_CASE("GetServerPort method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Checking GetServerPort return")
    {
        auto defaultSettings = std::make_unique<Settings>();
        (*defaultSettings.get()).SetValue(std::make_pair(ParamType::ServerPort, "65001"));
        
        SettingsManager testSettingsManager(std::move(defaultSettings));

        REQUIRE(testSettingsManager.GetServerPort() == 65001);
    }
}
