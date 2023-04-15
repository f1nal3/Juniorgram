#pragma once

#include <string>
#include <fstream>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include "RSAKeyGenerator.hpp"
#include "Models/Primitives.hpp"

namespace Base
{
using Base::Logger::FileLogger;
using Utility::GeneralCodes;
using CryptoPP::StringSink;
using CryptoPP::FileSink;
using CryptoPP::FileSource;

/* @class KeyPairManager
* @brief Class for managing RSA keys
* @details Provide saving, loading keys to/from strong specified file; contains key pair.
*/
class KeyPairManager
{
public:
    KeyPairManager() { loadKeyPair(); };

    /* @brief Method for getting public server key
    * @return public server key as std::string
    */
    std::string getPublicServerKey()
    {
        std::string publicKeyStr;
        StringSink  stringSink(publicKeyStr);
        _keyPair.publicKey.DEREncode(stringSink);
        return publicKeyStr;
    };

private:
    /// Contains public and private keys
    RSAKeyPair                          _keyPair;
    /// Name of file to save private RSA key
    const std::string                   PRIVATE_KEY_FILE{"juniorgram.rsa"};
    /// Name of file to save public RSA key
    const std::string                   PUBLIC_KEY_FILE{PRIVATE_KEY_FILE + ".pub"};

    /* @enum ValidationLevel
    * @details Determines, which checks will be performed by CryptoPP library. \
    * For more info: https://www.cryptopp.com/wiki/Keys_and_Formats#Validating_Keys
    */
    enum ValidationLevel : uint32_t
    {
        TRIFLING = 0,
        MINOR,
        BALANCED,
        HARD
    };

    /// @brief Method for loading key pair
    void loadKeyPair() { loadPrivateKey(); };

    /* @brief Method for loading private key from a file
    * @details Method loads private RSA key from file "PRIVATE_KEY_FILE". The following situations are possible:
    * - File not found and private key not loaded -> generate new key pair \
    * - Private key loaded, but invalid -> generate new key pair \
    * - Private key loaded and valid -> load public key
    */
    inline void loadPrivateKey()
    {
        bool privateKeyLoaded = false;

        std::ifstream privateKeyOutputFile(PRIVATE_KEY_FILE);
        if (privateKeyOutputFile.is_open())
        {
            FileSource privateKeySource(privateKeyOutputFile, true);
            _keyPair.privateKey.BERDecode(privateKeySource);
            privateKeyLoaded = true;
        }
        privateKeyOutputFile.close();

        if (privateKeyLoaded)
        {
            if (AutoSeededRandomPool randPool; _keyPair.privateKey.Validate(randPool, ValidationLevel::HARD))
            {
                loadPublicKey();
            }
            else
            {
                FileLogger::getInstance().log("Invalid (probably compromised) private key", Base::Logger::LogLevel::WARNING);
                _keyPair = RSAKeyGenerator().generateRSAKeyPair();
            }
        }
        else
        {
            FileLogger::getInstance().log("Private key not founded", Base::Logger::LogLevel::INFO);
            _keyPair = RSAKeyGenerator().generateRSAKeyPair();
        }
    }

     /* @brief Method for loading public key from a file
     * @details Method loads public RSA key from file "PUBLIC_KEY_FILE". The following situations are possible:
     * - Public key not loaded  -> generate a public key based on the uploaded private key  \
     * - File not found -> generate a public key based on the uploaded private key  \
     * - Public key loaded and valid -> end of key loading
     */
    inline void loadPublicKey()
    {
        bool publicKeyLoaded = false;

        std::ifstream publicKeyOutputFile(PUBLIC_KEY_FILE);
        if (publicKeyOutputFile.is_open())
        {
            FileSource publicKeySource(publicKeyOutputFile, true);
            _keyPair.publicKey.BERDecode(publicKeySource);
            publicKeyLoaded = true;
        }

        if (AutoSeededRandomPool randPool;
            !publicKeyLoaded || !_keyPair.publicKey.Validate(randPool, ValidationLevel::BALANCED))
        {
            FileLogger::getInstance().log("Public key not founded or invalid", Base::Logger::LogLevel::INFO);
            _keyPair.publicKey = RSAKeyGenerator().generateRSAPublicKey(_keyPair.privateKey);
        }
        publicKeyOutputFile.close();
    }

    /* @brief Method for saving rsa keys in files
    * @details Work steps:
    * 1. Save private key. If saving the private key fails, the public key is not saved too.
    * 2. Save public key.
    */
    void saveKeyPair()
    {
        if (savePrivateKey() == GeneralCodes::SUCCESS)
        {
            savePublicKey();
        }
    };
    /* @brief Method for saving private RSA key in file "PRIVATE_KEY_FILE"
    * @details Saves the private key by completely overwriting the file. Failed private key saving does not
    * startup public key saving.
    */
    GeneralCodes savePrivateKey()
    {
        std::ofstream privateKeyInputFile(PRIVATE_KEY_FILE);
        if (privateKeyInputFile.is_open())
        {
            FileSink privateKeySink(privateKeyInputFile /*ios::trunc*/);
            _keyPair.privateKey.DEREncode(privateKeySink);
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log(
                "Failure to save private key to a file, so the public key will not be saved too.",
                Base::Logger::LogLevel::ERR);
            return GeneralCodes::FAILED;
        }
        privateKeyInputFile.close();
        return GeneralCodes::SUCCESS;
    }

    /* @brief Method for saving public RSA key in file "PUBLIC_KEY_FILE"
     * @details Saves the public key by completely overwriting the file. Failed public key saving does nothing.
     */
    void savePublicKey()
    {
        std::ofstream publicKeyInputFile(PUBLIC_KEY_FILE);
        if (publicKeyInputFile.is_open())
        {
            FileSink publicKeySink(publicKeyInputFile /*ios::trunc*/);
            _keyPair.publicKey.DEREncode(publicKeySink);
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log(
                "The public key could not be saved to a file", Base::Logger::LogLevel::ERR);
        }
        publicKeyInputFile.close();
    }
};
}  // namespace Base
