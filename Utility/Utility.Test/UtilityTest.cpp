#include <catch2/catch.hpp>

#include "Utility/Utility.hpp"

TEST_CASE("Test removeSpaces")
{
    REQUIRE(Utility::removeSpaces("abcd    def     zxq")            == "abcd def zxq");
    REQUIRE(Utility::removeSpaces("     abcd    def     zxq     ")  == "abcd def zxq");
    REQUIRE(Utility::removeSpaces("     ")                          == "");
    REQUIRE(Utility::removeSpaces("")                               == "");
}