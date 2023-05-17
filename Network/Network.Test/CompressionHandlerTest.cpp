#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/YasSerializer.hpp"
TEST_CASE("CompressionHandler Test")
{
    yas::shared_buffer            bodyBuffer;
    Network::Message              message;
    SECTION("test method handleOutcomingMessage")
    {
        Network::CompressionHandler   compressionHandler;
        REQUIRE(Network::MessageProcessingState::FAILURE == compressionHandler.handleOutcomingMessage(message,bodyBuffer));
        compressionHandler.setNext(new Network::CompressionHandler());
        REQUIRE(Network::MessageProcessingState::SUCCESS == compressionHandler.handleOutcomingMessage(message,bodyBuffer));
        
    }
    SECTION("test method handleIncomingMessageBody")
    {
        Network::CompressionHandler   compressionHandler;
        REQUIRE(Network::MessageProcessingState::FAILURE == compressionHandler.handleIncomingMessageBody(bodyBuffer,message));
        compressionHandler.setNext(new Network::CompressionHandler());
        REQUIRE(Network::MessageProcessingState::SUCCESS == compressionHandler.handleIncomingMessageBody(bodyBuffer,message));
    }
}