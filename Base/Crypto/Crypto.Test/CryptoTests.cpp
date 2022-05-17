#include <catch2/catch.hpp>
#include "Crypto.Static/Cryptography.hpp"

using Base::Hashing::SHA_256;
using Base::Crypto::AESFactory;

TEST_CASE("Cryptopp test", "[dummy]")
{
    SECTION("HASH method testing")
    {
        
        auto result = SHA_256("Password", "login");
        auto shouldBe = std::string{"910C8F08EDC485FD882502A907912C57BEA192373A585DC742A36B6583CDA252"}; 

        REQUIRE(result == shouldBe);
    }

    SECTION("Generation of secret KEY") { 
        auto chiper = AESFactory().create();
        auto defAESkeyLenght = 128 / (sizeof(char) * 8);

        auto key = chiper->generateKey();
        REQUIRE(key.size() == defAESkeyLenght);

    }


    SECTION("We could encript and decript") {

        auto chiper = AESFactory().create();
        auto msg = std::string ("some random message");
        auto key = chiper->generateKey();

        auto encryptedMsg = chiper->encrypt(msg, key);
        auto decryptedMsg = chiper->decrypt(encryptedMsg, key);

        REQUIRE(encryptedMsg != msg);
        REQUIRE(encryptedMsg != decryptedMsg);
        REQUIRE(decryptedMsg == msg);
    }
  
}