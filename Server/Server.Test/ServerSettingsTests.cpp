#include <catch2/catch.hpp>

#include "Settings.hpp"

using Server::Builder::Settings;

TEST_CASE("SetValue method of the Settings [Settings][Success]")
{
    Settings testSettings;

    SECTION("Addition of the new KEY with new VALUE")
    {
        CHECK_NOTHROW(testSettings.SetValue(std::make_pair("SomeKey", "SomeValue")));
    }

    SECTION("Addition of the existing KEY with new VALUE")
    {
        testSettings.SetValue(std::make_pair("SomeKey", "SomeValue"));
        CHECK_NOTHROW(testSettings.SetValue(std::make_pair("SomeKey", "NEWSomeValue")));
        REQUIRE(testSettings.GetValue("SomeKey") != "NEWSomeValue");
    }
}

TEST_CASE("GetValue method of the Settings [Settings][Success]")
{
    Settings testSettings;

    testSettings.SetValue(std::make_pair("SomeKey", "SomeValue"));

    SECTION("Return a VALUE with existing KEY")
    {
        CHECK_NOTHROW(testSettings.GetValue("SomeKey"));
        REQUIRE(testSettings.GetValue("SomeKey") == std::string("SomeValue"));
    }

    SECTION("Return a VALUE with non existing KEY")
    {
        CHECK_NOTHROW(testSettings.GetValue("SomeOtherKey"));
        REQUIRE(testSettings.GetValue("SomeOtherKey").empty());
    }
}
