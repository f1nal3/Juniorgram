#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

#include <memory>
#include <string>

namespace SymmetricCipher
{
using namespace CryptoPP;

struct ICipher
{
    virtual std::string generateKey()                                                        = 0;
    virtual std::string encrypt(const std::string& data, const std::string& cipherKey)       = 0;
    virtual std::string decrypt(const std::string& cipherData, const std::string& cipherKey) = 0;
    virtual ~ICipher() = default;
};

struct ICiphersFactory
{
    virtual std::shared_ptr<ICipher> create() = 0;
    virtual ~ICiphersFactory()                = default;
};

struct AESCipher : ICipher
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

struct AESFactory : ICiphersFactory
{
    std::shared_ptr<ICipher> create() override { return std::make_shared<AESCipher>(); }
};
}  // namespace SymmetricCipher

namespace Hashing
{
using namespace CryptoPP;

std::string SHA_256(const std::string& message)
{
    SHA256 hash;
    std::string digest;
    StringSource(message, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    return digest;
}
}  // namespace Hashing
