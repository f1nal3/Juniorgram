#pragma once
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

inline std::string getSHA512HashingValue(const std::string& hashableString)
{
    std::string digest;
    CryptoPP::SHA512 hash;
    
    CryptoPP::StringSource foo(
        hashableString, true,
        new CryptoPP::HashFilter(hash,
        new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest), false)));

    return digest;
}
