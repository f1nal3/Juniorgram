#pragma once

#include "Message.hpp"
#include "Utility/YasSerializer.hpp"

class Handler
{
public:
    virtual Handler* setNext(Handler* handler)      = 0;
    virtual void handleOutcomingMessage(const Network::Message& message, yas::shared_buffer& headerBuffer,
                                        yas::shared_buffer& bodyBuffer)                     = 0;
    virtual void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                             Network::Message::MessageHeader& messgeHeader) = 0;
    virtual void handleIncomingMessageBody(const yas::shared_buffer buffer,
                                             Network::Message& message) = 0;
    
};

class AbstractHandler : public Handler
{
protected:
    Handler* nextHandler;

public:
    AbstractHandler() : nextHandler(nullptr) {}

    Handler* setNext(Handler* handler) override
    {
        this->nextHandler = handler;
        return handler;
    }

    void handleOutcomingMessage(const Network::Message& message, yas::shared_buffer& headerBuffer,
                       yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                    Network::Message::MessageHeader& messageHeader) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    void handleIncomingMessageBody(const yas::shared_buffer buffer,
                                   Network::Message& message) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
