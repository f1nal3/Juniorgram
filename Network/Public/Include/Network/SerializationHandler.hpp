#pragma once
#include "Handler.hpp"

class SerializationHandler : public AbstractHandler
{
public:
    void handleOutcomingMessage(const Network::Message& message, yas::shared_buffer& headerBuffer,
                       yas::shared_buffer& bodyBuffer) override
    {
        Network::Message::MessageHeader messageHeader = message.mHeader;

        if (message.mBody.has_value())
        {
            switch (message.mHeader.mConnectionID)
            {
                case Network::Message::MessageType::ServerAccept:
                    break;
                case Network::Message::MessageType::ServerPing:
                    break;
                case Network::Message::MessageType::MessageAll:
                    break;
                case Network::Message::MessageType::ServerMessage:
                    break;
                case Network::Message::MessageType::ChannelListRequest:
                    Network::YasSerializer::serialize<std::vector<Network::ChannelInfo>>(
                        bodyBuffer, std::any_cast<std::vector<Network::ChannelInfo>>(
                                        message.mBody));
                    break;
                case Network::Message::MessageType::MessageHistoryRequest:
                    Network::YasSerializer::serialize<std::vector<Network::MessageInfo>>(
                        bodyBuffer,
                        std::any_cast<std::vector<Network::MessageInfo>>(message.mBody));
                    break;
                case Network::Message::MessageType::MessageStoreRequest:
                    Network::YasSerializer::serialize<Network::MessageInfo>(
                        bodyBuffer,
                        std::any_cast<Network::MessageInfo>(message.mBody));
                    break;
                default:
                    break;
            }

            messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        }

        Network::YasSerializer::serialize<Network::Message::MessageHeader>(headerBuffer,
                                                                           messageHeader);
        
        if (!this->nextHandler)
        {
            AbstractHandler::handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                    Network::Message::MessageHeader& messageHeader) override
    {
        Network::YasSerializer::deserialize<Network::Message::MessageHeader>(buffer, messageHeader);

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    void handleIncomingMessageBody(const yas::shared_buffer buffer,
                                   Network::Message& message) override
    {
        switch (message.mHeader.mConnectionID)
        {
            case Network::Message::MessageType::ServerAccept :
                break;
            case Network::Message::MessageType::ServerPing:
                break;
            case Network::Message::MessageType::MessageAll:
                break;
            case Network::Message::MessageType::ServerMessage:
                break;
            case Network::Message::MessageType::ChannelListRequest:
            {
                std::vector<Network::ChannelInfo> channelInfo;
                Network::YasSerializer::deserialize<std::vector<Network::ChannelInfo>>(buffer,
                                                                                       channelInfo);
                message.mBody =
                    std::make_any<std::vector<Network::ChannelInfo>>(channelInfo);
                break;
            }
            case Network::Message::MessageType::MessageHistoryRequest:
            {
                std::vector<Network::MessageInfo> messageInfo;
                Network::YasSerializer::deserialize<std::vector<Network::MessageInfo>>(buffer,
                                                                                       messageInfo);
                message.mBody =
                    std::make_any<std::vector<Network::MessageInfo>>(messageInfo);
                break;
            }
            case Network::Message::MessageType::MessageStoreRequest:
                break;
            default:
                break;
        }

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
