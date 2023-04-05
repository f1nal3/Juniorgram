#include <catch2/catch.hpp>
#include "Crypto.Static/Hashing.hpp"

using Base::Hashing::SHA_256;

TEST_CASE("Cryptopp test", "[dummy]")
{
    SECTION("HASH method testing")
    {
        
        auto result = SHA_256("Password", "login");
        auto shouldBe = std::string{"910C8F08EDC485FD882502A907912C57BEA192373A585DC742A36B6583CDA252"}; 

        REQUIRE(result == shouldBe);
        REQUIRE_NOTHROW(SHA_256("Password", "login"));
    }
}
