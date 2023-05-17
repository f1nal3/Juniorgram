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
using Base::RSAKeyPair;
using Base::Generators::ByteBlockGenerator;
using Base::Verifiers::HashVerifier;
using Base::KeyConfirmators::KeyConfirmation;
using Base::Crypto::Symmetric::AES_GCM;
using Base::KeyAgreement::ECDH;
using Base::SessionKeyHolder;
using CryptoPP::SecByteBlock;
using Network::EncryptionState;

TEST_CASE("Hash functions test", "[dummy]")
{
    SECTION("SHA_256 method with salt testing")
    {
        auto result = SHA_256("Password", "login");
        auto shouldBe = std::string{"910C8F08EDC485FD882502A907912C57BEA192373A585DC742A36B6583CDA252"}; 

        REQUIRE(result == shouldBe);
        REQUIRE_NOTHROW(SHA_256("Password", "login"));
    }

    SECTION("SHA_256 method without salt testing")
    {
        auto result   = SHA_256("Password");
        auto shouldBe = std::string{"E7CF3EF4F17C3999A94F2C6F612E8A888E5B1026878E4E19398B23BD38EC221A"};
        REQUIRE(result == shouldBe);
        REQUIRE_NOTHROW(SHA_256("Password"));
    }
}

TEST_CASE("RSA components test", "[dummy]")
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

        remove("juniorgram.test");
        remove("juniorgram.test.pub");
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

TEST_CASE("ByteBlockGenerator test", "[dummy]")
{
    SECTION("Check on randomness")
    {
        auto block1 = ByteBlockGenerator::Instance().generateBlock(8);
        auto block2 = ByteBlockGenerator::Instance().generateBlock(8);

        REQUIRE(block1 != block2);
    }
}

TEST_CASE("HashVerifier test", "[dummy]")
{
    Models::ConnectionInfo connInfo;
    connInfo._connectionID    = 1;
    connInfo._publicServerKey = std::string("publicServerKey");
    HashVerifier hashVerifier;
    auto         verifyingString        = hashVerifier.calculateVerifyingHash("hashOfPassword", connInfo);
    auto         sameVerifyingString    = hashVerifier.calculateVerifyingHash("hashOfPassword", connInfo);
    auto         anotherVerifyingString = hashVerifier.calculateVerifyingHash("hashOfPass", connInfo);

    REQUIRE(verifyingString == sameVerifyingString);
    REQUIRE(verifyingString != anotherVerifyingString);
    REQUIRE_NOTHROW(hashVerifier.calculateVerifyingHash("hashOfPassword", connInfo));
};

TEST_CASE("KeyConfirmation test", "[dummy]") {
    std::string                  verificationUnit{"unitToCheck"};
    KeyConfirmation<std::string> keyConfirmator(verificationUnit);

    REQUIRE(keyConfirmator.compareWithVerificationUnit(verificationUnit));
    REQUIRE_NOTHROW(KeyConfirmation<std::string>(verificationUnit));
    REQUIRE_NOTHROW(keyConfirmator.compareWithVerificationUnit(verificationUnit));
}

