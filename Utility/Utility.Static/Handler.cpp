#include <Network.Static/Message.hpp>
#include "Handler.hpp"

namespace Utility
{
    AbstractHandler::AbstractHandler()
      : nextHandler(nullptr)
    {}

    AbstractHandler::~AbstractHandler()
    { 
        delete nextHandler; 
    }

    Handler* AbstractHandler::setNext(Handler* handler)
    {
        this->nextHandler = handler;
        return this->nextHandler;
    }

   MessageProcessingState AbstractHandler::handleOutcomingMessage(Network::Message& message,
                                                                   yas::shared_buffer& bodyBuffer)
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    MessageProcessingState AbstractHandler::handleIncomingMessageBody(yas::shared_buffer buffer,
                                                                      Network::Message& message)
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }
}