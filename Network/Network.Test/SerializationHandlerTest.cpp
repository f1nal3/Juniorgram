#include <catch2/catch.hpp>
#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/YasSerializer.hpp"
TEST_CASE("SerializationHandler Test")
{
    std::uint32_t                 unknownMessageType = 35;
	Network::SerializationHandler handler;
    yas::shared_buffer            bodyBuffer;
    Network::Message              message;
    message.mBody.emplace<std::string>("message");

    SECTION("test method handleOutcomingMessage")
    {
        message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerPing;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageAll;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = static_cast<Network::Message::MessageType>(unknownMessageType);
        REQUIRE(Network::MessageProcessingState::FAILURE == handler.handleOutcomingMessage(message, bodyBuffer));
        
    }
    SECTION("test method handleIncomingMessageBody")
    {
        message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody(bodyBuffer,message));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerPing;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody(bodyBuffer,message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageAll;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody(bodyBuffer,message));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody(bodyBuffer,message));

        message.mHeader.mMessageType = static_cast<Network::Message::MessageType>(unknownMessageType);
        REQUIRE(Network::MessageProcessingState::FAILURE == handler.handleIncomingMessageBody(bodyBuffer,message));
    }
}
