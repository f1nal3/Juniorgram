#pragma once
#include "Handler.hpp"

namespace Network
{
/** 
* @class EncryptionHandler
* @brief handler class for messages encryption.
*/
class EncryptionHandler : public AbstractHandler
{
public:
    /**
    * @brief Method for encryption of outcoming messages.
    * @param source - variable that contains data that should be encrypted.
    * @param destination - buffer that will contain encrypted message body.
    */
    MessageProcessingState handleOutcomingMessage(const Message& source, yas::shared_buffer& destination) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(source, destination);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
    * @brief Method for decryption of incoming message bodies.
    * @param source - buffer that contains data that should be decrypted.
    * @param destination - variable that will contain decrypted message body.
    */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer& source, Message& destination) override
    {
        /// body decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(source, destination);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  /// namespace Network
