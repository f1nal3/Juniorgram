#pragma once
#include "Network/Message.hpp"
#include "YasSerializer.hpp"

namespace Network
{
class Handler
{
public:
    Handler()               = default;
    Handler(const Handler&) = delete;
    Handler(Handler&&)      = delete;
    Handler& operator=(const Handler&) = delete;
    Handler& operator=(Handler&&) = delete;
    virtual ~Handler()            = default;

    virtual Handler* setNext(Handler* handler)                                                = 0;
    virtual void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                        yas::shared_buffer& bodyBuffer)                       = 0;
    virtual void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                             Message::MessageHeader& messgeHeader)            = 0;
    virtual void handleIncomingMessageBody(const yas::shared_buffer buffer, Network::Message& message) = 0;
};

class AbstractHandler : public Handler
{
protected:
    Handler* nextHandler;

public:
    AbstractHandler() : nextHandler(nullptr) {}

    virtual ~AbstractHandler() { delete nextHandler; }

    Handler* setNext(Handler* handler) override
    {
        this->nextHandler = handler;
        return this->nextHandler;
    }

    void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
}  // namespace Network
