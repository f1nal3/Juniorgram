#include <Network.Static/Message.hpp>
#include <Network.Static/Primitives.hpp>

#include "TokenBuilder.hpp"
#include "SerializationHandler.hpp"


namespace Utility
{
MessageProcessingState SerializationHandler::handleOutcomingMessage(Network::Message& message,
                                                                        yas::shared_buffer& bodyBuffer)
{
    SerializedState state = SerializedState::SUCCESS;

    switch (message.mHeader.mMessageType)
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
            state = processOutcomingMessageBody<std::vector<Network::ChannelInfo>>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::MessageHistoryRequest:
            state = processOutcomingMessageBody<std::vector<Network::MessageInfo>>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::MessageStoreRequest:
            state = processOutcomingMessageBody<Network::MessageInfo>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::RevokeSession:
            state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::RegistrationRequest:
            state = processOutcomingMessageBody<Network::ClientPayload, Network::RegistrationInfo>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::RegistrationAnswer:
            state = processOutcomingMessageBody<Utility::AccessAndRefreshToken,Utility::RegistrationCodes>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::SendIV:
            state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::RegistrationRequestToClient:
            state =
                processOutcomingMessageBody<Utility::RegistrationCodes>(bodyBuffer, message.mBody);
            break;
        case Network::Message::MessageType::SetEncryptedConnection:
            state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
            break;
        default:
            break;
    }

    if (state == SerializedState::SUCCESS)
    {
        if (this->nextHandler)
        {
            return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }
    return MessageProcessingState::FAILURE;
    }

    MessageProcessingState SerializationHandler::handleIncomingMessageBody(yas::shared_buffer buffer, Network::Message& message)
    {
    SerializedState state = SerializedState::SUCCESS;

    switch (message.mHeader.mMessageType)
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
            state = processIncomingMessageBody<std::vector<Network::ChannelInfo>>(buffer, message);
            break;
        case Network::Message::MessageType::MessageHistoryRequest:
            state = processIncomingMessageBody<std::vector<Network::MessageInfo>>(buffer, message);
            break;
        case Network::Message::MessageType::MessageStoreRequest:
            state = processIncomingMessageBody<Network::MessageInfo>(buffer, message);
            break;
        case Network::Message::MessageType::RevokeSession:
            state = processIncomingMessageBody<std::string>(buffer, message);
            break;
        case Network::Message::MessageType::RegistrationRequest:
            state = processIncomingMessageBody<Network::ClientPayload, Network::RegistrationInfo>(buffer, message);  
            break;
        case Network::Message::MessageType::RegistrationAnswer:
            state = processIncomingMessageBody<Utility::AccessAndRefreshToken,Utility::RegistrationCodes>(buffer, message);
            break;
        case Network::Message::MessageType::RegistrationRequestToClient:
            state = processIncomingMessageBody<Utility::RegistrationCodes>(buffer, message);
            break;
        case Network::Message::MessageType::SendIV:
            state = processIncomingMessageBody<std::string>(buffer, message);
            break;
        case Network::Message::MessageType::SetEncryptedConnection:
            state = processIncomingMessageBody<std::string>(buffer, message);
            break;
        default:
            break;
    }

    if (state == SerializedState::SUCCESS)
    {
    if (this->nextHandler)
    {
        return this->nextHandler->handleIncomingMessageBody(buffer, message);
    }
    return MessageProcessingState::SUCCESS;
    }
    return MessageProcessingState::FAILURE;
    }

    template <typename T, typename U>
    SerializedState SerializationHandler::processOutcomingMessageBody(
        yas::shared_buffer& bodyBuffer, const std::any messageBody)
    {
        try
        {
            return YasSerializer::template serialize<std::pair<T,U>>(bodyBuffer, std::any_cast<std::pair<T,U>>(messageBody));
        }
        catch (const std::bad_any_cast& e)
        {
            std::cout << e.what() << '\n';
            std::cout << "Message body cann't be serialized\n";

            return SerializedState::FAILURE;
        }
    }

    template <typename T>
    SerializedState SerializationHandler::processOutcomingMessageBody(yas::shared_buffer& bodyBuffer,const std::any messageBody)
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

    template <typename T, typename U>
    SerializedState SerializationHandler::processIncomingMessageBody(yas::shared_buffer& bodyBuffer,
                                                                     Network::Message& message)
    {
        SerializedState state;

        std::pair<T,U> messageInfo = std::make_pair<T, U>(T(), U());

        state = YasSerializer::template deserialize<std::pair<T,U>>(bodyBuffer, messageInfo);

        if (state == SerializedState::SUCCESS)
        {
            message.mBody = std::make_any<std::pair<T,U>>(messageInfo);
        }

        return state;
    }

    template <typename T>
    SerializedState SerializationHandler::processIncomingMessageBody(yas::shared_buffer& bodyBuffer,
                                                                     Network::Message& message)
    {
        try
        {
            SerializedState state;

            T messageInfo;
            
            state = YasSerializer::template deserialize<T>(bodyBuffer, messageInfo);
           
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
    }  // namespace Utility