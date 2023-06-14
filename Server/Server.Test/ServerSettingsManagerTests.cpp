#include <catch2/catch.hpp>

#include "SettingsManager.hpp"

using Server::Builder::SettingsManager;
using Server::Builder::Settings;
using Server::Builder::ParamType;

TEST_CASE("Constructor of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Constructing with SettingsManager argument") { CHECK_NOTHROW(SettingsManager(std::make_unique<Settings>())); }

    SECTION("Constructing with ArgParser argument")
    {
        std::vector<const char*> args{"SomeEmptyVector"};
        ArgParser                parserDefault(static_cast<int>(args.size()), args.data());

        CHECK_NOTHROW(SettingsManager(parserDefault));
    }
}

TEST_CASE("Reset method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Changing private Settings field")
    {
        std::unique_ptr<Settings> nullSettings = nullptr;
        auto someSettings = std::make_unique<Settings>();
        
        SettingsManager testSettingsManager(std::move(nullSettings));

        CHECK_NOTHROW(testSettingsManager.reset(std::move(someSettings))); 
    }
}

TEST_CASE("GetConnectionOptions method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Checking GetConnectionOptions line")
    {
        auto defaultSettings = std::make_unique<Settings>();
        (*defaultSettings.get()).setValue(std::make_pair(ParamType::DBPort, "DBPort"))
                                .setValue(std::make_pair(ParamType::DBName, "DBName"))
                                .setValue(std::make_pair(ParamType::HostAddress, "HostAddr"))
                                .setValue(std::make_pair(ParamType::DBUser, "DBUser"))
                                .setValue(std::make_pair(ParamType::DBPassword, "DBPass"));

        SettingsManager testSettingsManager(std::move(defaultSettings));
        std::string     checkString{"dbname=DBName hostaddr=HostAddr password=DBPass port=DBPort user=DBUser"};

        REQUIRE(testSettingsManager.getConnectionOptions() == checkString);
    }
}

TEST_CASE("GetServerPort method of the SettingsManager [SettingsManager][Success]")
{
    SECTION("Checking GetServerPort return")
    {
        auto defaultSettings = std::make_unique<Settings>();
        (*defaultSettings.get()).setValue(std::make_pair(ParamType::ServerPort, "65001"));
        
        SettingsManager testSettingsManager(std::move(defaultSettings));

        REQUIRE(testSettingsManager.getServerPort() == 65001);
    }
}
