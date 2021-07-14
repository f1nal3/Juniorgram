#include "Cryptography.hpp"

namespace SymmetricCipher
{
using namespace CryptoPP;

std::string AESCipher::generateKey()
{
    AutoSeededRandomPool rnd;

    SecByteBlock bytes(0x0, AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(bytes, bytes.size());

    return std::string(reinterpret_cast<const char*>(bytes.BytePtr()), bytes.size());
}

std::string AESCipher::encrypt(const std::string& data, const std::string& key)
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

std::string AESCipher::decrypt(const std::string& cipherData, const std::string& key)
{
    ECB_Mode<AES>::Decryption decryptor;

    const byte* pKey = reinterpret_cast<const byte*>(&key[0]);
    decryptor.SetKey(pKey, key.size());

    std::string decryptedData;

    StringSource(cipherData, true,
        new HexDecoder(new StreamTransformationFilter(decryptor, 
            new StringSink(decryptedData))));

    return decryptedData;
}
}  // namespace SymmetricCipher

namespace Hashing
{

std::string SHA_256(const std::string& plainText, const std::string& componentForSalt)
{
    using namespace CryptoPP;

    SHA256 hash;
    std::string digest;
    StringSource(componentForSalt + plainText, true,
                 new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    return digest;
}

std::string getSHA512HashingValue(const std::string& hashableStr)
{
    CryptoPP::SHA512 hash;
    std::string digest;

    CryptoPP::StringSource foo(
        hashableStr, true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::Base64Encoder(
                            new CryptoPP::StringSink(digest), false)));

    return digest;
}

std::string getSHA1HashingValue(const std::string& hashableStr)
{
    CryptoPP::SHA1 hash;
    std::string digest;

    CryptoPP::StringSource foo(
        hashableStr, true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest), false)));

    return digest;
}

}  // namespace Hashing

namespace Coding
{
std::string getBASE64CodedValue(std::string& decodedStr)
{
    using CryptoPP::Name::Pad;
    using CryptoPP::Name::InsertLineBreaks;

   
       std::string encoded;
        CryptoPP::Base64Encoder encoder;

       CryptoPP::AlgorithmParameters params =
            CryptoPP::MakeParameters(Pad(), false)(InsertLineBreaks(), false);
       encoder.IsolatedInitialize(params);
       encoder.Attach(new CryptoPP::StringSink(encoded));

       CryptoPP::ArraySource as(reinterpret_cast<CryptoPP::byte*>(decodedStr.data()),
                                sizeof(decodedStr), true, new CryptoPP::Redirector(encoder));
      
    return encoded;
}

std::string getBASE64DecodedValue(std::string& codedStr)
{
    using CryptoPP::Name::Pad;
    using CryptoPP::Name::InsertLineBreaks;

    std::string decoded;
    CryptoPP::Base64Decoder decoder;

    CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(Pad(), false)(InsertLineBreaks(), false);
    
    decoder.IsolatedInitialize(params);
    decoder.Attach(new CryptoPP::StringSink(decoded));

    CryptoPP::ArraySource as(reinterpret_cast<CryptoPP::byte*>(codedStr.data()), sizeof(codedStr),
                               true, new CryptoPP::Redirector(decoder));

    return decoded;
}
}  // namespace Coding

namespace Signing
{
std::string signData(const std::shared_ptr<Network::Connection>& client, std::string& data)
{
    Utility::SignerAndVerifier::Instance().initPoint(
        client->getKeyDestributor().get()->getPrivateServerKey());
    Utility::SignerAndVerifier::Instance().initAndSavePrivateAndPublicKey();
    Utility::SignerAndVerifier::Instance().loadPrivateKey();

    return Utility::SignerAndVerifier::Instance().sign(data);
}

bool verifyData(const std::string& data, const std::string& signature)
{
    Utility::SignerAndVerifier::Instance().loadPublicKey();

    return Utility::SignerAndVerifier::Instance().verify(data, signature);
}
}  // namespace Signing
