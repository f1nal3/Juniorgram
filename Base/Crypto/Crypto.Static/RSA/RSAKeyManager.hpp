#pragma once

#include <fstream>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include "RSAKeyGenerator.hpp"

namespace Base
{
using Base::Logger::FileLogger;
using Base::Crypto::Primitives::RSAKeyPair;
using Utility::GeneralCodes;
using Generators::RSAKeyGenerator;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::StringSink;
using CryptoPP::FileSink;
using CryptoPP::FileSource;

/** @class RSAKeyManager
* @brief Class for managing RSA keys
* @details Provide saving, loading keys to/from strong specified file; contains long-term key pair.
* @warning That's why code for saving and loading keys is a bit strange (about using fstream only to verify the existence of a file):
* by trial and error I found out, that using FileSource(ifstream) throws error in BERDecode
* (why so, I do not know; I found only one example of such use, and the person couldn't get it to work:
* https://stackoverflow.com/questions/58493934/how-to-sign-a-file-via-crypto-and-rsa). Using FileSink(ofstream) does not cause
* problems, but if you call FileSource(c-style filename) for reading file, which was be saved with FileSink(ofstream), you will get
* BERDecode error too. FileSource does not allow to check the existence of the file (error will be thrown inside the library),
* so for first launch you should check file existence.
*/
class RSAKeyManager
{
public:
    RSAKeyManager() = default;

    /** @brief Constructor that allows you to change the name of the file in which the keys will be saved
     * @param filename Name of file for private RSA key; filename for public key is filename + ".pub"
     */
    RSAKeyManager(const std::string& filename)
    {
        PRIVATE_KEY_FILE = filename;
        PUBLIC_KEY_FILE  = filename + ".pub";
    };

    /** @brief Method for getting public server key
    * @return public server key as std::string
    */
    std::string getPublicServerKeyStr() { return _keyPair.getPublicKeyAsString(); };

    /// @brief Method for loading key pair
    void loadKeyPair() { loadPrivateKey(); };

    /** @brief Method for saving rsa keys in files
     * @details Work steps: \
     * 1. Save private key. If saving the private key fails, the public key is not saved too. \
     * 2. Save public key.
     */
    void saveKeyPair()
    {
        if (savePrivateKey() == GeneralCodes::SUCCESS)
        {
            savePublicKey();
        }
    };

    /// @brief Getter for private server key
    CryptoPP::RSA::PrivateKey getPrivateKey() { return _keyPair.privateKey; };

private:
    /// Contains public and private keys
    RSAKeyPair                          _keyPair;
    /// Name of file to save private RSA key
    std::string                         PRIVATE_KEY_FILE{"juniorgram.rsa"};
    /// Name of file to save public RSA key
    std::string                         PUBLIC_KEY_FILE{"juniorgram.rsa.pub"};

    /** @enum ValidationLevel
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

    /** @brief Method for loading private key from a file
    * @details Method loads private RSA key from file "PRIVATE_KEY_FILE". The following situations are possible: \
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
            privateKeyOutputFile.close();

            FileSource privateKeySource(PRIVATE_KEY_FILE.c_str(), true);
            _keyPair.privateKey.BERDecode(privateKeySource);
            privateKeyLoaded = true;
        }
        privateKeyOutputFile.close();

        if (privateKeyLoaded)
        {
            if (AutoSeededRandomPool randPool; _keyPair.privateKey.Validate(randPool, ValidationLevel::HARD))
            {
                FileLogger::getInstance().log("[SERVER] Private key loaded from file successfully",
                                                            Base::Logger::LogLevel::INFO);
                loadPublicKey();
            }
            else
            {
                FileLogger::getInstance().log("[SERVER] Invalid (probably compromised) private key", Base::Logger::LogLevel::WARNING);
                _keyPair = RSAKeyGenerator().generateRSAKeyPair();
            }
        }
        else
        {
            FileLogger::getInstance().log("[SERVER] Private key not found", Base::Logger::LogLevel::INFO);
            _keyPair = RSAKeyGenerator().generateRSAKeyPair();
        }
    }

     /** @brief Method for loading public key from a file
     * @details Method loads public RSA key from file "PUBLIC_KEY_FILE". The following situations are possible: \
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
            publicKeyOutputFile.close();

            FileSource publicKeySource(PUBLIC_KEY_FILE.c_str(), true);
            _keyPair.publicKey.BERDecode(publicKeySource);
            publicKeyLoaded = true;
        }

        if (AutoSeededRandomPool randPool;
            !publicKeyLoaded || !_keyPair.publicKey.Validate(randPool, ValidationLevel::BALANCED))
        {
            FileLogger::getInstance().log("[SERVER] Public key not found or invalid", Base::Logger::LogLevel::INFO);
            _keyPair.publicKey = RSAKeyGenerator().generateRSAPublicKey(_keyPair.privateKey);
        }
        else
        {
            FileLogger::getInstance().log("[SERVER] Public key loaded from file successfully", Base::Logger::LogLevel::INFO);
        }
        publicKeyOutputFile.close();
    }

    /** @brief Method for saving private RSA key in file "PRIVATE_KEY_FILE"
    * @details Saves the private key by completely overwriting the file. Failed private key saving does not
    * startup public key saving.
    */
    GeneralCodes savePrivateKey()
    {
        std::ofstream privateKeyInputFile(PRIVATE_KEY_FILE);
        if (privateKeyInputFile.is_open())
        {
            privateKeyInputFile.close();

            FileSink privateKeySink(PRIVATE_KEY_FILE.c_str() /*ios::trunc*/);
            _keyPair.privateKey.DEREncode(privateKeySink);
            FileLogger::getInstance().log("[SERVER] Private key saved in file successfully", Base::Logger::LogLevel::INFO);
        }
        else
        {
            FileLogger::getInstance().log(
                "[SERVER] Failure to save private key to a file, so the public key will not be saved too.", Base::Logger::LogLevel::ERR);
            return GeneralCodes::FAILED;
        }
        privateKeyInputFile.close();
        return GeneralCodes::SUCCESS;
    }

    /** @brief Method for saving public RSA key in file "PUBLIC_KEY_FILE"
     * @details Saves the public key by completely overwriting the file. Failed public key saving does nothing.
     */
    void savePublicKey()
    {
        std::ofstream publicKeyInputFile(PUBLIC_KEY_FILE);
        if (publicKeyInputFile.is_open())
        {
            publicKeyInputFile.close();

            FileSink publicKeySink(PUBLIC_KEY_FILE.c_str() /*ios::trunc*/);
            _keyPair.publicKey.DEREncode(publicKeySink);
            FileLogger::getInstance().log("[SERVER] Public key saved in file successfully", Base::Logger::LogLevel::INFO);
        }
        else
        {
            FileLogger::getInstance().log("[SERVER] The public key could not be saved to a file", Base::Logger::LogLevel::ERR);
        }
        publicKeyInputFile.close();
    }
};
}  // namespace Base
