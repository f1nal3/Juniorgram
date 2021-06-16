#pragma once

#include <cryptopp/dh2.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hmac.h>
#include <cryptopp/modes.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>

#include <cstdlib>

//#define ATEXIT_FIXED important moment!
namespace Utility
{
class KeyDestributor
{
public:
    KeyDestributor()
        : mRng{},
          mEllipticCurve{CryptoPP::ASN1::secp256r1()},
          mDHServer{mEllipticCurve},
          mPublicServer{mDHServer.PublicKeyLength()},
          mPrivServer{mDHServer.PrivateKeyLength()}
    {}
    virtual ~KeyDestributor() = default;

    KeyDestributor(const KeyDestributor& other) = delete;
    KeyDestributor& operator=(const KeyDestributor& other) = delete;

    KeyDestributor(KeyDestributor&& other) = delete;
    KeyDestributor& operator=(KeyDestributor&& other) = delete;

    inline std::string getPublicServerKey()
    {
        return {reinterpret_cast<const char*>(mPublicServer.data()), mPublicServer.size()};
    }

private:
    CryptoPP::AutoSeededRandomPool mRng;
    CryptoPP::OID mEllipticCurve;
    CryptoPP::ECDH<CryptoPP::ECP>::Domain mDHServer;
    
    CryptoPP::SecByteBlock mPublicServer;
    CryptoPP::SecByteBlock mPrivServer;
};
    
}  // namespace Utility