#pragma once

#include <memory>
#include <string>

namespace Base::Crypto
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
}  // namespace Base::Crypto

namespace Base::Hashing
{
std::string SHA_256(const std::string& message, const std::string& componentForSalt);
}
