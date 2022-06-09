#include <catch2/catch.hpp>

#include "Utility/Utility.hpp"

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
        std::string_view path = "E:\\Log";
        REQUIRE(Utility::getFldPath(path) == path);
    }
    SECTION("Input selected existing path")
    {
        REQUIRE(Utility::getFldPath() ==
                std::filesystem::absolute("E:\\C++\\Juniorgram\\out\\build\\x64-Debug\\Utility\\Utility.Test\\Log").string());
    }
}