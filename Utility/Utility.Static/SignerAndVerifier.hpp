#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>

namespace Utility
{

class SignerAndVerifier
{
public:
   /* static SignerAndVerifier& Instance();*/

    SignerAndVerifier() = default;

    SignerAndVerifier(const SignerAndVerifier& other) = delete;
    SignerAndVerifier& operator=(const SignerAndVerifier& other) = delete;

    SignerAndVerifier(SignerAndVerifier&& other) = delete;
    SignerAndVerifier& operator=(SignerAndVerifier&& other) = delete;

    static void initPoint(const CryptoPP::SecByteBlock& privKey);

    static void initPrivateAndPublicKey();

    static void loadPrivateKey();

    static void loadPublicKey();

    static std::string getPublicKey();

    static std::string getPrivateKey();

    static std::string sign(const std::string& data);

    static bool verify(const std::string& data, const std::string& signature);

    virtual ~SignerAndVerifier();

    static void killSignerAndVerifier();

private:
    //static void create();
    //    
    //static void onDeadReference();

    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey mPrivKey{};
    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey mPubKey{};

    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer mSigner{};
    inline static CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier mVerifier{};

    inline static CryptoPP::Integer mPoint{};

    //inline static SignerAndVerifier* mpInstance = nullptr;
    inline static bool mDestroyed       = false;
    inline static CryptoPP::AutoSeededRandomPool mRng{};
    inline static CryptoPP::OID mEllipticCurve{CryptoPP::ASN1::secp256r1()};
};
}
