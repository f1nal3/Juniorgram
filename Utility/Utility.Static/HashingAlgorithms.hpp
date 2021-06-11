#pragma once
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

std::string getHashingValue(std::string aString)
{
    std::string digest;
    CryptoPP::SHA512 hash;
    
    CryptoPP::StringSource foo(aString, true, new CryptoPP::HashFilter(hash,  new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest))));
   
    return digest;
}