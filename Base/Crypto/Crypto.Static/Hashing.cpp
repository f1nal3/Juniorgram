#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include "Hashing.hpp"

namespace Base::Hashing
{
    using CryptoPP::SHA256;
    using CryptoPP::HashFilter;
    using CryptoPP::HexEncoder;
    using CryptoPP::StringSink;
    using CryptoPP::StringSource;

    std::string SHA_256(const std::string& plainText, const std::string& componentForSalt)
    {
        return SHA_256(componentForSalt + plainText);
    }

    std::string SHA_256(const std::string& plainText)
    {
        SHA256       sha256;
        std::string  hashedValue;
        StringSource(plainText, true, new HashFilter(sha256, new HexEncoder(new StringSink(hashedValue))));

        return hashedValue;
    }
}  // namespace Base::Hashing