TEST_CASE("AES GCM test", "[dummy]")
{
    std::string        plainMessage{"Message"};
    std::string        authData{"authentificationData"};
    AES_GCM            aesGcm;

    SecByteBlock key = ByteBlockGenerator::Instance().generateBlock(CryptoPP::AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv  = ByteBlockGenerator::Instance().generateBlock(12);

    SECTION("Sended message has not any problems")
    {
        /// before encryption cipheredTextBuffer == plainMessage
        yas::shared_buffer cipheredTextBuffer(plainMessage.data(), plainMessage.size());

        REQUIRE(aesGcm.encrypt(cipheredTextBuffer, key, iv, authData) == EncryptionState::SUCCESS);

        // cipheredTextBuffer is send to other side; before decryption cipheredTextBuffer == decryptedTextBuffer
        yas::shared_buffer decryptedTextBuffer(cipheredTextBuffer);

        REQUIRE(aesGcm.decrypt(decryptedTextBuffer, key, iv, authData) == EncryptionState::SUCCESS);

        std::string decryptedMessage(decryptedTextBuffer.data.get(), decryptedTextBuffer.size);

        REQUIRE(decryptedMessage == plainMessage);
    }

    SECTION("Sended message has integration or verification problems")
    {
        /// before encryption cipheredTextBuffer == plainMessage
        yas::shared_buffer cipheredTextBuffer(plainMessage.data(), plainMessage.size());

        REQUIRE(aesGcm.encrypt(cipheredTextBuffer, key, iv, authData) == EncryptionState::SUCCESS);

        // Attack the first and last byte of the encrypted data and tag
        if (cipheredTextBuffer.size > 1)
        {
            *cipheredTextBuffer.data.get() |= 0x0F;
            *(cipheredTextBuffer.data.get() + cipheredTextBuffer.size - 1) |= 0x0F;
        }

        yas::shared_buffer decryptedTextBuffer(cipheredTextBuffer);

        REQUIRE(aesGcm.decrypt(decryptedTextBuffer, key, iv, authData) == EncryptionState::FAILURE);

        std::string decryptedMessage(decryptedTextBuffer.data.get(), decryptedTextBuffer.size);

        REQUIRE(decryptedMessage != plainMessage);
    }
}

TEST_CASE("ECDH test", "[dummy]")
{
    ECDH side1, side2;
    side1.generateKeys();
    side2.generateKeys();

    SecByteBlock sendedPublicKeyOfSide1 = side1.getPublicKey();
    SecByteBlock sendedPublicKeyOfSide2 = side2.getPublicKey();

    SecByteBlock sharedSecretSide1 = side1.calculateSharedKey(sendedPublicKeyOfSide2);
    SecByteBlock sharedSecretSide2 = side2.calculateSharedKey(sendedPublicKeyOfSide1);

    REQUIRE(sharedSecretSide1 == sharedSecretSide2);
}

TEST_CASE("SessionKeyHolder test")
{
    using Utility::GeneralCodes;

    SessionKeyHolder::Instance();
    SecByteBlock testKey = ByteBlockGenerator::Instance().generateBlock(8);
    SecByteBlock newKey  = ByteBlockGenerator::Instance().generateBlock(8);
    uint64_t     userId  = 1;

    SECTION("Correct situations", "[dummy]")
    {
        SecByteBlock sameTestKey = testKey;
        SecByteBlock sameNewKey  = newKey;

        REQUIRE_NOTHROW(SessionKeyHolder::Instance());
        REQUIRE(SessionKeyHolder::Instance().setKey(std::move(sameTestKey), userId) == GeneralCodes::SUCCESS);
        REQUIRE(SessionKeyHolder::Instance().getKey(userId) == testKey);
        REQUIRE(SessionKeyHolder::Instance().refreshKey(std::move(sameNewKey), userId) == GeneralCodes::SUCCESS);
        REQUIRE_NOTHROW(SessionKeyHolder::Instance().removeKey(userId));
    }

    SECTION("Incorrect situations", "[dummy]")
    {
        uint64_t     incorrectUserId = userId + 5;
        SecByteBlock sameTestKey     = testKey;
        SecByteBlock sameNewKey      = newKey;
        SessionKeyHolder::Instance().setKey(std::move(sameTestKey), userId);

        REQUIRE(SessionKeyHolder::Instance().setKey(std::move(sameTestKey), userId) == GeneralCodes::FAILED);
        REQUIRE(SessionKeyHolder::Instance().getKey(incorrectUserId).empty() == true);
        REQUIRE(SessionKeyHolder::Instance().refreshKey(std::move(sameNewKey), incorrectUserId) == GeneralCodes::FAILED);
        REQUIRE_NOTHROW(SessionKeyHolder::Instance().removeKey(incorrectUserId));
    }
}
