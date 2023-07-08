#include <catch2/catch.hpp>
#include "Crypto.Static/Hashing.hpp"
#include "Crypto.Static/RSA.hpp"
#include "Crypto.Static/SessionKeyHolder.hpp"
#include "Crypto.Static/RSAKeyManager.hpp"
#include "Crypto.Static/HashVerifier.hpp"
#include "Crypto.Static/ByteBlockGenerator.hpp"
#include "Crypto.Static/AES_GCM.hpp"
#include "Crypto.Static/ECDH.hpp"
#include "Crypto.Static/KeyConfirmation.hpp"

using Base::Hashing::SHA_256;
using Base::Crypto::Asymmetric::RSA;
using Base::RSAKeyManager;
using Base::RSAKeyGenerator;
using Models::RSAKeyPair;

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

TEST_CASE("RSA encoding test", "[dummy]")
{
    auto testKeySize = RSAKeyGenerator::RSASecurityStrength::test;

    SECTION("RSAKeyGenerator test")
    {
        using CryptoPP::StringSink;

        RSAKeyGenerator rsaKeyGenerator;
        CryptoPP::RSA::PublicKey publicRsaKeyToCompare;

        RSAKeyPair keyPair = rsaKeyGenerator.generateRSAKeyPair(testKeySize);

        REQUIRE_NOTHROW(rsaKeyGenerator.generateRSAKeyPair(testKeySize));
        REQUIRE_NOTHROW(publicRsaKeyToCompare = rsaKeyGenerator.generateRSAPublicKey(keyPair.privateKey));

        std::string publicKeyStr = keyPair.getPublicKeyAsString();

        std::string publicRsaKeyToCompareStr;
        StringSink stringSink(publicRsaKeyToCompareStr);
        publicRsaKeyToCompare.DEREncode(stringSink);

        REQUIRE(publicKeyStr == publicRsaKeyToCompareStr);
    }

    SECTION("RSAKeyManager test")
    {
        using CryptoPP::StringSink;

        RSAKeyManager rsaKeyManager(std::string("juniorgram.test"));

        REQUIRE_NOTHROW(RSAKeyManager());
        REQUIRE_NOTHROW(RSAKeyManager(std::string("JG.template")));
        REQUIRE_NOTHROW(rsaKeyManager.getPrivateKey());
        REQUIRE_NOTHROW(rsaKeyManager.getPublicServerKeyStr());
        REQUIRE_NOTHROW(rsaKeyManager.loadKeyPair());
        REQUIRE_NOTHROW(rsaKeyManager.saveKeyPair());

        RSAKeyManager sameRsaKeyManager(std::string("juniorgram.test"));
        sameRsaKeyManager.loadKeyPair();

        std::string privateKeyStr;
        StringSink  stringSink(privateKeyStr);
        rsaKeyManager.getPrivateKey().DEREncode(stringSink);

        std::string samePrivateKeyStr;
        StringSink  sameStringSink(samePrivateKeyStr);
        sameRsaKeyManager.getPrivateKey().DEREncode(sameStringSink);

        REQUIRE(privateKeyStr == samePrivateKeyStr);
        REQUIRE(rsaKeyManager.getPublicServerKeyStr() == sameRsaKeyManager.getPublicServerKeyStr());
    }

    SECTION("RSA encryption test")
    {
        std::string testMessage{"testMessage123"};
        RSA rsa;
        RSAKeyPair  keyPair = RSAKeyGenerator().generateRSAKeyPair(testKeySize);

        auto encryptedData = rsa.encrypt(testMessage, keyPair.publicKey);
        auto decryptedData = rsa.decrypt(encryptedData, keyPair.privateKey);

        REQUIRE(testMessage == decryptedData);
        REQUIRE_NOTHROW(rsa.encrypt(testMessage, keyPair.publicKey));
        REQUIRE_NOTHROW(rsa.decrypt(encryptedData, keyPair.privateKey));
    };
}
