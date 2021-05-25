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
    void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
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
                    YasSerializer::template serialize<std::vector<ChannelInfo>>(
                        bodyBuffer, std::any_cast<std::vector<ChannelInfo>>(message.mBody));
                    break;
                case Message::MessageType::MessageHistoryRequest:
                    YasSerializer::template serialize<std::vector<MessageInfo>>(
                        bodyBuffer, std::any_cast<std::vector<MessageInfo>>(message.mBody));
                    break;
                case Message::MessageType::MessageStoreRequest:
                    YasSerializer::template serialize<MessageInfo>(
                        bodyBuffer, std::any_cast<MessageInfo>(message.mBody));
                    break;
                default:
                    break;
            }

            messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        }

        YasSerializer::template serialize<Message::MessageHeader>(headerBuffer, messageHeader);

        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    /**
     * @brief Method for deserialization of incoming message headers.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message header data.
     */
    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        YasSerializer::template deserialize<Message::MessageHeader>(buffer, messageHeader);

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    /**
     * @brief Method for deserialization of incoming message bodies.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message body.
     */
    void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
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
            {
                std::vector<ChannelInfo> channelInfo;
                YasSerializer::template deserialize<std::vector<ChannelInfo>>(buffer, channelInfo);
                message.mBody = std::make_any<std::vector<ChannelInfo>>(channelInfo);
                break;
            }
            case Message::MessageType::MessageHistoryRequest:
            {
                std::vector<MessageInfo> messageInfo;
                YasSerializer::template deserialize<std::vector<MessageInfo>>(buffer, messageInfo);
                message.mBody = std::make_any<std::vector<MessageInfo>>(messageInfo);
                break;
            }
            case Message::MessageType::MessageStoreRequest:
            {
                MessageInfo messageInfo;
                YasSerializer::template deserialize<MessageInfo>(buffer, messageInfo);
                message.mBody = std::make_any<MessageInfo>(messageInfo);
                break;
            }
            default:
                break;
        }

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
}  // namespace Network
