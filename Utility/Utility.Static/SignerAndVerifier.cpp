#include "SignerAndVerifier.hpp"

namespace Utility
{
    SignerAndVerifier& SignerAndVerifier::Instance()
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

    void SignerAndVerifier::initPoint(const CryptoPP::SecByteBlock& privKey)
    {
        mPoint = CryptoPP::Integer(privKey.data(), privKey.size());
    }

    void SignerAndVerifier::initAndSavePrivateAndPublicKey()
    {
 
        mPrivKey.Initialize(mEllipticCurve, mPoint);
        mPrivKey.Save(mPrivateKey);

        mPrivKey.MakePublicKey(mPubKey);
        mPubKey.Save(mPublicKey);

        
        std::string publickey;
        CryptoPP::StringSink ss(publickey);
        mPubKey.Save(ss);

        std::string privatekey;
        CryptoPP::StringSink ssp(privatekey);
        mPrivKey.Save(ssp);

        /*CryptoPP::StringSource ss(key, true);
        mPrivKey.BERDecode(ss);*/

        std::cout << mPrivateKey.CurrentSize();
       CryptoPP::byte* bts = new CryptoPP::byte[mPrivateKey.MaxRetrievable()];

       std::cout << mPrivateKey.MaxRetrievable();

        mPrivateKey.Peek(bts, mPrivateKey.MaxRetrievable());

    }

    void SignerAndVerifier::loadPrivateKey()
    {
        // Load private key (in ByteQueue, PKCS#8 format)
        mSigner = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer(mPrivateKey);
    }

    void SignerAndVerifier::loadPublicKey()
    {
        mVerifier = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier(mPublicKey);
    }

    std::string SignerAndVerifier::sign(const std::string& data)
    {
        // Determine maximum size, allocate a string with that size
        size_t siglen         = mSigner.MaxSignatureLength();
        std::string signature = std::string(siglen, 0x00);

        // Sign, and trim signature to actual size
        std::size_t sigLen = mSigner.SignMessage(mRng, (const CryptoPP::byte*)data.data(),
                                                 data.size(), (CryptoPP::byte*)signature.data());
        signature.resize(sigLen);

        return signature;
    }

    bool SignerAndVerifier::verify(const std::string& data, const std::string& signature)
    {
        bool result =
            mVerifier.VerifyMessage((const CryptoPP::byte*)data.data(), data.size(),
                                    (const CryptoPP::byte*)signature.data(), signature.size());
        if (result)
            std::cout << "Verified signature on message.\n";
        else
            std::cout << "Failed to verify signature on message!\n";

        return result;
    }

    SignerAndVerifier::~SignerAndVerifier()
    {
        mpInstance = nullptr;
        mDestroyed = true;
    }

    void SignerAndVerifier::killSignerAndVerifier()
    {
        mpInstance->~SignerAndVerifier();
    }

    void SignerAndVerifier::create()
    {
        static SignerAndVerifier theInstance;
        mpInstance = &theInstance;
    }

    void SignerAndVerifier::onDeadReference()
    {
        create();
        new (mpInstance) SignerAndVerifier;
        #ifdef ATEXIT_FIXED
        std::atexit(killSignerAndVerifier);
        #endif
        mDestroyed = false;
    }

}
