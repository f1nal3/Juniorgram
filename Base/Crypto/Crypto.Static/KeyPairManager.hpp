#pragma once

#include <string>
#include <fstream>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include "ByteBlockGenerator.hpp"
#include "Models/Primitives.hpp"
#include "Utility/JGExceptions.hpp"

namespace Base
{
using Base::Logger::FileLogger;
using CryptoPP::StringSink;
using CryptoPP::FileSink;
using CryptoPP::FileSource;

class KeyPairManager
{
public:
    KeyPairManager() { loadPrivateKey(); };

    void loadPrivateKey()
    {
        std::ifstream privateKeyOutputFile(PRIVATE_KEY_FILE);
        if (privateKeyOutputFile.is_open())
        {
            FileSource privateKeySource(privateKeyOutputFile, true);
            _keyPair.privateKey.BERDecode(privateKeySource);
            _privateKeyPlaced = true;
        }
        privateKeyOutputFile.close();

        if (_privateKeyPlaced)
        {
            if (AutoSeededRandomPool randPool; _keyPair.privateKey.Validate(randPool, ValidationLevel::HARD))
            {
                loadPublicKey();
            }
            else
            {
                FileLogger::getInstance().log("Invalid (probably compromised) private key", Base::Logger::LogLevel::WARNING);
                _keyPair = _generator->generateRSAKeyPair();
            }
        }
        else
        {
            FileLogger::getInstance().log("Private key not founded", Base::Logger::LogLevel::INFO);
            _keyPair = _generator->generateRSAKeyPair();
        }
    }

    std::string getPublicServerKey()
    {
        std::string publicKeyStr;
        StringSink  stringSink(publicKeyStr);
        _keyPair.publicKey.DEREncode(stringSink);
        return publicKeyStr;

        /* std::string rsaPublicStr;
        RSA::PublicKey rsaPublic;
        StringSource    stringSource(rsaPublicStr, true);
        rsaPublic.BERDecode(stringSource);*/
    };

private:
    const std::string  PRIVATE_KEY_FILE{"juniorgram.rsa"};
    const std::string  PUBLIC_KEY_FILE = {PRIVATE_KEY_FILE + ".pub"};
    RSAKeyPair _keyPair;
    std::unique_ptr<ByteBlockGenerator> _generator;
    bool               _privateKeyPlaced = false;
    bool               _publicKeyPlaced  = false;

    /// @detail for more info: https://www.cryptopp.com/wiki/Keys_and_Formats#Validating_Keys
    enum ValidationLevel : unsigned int
    {
        TRIFLING = 0,
        MINOR,
        BALANCED,
        HARD
    };

    void loadPublicKey()
    {
        std::ifstream publicKeyOutputFile(PUBLIC_KEY_FILE);
        if (publicKeyOutputFile.is_open())
        {
            FileSource publicKeySource(publicKeyOutputFile, true);
            _keyPair.publicKey.BERDecode(publicKeySource);
            _publicKeyPlaced = true;
        }
        publicKeyOutputFile.close();
        _publicKeyPlaced = true;
    }

    void savePrivateKey()
    {
        std::ofstream privateKeyInputFile(PRIVATE_KEY_FILE);
        if (privateKeyInputFile.is_open())
        {
            FileSink privateKeySink(privateKeyInputFile /*ios::trunc*/);
            _keyPair.privateKey.DEREncode(privateKeySink);
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log("Failure to save private key to a file ", Base::Logger::LogLevel::ERR);
        }
        privateKeyInputFile.close();
    }

    void savePublicKey()
    {
        //todo
    }

};
}  // namespace Base