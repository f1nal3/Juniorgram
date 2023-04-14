#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include "Utility/Utility.hpp"

namespace Base
{
	using Models::RSAKeyPair;
	using CryptoPP::AutoSeededRandomPool;
	using CryptoPP::RSA;

class ByteBlockGenerator
{
public:
    RSAKeyPair generateRSAKeyPair()
    {
        RSAKeyPair           rsaKeyPair;
        AutoSeededRandomPool randPool;

        rsaKeyPair.privateKey.GenerateRandomWithKeySize(randPool, 3072);
        rsaKeyPair.publicKey = RSA::PublicKey(rsaKeyPair.privateKey);

        return rsaKeyPair;
    }

	RSA::PublicKey generateRSAPublicKey(const RSA::PrivateKey& privateKey) { return RSA::PublicKey(privateKey); };
};
}
