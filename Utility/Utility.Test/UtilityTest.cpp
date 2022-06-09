#include <catch2/catch.hpp>

#include "Utility/Utility.hpp"

using Catch::Matchers::EndsWith;

TEST_CASE("Test removeSpaces")
{
    std::string checkString("abcd def zxq");
    std::string ususalString("abcd    def     zxq");
    std::string startEndSpacesString("     abcd    def     zxq     ");
    std::string manySpacesString("     ");
    std::string emptyString("");
    REQUIRE(Utility::removeSpaces(ususalString) == checkString);
    REQUIRE(Utility::removeSpaces(startEndSpacesString) == checkString);
    REQUIRE(Utility::removeSpaces(manySpacesString) == "");
    REQUIRE(Utility::removeSpaces(emptyString) == "");
}

TEST_CASE("Test getFldPath")
{
    SECTION("Input selected not-existent path")
    {
        REQUIRE_THAT(Utility::getFldPath("Logs"), EndsWith("Logs"));
    }
    SECTION("Input selected existing path")
    {
        REQUIRE_THAT(Utility::getFldPath("Logs"), EndsWith("Logs"));
    }
    std::filesystem::remove("Logs");
}