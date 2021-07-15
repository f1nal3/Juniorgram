#pragma once
#define ATEXIT_FIXED 

#include <Utility.Static/EncryptionHandler.hpp>
    
#include <cryptopp/dh2.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/hmac.h>
#include <cryptopp/modes.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>

#include <cstdlib>

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
        mDHServer.GenerateKeyPair(mRng, mPrivServer, mPublicServer);
        return {reinterpret_cast<const char*>(mPublicServer.data()), mPublicServer.size()};
    }

    inline CryptoPP::SecByteBlock getPublicSecByteBlockServerKey()
    { 
        return mPublicServer;
    }

    inline CryptoPP::SecByteBlock getPrivateServerKey()
    {
        return mPrivServer;
    }

    inline void setPublicClientKey(const std::string& publicClientKey)
    {
        mPublicClient = CryptoPP::SecByteBlock(reinterpret_cast<const CryptoPP::byte*>(publicClientKey.data()),
                      publicClientKey.size());
    }

    inline void calculateSharedSecret()
    {
        mSharedSecret.Assign(CryptoPP::SecByteBlock(mDHServer.AgreedValueLength()));
     
        if (!mDHServer.Agree(mSharedSecret, mPrivServer, mPublicClient))
            throw std::runtime_error("Failed to reach shared secret!");

        Network::EncryptionHandler::calculateDigestAndGenerateIVBlock(mSharedSecret);

    }

    inline CryptoPP::OID getEC()
    { 
        return mEllipticCurve;
    }

private:
    CryptoPP::AutoSeededRandomPool mRng;
    CryptoPP::OID mEllipticCurve;
    CryptoPP::ECDH<CryptoPP::ECP>::Domain mDHServer;
    
    CryptoPP::SecByteBlock mPublicServer;
    CryptoPP::SecByteBlock mPrivServer;

    CryptoPP::SecByteBlock mPublicClient;

    CryptoPP::SecByteBlock mSharedSecret;
};
    
}  // namespace Utility