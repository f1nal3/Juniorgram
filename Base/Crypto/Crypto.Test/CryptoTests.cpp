#include <catch2/catch.hpp>
#include "Crypto.Static/Hashing.hpp"
#include "Crypto.Static/RSA.hpp"
#include "Crypto.Static/SessionKeyHolder.hpp"
#include "Crypto.Static/RSAKeyManager.hpp"
#include "Crypto.Static/HashVerifier.hpp"
#include "Crypto.Static/ByteBlockGenerator.hpp"
#include "Crypto.Static/AES_GCM.hpp"

using Base::Hashing::SHA_256;

TEST_CASE("Hash functions test", "[dummy]")
{
    SECTION("SHA_256 method testing")
    {

        auto result = SHA_256("Password", "login");
        auto shouldBe = std::string{"910C8F08EDC485FD882502A907912C57BEA192373A585DC742A36B6583CDA252"}; 

        REQUIRE(result == shouldBe);
        REQUIRE_NOTHROW(SHA_256("Password", "login"));
    }
}
