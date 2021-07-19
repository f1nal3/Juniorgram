#pragma once

//#include <Network.Static/Connection.hpp>
#include "SignerAndVerifier.hpp"
#include "KeyDestributor.hpp"

#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/dh2.h>
#include <cryptopp/eccrypto.h>


#include <memory>
#include <string>

namespace Network
{
class Connection;   
}

namespace SymmetricCipher
{

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
    std::string generateKey();
    std::string encrypt(const std::string& data, const std::string& key);
    std::string decrypt(const std::string& cipherData, const std::string& key);
};

struct AESFactory : ICiphersFactory
{
    std::shared_ptr<ICipher> create() override { return std::make_shared<AESCipher>(); }
};
}  // namespace SymmetricCipher

namespace Hashing
{
std::string SHA_256(const std::string& message, const std::string& componentForSalt);
std::string getSHA512HashingValue(const std::string& hashableString);
std::string getSHA1HashingValue(const std::string& hashableStr);
}

namespace Coding
{
std::string getBASE64CodedValue(std::string& decodedStr);
std::string getBASE64DecodedValue(std::string& decodedStr);
}


namespace Signing
{
std::string signData(const std::shared_ptr<Network::Connection>& client, std::string& data);
bool verifyData(const std::string& data, const std::string& signature);
}
