#pragma once

namespace Base::Crypto::Primitives
{
struct RSAKeyPair
{
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::RSA::PrivateKey privateKey;

    std::string getPublicKeyAsString()
    {
        std::string publicKeyStr;
        CryptoPP::StringSink stringSink(publicKeyStr);
        publicKey.DEREncode(stringSink);
        return publicKeyStr;
    }

    static CryptoPP::RSA::PublicKey getPublicKeyFromString(const std::string& publicServerKeyStr)
    {
        CryptoPP::StringSource stringSource(publicServerKeyStr, true);
        CryptoPP::RSA::PublicKey publicServerKey;
        publicServerKey.BERDecode(stringSource);
        return publicServerKey;
    }
};
}  // namespace Base::Crypto::Primitives
