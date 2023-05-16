#include <catch2/catch.hpp>

#include "Utility/Utility.hpp"

using Catch::Matchers::EndsWith;

TEST_CASE("Test removeSpaces")
{
    std::string ExpectingString("abcd def zxq");

    std::string ususalString("abcd    def     zxq");
    REQUIRE(Utility::removeSpaces(ususalString) == ExpectingString);

    std::string newlineString("abcd    def     zxq\n");
    REQUIRE(Utility::removeSpaces(newlineString) == ExpectingString);

    std::string startEndSpacesString("     abcd    def     zxq     ");
    REQUIRE(Utility::removeSpaces(startEndSpacesString) == ExpectingString);

    std::string ExpectingEmptyString("");

    std::string manySpacesString("     ");
    REQUIRE(Utility::removeSpaces(manySpacesString) == ExpectingEmptyString);

    std::string emptyString("");
    REQUIRE(Utility::removeSpaces(emptyString) == ExpectingEmptyString);
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
