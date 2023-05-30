#include <catch2/catch.hpp>

#include "Settings.hpp"

using Server::Builder::Settings;

TEST_CASE("SetValue method of the Settings [Settings][Success]")
{
    Settings testSettings;

    SECTION("Addition of the new KEY with new VALUE")
    {
        CHECK_NOTHROW(testSettings.setValue(std::make_pair("SomeKey", "SomeValue")));
    }

    SECTION("Addition of the existing KEY with new VALUE")
    {
        testSettings.setValue(std::make_pair("SomeKey", "SomeValue"));
        CHECK_NOTHROW(testSettings.setValue(std::make_pair("SomeKey", "NEWSomeValue")));
        REQUIRE(testSettings.getValue("SomeKey") != "NEWSomeValue");
    }
}

TEST_CASE("GetValue method of the Settings [Settings][Success]")
{
    Settings testSettings;

    testSettings.setValue(std::make_pair("SomeKey", "SomeValue"));

    SECTION("Return a VALUE with existing KEY")
    {
        CHECK_NOTHROW(testSettings.getValue("SomeKey"));
        REQUIRE(testSettings.getValue("SomeKey") == std::string("SomeValue"));
    }

    SECTION("Return a VALUE with non existing KEY")
    {
        CHECK_NOTHROW(testSettings.getValue("SomeOtherKey"));
        REQUIRE(testSettings.getValue("SomeOtherKey").empty());
    }
}
