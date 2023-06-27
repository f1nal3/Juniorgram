#include <catch2/catch.hpp>

#include "Settings.hpp"

using Kernel::Builder::Settings;

SCENARIO("Settings can set and get a value", "[Settings]")
{
    GIVEN("Empty settings object")
    {
        Settings testSettings;

        WHEN("Some value has been set to object by unused key")
        {
            CHECK_NOTHROW(testSettings.setValue(std::make_pair("TestKey", "TestValue")));

            THEN("We can get value from object by existing key") { REQUIRE(testSettings.getValue("TestKey") == "TestValue"); }

            THEN("We'll get an empty std::string object when trying to get a value with unused key")
            {
                REQUIRE(testSettings.getValue("UnusedKey") == std::string());
            }

            THEN("Setting a value using an existing key is not an error.")
            {
                CHECK_NOTHROW(testSettings.setValue(std::make_pair("TestKey", "TestNewValue")));
                AND_THEN("New value isn't set.") { REQUIRE(testSettings.getValue("TestKey") != "TestNewValue"); }
            }
        }
    }
}
 