#include "Cryptography.hpp"


using CryptoPP::SHA256;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::SecByteBlock;
using CryptoPP::ECB_Mode;
using CryptoPP::AES;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using CryptoPP::HashFilter;
using crppByte_t = CryptoPP::byte;

namespace Base::Crypto
{

std::string AESCipher::generateKey()
{
    AutoSeededRandomPool rnd;

    SecByteBlock bytes(nullptr, AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(bytes, bytes.size());

    return {bytes.BytePtr(), bytes.BytePtr() + bytes.size()};
}

std::string AESCipher::encrypt(const std::string& data, const std::string& key)
{
    auto pKey = reinterpret_cast<const crppByte_t*>(&key[0]);
    encryptor.SetKey(pKey, key.size());

    std::string encryptedData;
    auto stream = new StreamTransformationFilter(encryptor, new HexEncoder(new StringSink(encryptedData)));
    StringSource strSrc(data, true, stream);
    return encryptedData;
}

std::string AESCipher::decrypt(const std::string& cipherData, const std::string& key)
{
    auto pKey = reinterpret_cast<const crppByte_t*>(&key[0]);
    decryptor.SetKey(pKey, key.size());

    std::string decryptedData;
    auto stream = new HexDecoder(new StreamTransformationFilter(decryptor, new StringSink(decryptedData)));
    StringSource strSrc(cipherData, true, stream);

    return decryptedData;
}
}  // namespace Base::Crypto

std::string Base::Hashing::SHA_256(const std::string& plainText, const std::string& componentForSalt)
{ 
    SHA256 hash;
    std::string digest;
    auto stream = new HashFilter(hash, new HexEncoder(new StringSink(digest)));
    StringSource strSrc(componentForSalt + plainText, true, stream);

    return digest;
}
