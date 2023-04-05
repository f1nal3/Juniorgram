#include "Hashing.hpp"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

namespace Base::Hashing
{
    using namespace CryptoPP;

std::string Base::Hashing::SHA_256(const std::string& plainText, const std::string& componentForSalt)
{
    SHA256       hash;
    std::string  digest;
    auto         stream = new HashFilter(hash, new HexEncoder(new StringSink(digest)));
    StringSource strSrc(componentForSalt + plainText, true, stream);

    return digest;
}
}  // namespace Base::Hashing