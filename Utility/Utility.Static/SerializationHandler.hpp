#pragma once

#include <iostream>
#include <any>
#include <type_traits>

#include "Handler.hpp"

namespace Utility
{
/** @class SerializationHandler
 *  @brief handler class for messages serialization.
 */
class SerializationHandler : public AbstractHandler
{
public:

    /**
     * @brief Method for serialization of outcoming messages.
     * @param message - buffer that contains data that should be serialized.
     * @param bodyBuffer - buffer that will contain serialized body.
     */
    MessageProcessingState handleOutcomingMessage(Network::Message& message, yas::shared_buffer& bodyBuffer) override;
    
    /**
     * @brief Method for deserialization of incoming message bodies.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                     Network::Message& message) override;
    

private:

    template <typename T, typename U>
    SerializedState processOutcomingMessageBody(yas::shared_buffer& bodyBuffer,
                                                const std::any messageBody);

    template <typename T>
    SerializedState processOutcomingMessageBody(yas::shared_buffer& bodyBuffer,
                                                const std::any messageBody);

    template <typename T>
    SerializedState processIncomingMessageBody(yas::shared_buffer& bodyBuffer,
                                               Network::Message& message);

    template <typename T, typename U>
    SerializedState processIncomingMessageBody(yas::shared_buffer& bodyBuffer,
                                               Network::Message& message);
};
}  // namespace Network
