#pragma once
#include "Handler.hpp"

namespace Network
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
     * @param headerBuffer - buffer that will contain serialized header.
     * @param bodyBuffer - buffer that will contain serialized body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message,
                                             yas::shared_buffer& headerBuffer,
                                             yas::shared_buffer& bodyBuffer) override
    {
        SerializedState state                = SerializedState::SUCCESS;
        Message::MessageHeader messageHeader = message.mHeader;

        if (message.mBody.has_value())
        {
            switch (message.mHeader.mMessageType)
            {
                case Message::MessageType::ServerAccept:
                    break;
                case Message::MessageType::ServerPing:
                    break;
                case Message::MessageType::MessageAll:
                    break;
                case Message::MessageType::ServerMessage:
                    break;
                case Message::MessageType::ChannelListRequest:
                    state = processOutcomingMessageBody<std::vector<ChannelInfo>>(bodyBuffer, message.mBody);
                    break;
                case Message::MessageType::MessageHistoryRequest:
                    state = processOutcomingMessageBody<std::vector<MessageInfo>>(bodyBuffer,
                                                                                  message.mBody);
                    break;
                case Message::MessageType::MessageStoreRequest:
                    state = processOutcomingMessageBody<MessageInfo>(bodyBuffer, message.mBody);
                    break;
                case Message::MessageType::MessageStoreRequest:
                    state = processOutcomingMessageBody<RegistrationInfo>(bodyBuffer, 
                                                                          message.mBody);
                default:
                    break;
            }

            if (state == SerializedState::SUCCESS)
            {
                messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
            }
        }

        if (state == SerializedState::SUCCESS)
        {
            YasSerializer::template serialize<Message::MessageHeader>(headerBuffer, messageHeader);

            if (this->nextHandler)
            {
                this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
            }
            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

    /**
     * @brief Method for deserialization of incoming message headers.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message header data.
     */
    MessageProcessingState handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                                  Message::MessageHeader& messageHeader) override
    {
        SerializedState state = YasSerializer::template deserialize<Message::MessageHeader>(buffer, messageHeader);

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
            }

            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

    /**
     * @brief Method for deserialization of incoming message bodies.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer,
                                                Message& message) override
    {
        SerializedState state = SerializedState::FAILURE;

        switch (message.mHeader.mMessageType)
        {
            case Message::MessageType::ServerAccept:
                break;
            case Message::MessageType::ServerPing:
                break;
            case Message::MessageType::MessageAll:
                break;
            case Message::MessageType::ServerMessage:
                break;
            case Message::MessageType::ChannelListRequest:
            {
                state = processIncomingMessageBody<std::vector<ChannelInfo>>(buffer, message);
                break;
            }
            case Message::MessageType::MessageHistoryRequest:
            {
                state = processIncomingMessageBody<std::vector<MessageInfo>>(buffer, message);
                break;
            }
            case Message::MessageType::MessageStoreRequest:
            {
                state = processIncomingMessageBody<MessageInfo>(buffer, message);
                break;
            }
            case Message::MessageType::RegistrationRequest:
            {
                state = processIncomingMessageBody<RegistrationInfo>(buffer, message);
                break;
            }
            default:
                break;
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleIncomingMessageBody(buffer, message);
            }
            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

private:
    template <typename T>
    SerializedState processOutcomingMessageBody(yas::shared_buffer& bodyBuffer, const std::any messageBody)
    {
        try
        {
            return YasSerializer::template serialize<T>(bodyBuffer, std::any_cast<T>(messageBody));
        }
        catch (const std::bad_any_cast& e)
        {
            std::cout << e.what() << '\n';
            std::cout << "Message body cann't be serialized\n";

            return SerializedState::FAILURE;
        }
    }

    template <typename T>
    SerializedState processIncomingMessageBody(const yas::shared_buffer& bodyBuffer,
                                               Message& message)
    {
        try
        {
            T messageInfo;

            SerializedState state = YasSerializer::template deserialize<T>(bodyBuffer, messageInfo);

            if (state == SerializedState::SUCCESS)
            {
                message.mBody = std::make_any<T>(messageInfo);
            }

            return state;
        }
        catch (const std::bad_any_cast& e)
        {
            std::cout << e.what() << '\n';
            std::cout << "Message body cann't be deserialized\n";
            
            return SerializedState::FAILURE;
        }
    }
};
}  // namespace Network
