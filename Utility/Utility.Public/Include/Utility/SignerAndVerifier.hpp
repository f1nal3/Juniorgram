#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>

namespace Utility
{

class SignerAndVerifier
{
public:
    static SignerAndVerifier& Instance()
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

    inline static void initPoint(const CryptoPP::SecByteBlock& privKey)
    { 
        mPoint = CryptoPP::Integer(privKey.data(), privKey.size());
    }

    inline static void initAndSavePrivateAndPublicKey()
    {
        mPrivKey.Initialize(mEllipticCurve, mPoint);
        mPrivKey.Save(mPrivateKey);

        mPrivKey.MakePublicKey(mPubKey);
        mPubKey.Save(mPublicKey);
    }

    inline static void loadPrivateKey()
    { 
         // Load private key (in ByteQueue, PKCS#8 format)
        mSigner = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer(mPrivateKey);
    }

    inline static void loadPublicKey()
    { 
         mVerifier = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier(mPublicKey);
    }

    inline static std::string sign(const std::string& data)
    {
        // Determine maximum size, allocate a string with that size
        size_t siglen = mSigner.MaxSignatureLength();
        std::string signature = std::string(siglen, 0x00);
        
        // Sign, and trim signature to actual size
        std::size_t sigLen = mSigner.SignMessage(mRng, (const CryptoPP::byte*)data.data(), data.size(),
                                      (CryptoPP::byte*)signature.data());
        signature.resize(sigLen);

        return signature;
    }

    inline static bool verify(const std::string& data, const std::string& signature)
    {
        bool result = mVerifier.VerifyMessage((const CryptoPP::byte*)data.data(), data.size(),
                                    (const CryptoPP::byte*)signature.data(), signature.size());
        if (result)
            std::cout << "Verified signature on message.\n";
        else
            std::cout << "Failed to verify signature on message!\n";
        
        return result;
    }

    virtual ~SignerAndVerifier()
    {
        mpInstance = nullptr;
        mDestroyed = true;
    }

    inline static void killSignerAndVerifier() { mpInstance->~SignerAndVerifier(); }

private:
    inline static void create()
    {
        static SignerAndVerifier theInstance;
        mpInstance = &theInstance;
    }

    inline static void onDeadReference()
    {
        create();
        new (mpInstance) SignerAndVerifier;
        #ifdef ATEXIT_FIXED
        std::atexit(killSignerAndVerifier);
        #endif
        mDestroyed = false;
    }

    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey mPrivKey{};
    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey mPubKey{};

    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer mSigner{};
    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier mVerifier{};

    inline static CryptoPP::Integer mPoint{};

    inline static SignerAndVerifier* mpInstance = nullptr;
    inline static bool mDestroyed       = false;
    inline static CryptoPP::ByteQueue mPrivateKey{}, mPublicKey{};
    inline static CryptoPP::AutoSeededRandomPool mRng{};
    inline static CryptoPP::OID mEllipticCurve{CryptoPP::ASN1::secp256r1()};
    
    /*inline static std::string mSignature{};

    inline static std::size_t mSigLen{};*/
};
}
