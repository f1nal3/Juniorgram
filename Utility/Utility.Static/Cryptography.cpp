#pragma once
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

#include <memory>
#include <string>

using namespace CryptoPP;

struct ISymmetricCipher
{
    virtual std::string generateKey()                                                        = 0;
    virtual std::string encrypt(const std::string& data, const std::string& cipherKey)       = 0;
    virtual std::string decrypt(const std::string& cipherData, const std::string& cipherKey) = 0;
    virtual ~ISymmetricCipher() = default;
};

struct ISymmetricCipherFactory
{
    virtual std::shared_ptr<ISymmetricCipher> makeCipherWay() = 0;
    virtual ~ISymmetricCipherFactory()                        = default;
};

struct AESCipher : ISymmetricCipher
{
    std::string generateKey()
    {
        AutoSeededRandomPool rnd;

        SecByteBlock bytes(0x0, AES::DEFAULT_KEYLENGTH);
        rnd.GenerateBlock(bytes, bytes.size());

        return std::string(reinterpret_cast<const char*>(bytes.BytePtr()), bytes.size());
    }

    std::string encrypt(const std::string& data, const std::string& key)
    {
        ECB_Mode<AES>::Encryption encryptor;

        const byte* pKey = reinterpret_cast<const byte*>(&key[0]);
        encryptor.SetKey(pKey, key.size());

        std::string encryptedData;

        StringSource(data, true,
            new StreamTransformationFilter(encryptor,
                new HexEncoder(new StringSink(encryptedData))));

        return encryptedData;
    }

    std::string decrypt(const std::string& cipherData, const std::string& key)
    {
        ECB_Mode<AES>::Decryption decryptor;

        const byte* pKey = reinterpret_cast<const byte*>(&key[0]);
        decryptor.SetKey(pKey, key.size());

        std::string decryptedData;

        StringSource(cipherData, true,
                     new HexDecoder(
                         new StreamTransformationFilter(decryptor, new StringSink(decryptedData))));

        return decryptedData;
    }
};

struct AESFactory : ISymmetricCipherFactory
{
    std::shared_ptr<ISymmetricCipher> makeCipherWay() override
    {
        return std::make_shared<AESCipher>();
    }
};
