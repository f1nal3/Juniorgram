#pragma once
#include "Handler.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>

namespace Network
{
/** @class EncryptionHandler
 *  @brief handler class for messages encryption.
 */
class EncryptionHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for encryption of outcoming messages.
     * @param message - buffer that contains data that should be encrypted.
     * @param headerBuffer - buffer that will contain encrypted header.
     * @param bodyBuffer - buffer that will contain encrypted body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message,
                                             yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body encryption
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header encryption
        std::vector<char> vc3;
        int l = 10;   

        for (size_t i = 0; i < bodyBuffer.size; i++)
        {
            vc3.push_back(bodyBuffer.data.get()[i]);
            l = 0;

            std::cout << bodyBuffer.data.get()[i];
        }
        l = 11;

        //std::cout << "\n\n";
        //std::cout << "Buffer length before encrypt - " << bodyBuffer.size << "\n\n\n\n\n";

        CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(mEncryptionKey, CryptoPP::AES::BLOCKSIZE, mIV);
       
   ////     suppressWarning(4996, init)
   /////*     char* ret = new char[bodyBuffer.size]{};
   ////     strcpy(ret, bodyBuffer.data.get());
   ////     restoreWarning*/

        cfbEncryption.ProcessData((CryptoPP::byte*)bodyBuffer.data.get(),
                                  (CryptoPP::byte*)bodyBuffer.data.get(),
                                  bodyBuffer.size /*(strlen(bodyBuffer.data.get())+1)*/);

         std::vector<char> vc4;
        
        std::cout << "\n\n\n\nShared buff after encrypt - ";

        for (size_t i = 0; i < bodyBuffer.size; i++)
        {
            vc4.push_back(bodyBuffer.data.get()[i]);

            std::cout << bodyBuffer.data.get()[i];
        }
        l = 11;

        std::cout << "\n\n";
        std::cout << "Buffer length after encrypt - " << bodyBuffer.size << "\n\n\n\n\n";


      /*  suppressWarning(4996, init) 
        char* retA = new char[bodyBuffer.size]{};
        strcpy(retA, bodyBuffer.data.get());
        restoreWarning

        std::cout << "After encrypt - " <<bodyBuffer.data.get() << "\n";
        std::cout << "Length of encrypt buf - " << strlen(bodyBuffer.data.get()) << "\n";*/


        if (this->nextHandler)
        {
            return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
     * @brief Method for decryption of incoming message bodies.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer,
                                                Message& message) override
    {
        // body decryption

        std::vector<char> vc;
        int l = 10;

        std::cout << "\n\n\n\nShared buff before decrypt - ";

        for (size_t i = 0; i < buffer.size; i++)
        {
            vc.push_back(buffer.data.get()[i]);
            l = 0;

            std::cout << buffer.data.get()[i];

        }
        l = 11;

        std::cout << "\n\n";
        std::cout << "Buffer length before dencrypt - " << buffer.size << "\n\n\n\n\n";

        CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(mEncryptionKey,
                                                                    CryptoPP::AES::BLOCKSIZE, mIV);
       
        cfbDecryption.ProcessData((CryptoPP::byte*)buffer.data.get(),
                                  (CryptoPP::byte*)buffer.data.get(),
                                  buffer.size/* (strlen(buffer.data.get()) + 1)*/);

        std::vector<char> vc1;
       
        std::cout << "\n\n\n\nShared buff after decrypt - ";

        for (size_t i = 0; i < buffer.size; i++)
        {
            vc1.push_back(buffer.data.get()[i]);

            std::cout << buffer.data.get()[i];
        }
        l = 11;

        std::cout << "\n\n";
        std::cout << "Buffer length after decrypt - " << buffer.size << "\n\n\n\n\n";

        if (this->nextHandler)
        {
          return this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }

    inline static void calculateDigest(const CryptoPP::SecByteBlock& sharedSecret)
    {
        CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());
    }

    inline static void setIVBlock(const std::string& IV)
    {
        for (size_t i = 0; i < IV.size(); ++i)
        {
            mIV[i] = IV[i];
        }
    }

    inline static std::string getIVBlock()
    {
        std::string IV;

        IV.resize(CryptoPP::AES::BLOCKSIZE);

        for (size_t i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i)
        {
            IV[i] = mIV[i];
        }

        return IV;
    }

    inline static void calculateDigestAndGenerateIVBlock(const CryptoPP::SecByteBlock& sharedSecret)
    {
        CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());

        // Generate a random IV
        mRng.GenerateBlock(mIV, CryptoPP::AES::BLOCKSIZE);
    }

private:
    inline static CryptoPP::byte mIV[CryptoPP::AES::BLOCKSIZE]{};
    inline static CryptoPP::AutoSeededRandomPool mRng{};
    inline static CryptoPP::SecByteBlock mEncryptionKey{CryptoPP::SHA256::DIGESTSIZE};
};
}  // namespace Network
