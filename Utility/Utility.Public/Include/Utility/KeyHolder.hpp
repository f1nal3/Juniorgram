#pragma once 

#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>

#include <cryptopp/oids.h>
#include <cryptopp/dh2.h>
#include <cryptopp/modes.h>

#include <cstdlib>

//#define ATEXIT_FIXED important moment!
namespace Utility
{
    class KeyHolder
    {
    public:

        static KeyHolder& Instance() 
        { 
            if (!mpInstance)
            {
                if (mDestroyed)
                {
                    onDeadReference();
                }
                else
                {
                    create();
                }
            }
            return *mpInstance;
        }

        virtual ~KeyHolder()
        { 
            mpInstance = nullptr;
            mDestroyed = true;
        }

        inline static void killKeyHolder()
        { 
           mpInstance->~KeyHolder();
        }

        inline std::string getPublicClientKey()
        {
            return {reinterpret_cast<const char*>(mPublicClient.data()), mPublicClient.size()};
        }

        inline void setPublicServerKey(const std::string& publicServerKey)
        {
            mPublicServer = CryptoPP::SecByteBlock(
                reinterpret_cast<const CryptoPP::byte*>(publicServerKey.data()),
                publicServerKey.size());
        }

        inline void calculateSharedSecret()
        {
            mSharedSecret.Assign(CryptoPP::SecByteBlock(mDHClient.AgreedValueLength()));

            if (!mDHClient.Agree(mSharedSecret, mPrivClient, mPublicServer))
                throw std::runtime_error("Failed to reach shared secret!");

             Network::EncryptionHandler::calculateDigest(mSharedSecret);
        }

    private:
        
        inline static void create()
        {
            static KeyHolder theInstance;
            mDHClient.GenerateKeyPair(mRng, mPrivClient, mPublicClient);
            mpInstance = &theInstance;
        }

        inline static void onDeadReference()
        {
            create();
            new (mpInstance) KeyHolder;
            #ifdef ATEXIT_FIXED
            std::atexit(killKeyHolder);
            #endif
            mDestroyed = false;
        }

        inline static KeyHolder* mpInstance = nullptr;
        inline static bool mDestroyed       = false;
      
        inline static CryptoPP::AutoSeededRandomPool mRng{};
        inline static CryptoPP::OID mEllipticCurve{CryptoPP::ASN1::secp256r1()};
        inline static CryptoPP::ECDH<CryptoPP::ECP>::Domain mDHClient{mEllipticCurve};
        inline static CryptoPP::SecByteBlock mPublicClient{mDHClient.PublicKeyLength()};
        inline static CryptoPP::SecByteBlock mPrivClient{mDHClient.PrivateKeyLength()};

        inline static CryptoPP::SecByteBlock mPublicServer{};

        inline static CryptoPP::SecByteBlock mSharedSecret{};
    };

}
