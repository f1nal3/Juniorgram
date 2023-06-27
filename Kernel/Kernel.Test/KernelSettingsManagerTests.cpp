#include <catch2/catch.hpp>

#include "SettingsManager.hpp"

using Kernel::Builder::SettingsManager;
using Kernel::Builder::Settings;
using Kernel::Builder::ParamType;

TEST_CASE("Constructor of the SettingsManager", "[SettingsManager]")
{
    SECTION("Constructing with SettingsManager argument") { CHECK_NOTHROW(SettingsManager(std::make_unique<Settings>())); }

    SECTION("Constructing with ArgParser argument")
    {
        std::vector<const char*> args{"SomeEmptyVector"};
        ArgParser                parserDefault(static_cast<int>(args.size()), args.data());

        CHECK_NOTHROW(SettingsManager(parserDefault));
    }
}

TEST_CASE("Reset method of the SettingsManager", "[SettingsManager]")
{
    SECTION("Changing private Settings field")
    {
        std::unique_ptr<Settings> nullSettings = nullptr;
        auto someSettings = std::make_unique<Settings>();
        
        SettingsManager testSettingsManager(std::move(nullSettings));

        CHECK_NOTHROW(testSettingsManager.reset(std::move(someSettings))); 
    }
}

TEST_CASE("GetConnectionOptions method of the SettingsManager", "[SettingsManager]")
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

TEST_CASE("GetKernelPort method of the SettingsManager", "[SettingsManager]")
{
    SECTION("Checking GetKernelPort return")
    {
        auto defaultSettings = std::make_unique<Settings>();
        (*defaultSettings.get()).setValue(std::make_pair(ParamType::KernelPort, "65001"));
        
        SettingsManager testSettingsManager(std::move(defaultSettings));

        REQUIRE(testSettingsManager.getKernelPort() == 65001);
    }
}
