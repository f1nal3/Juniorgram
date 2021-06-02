#pragma once
#include "Network/Message.hpp"
#include "YasSerializer.hpp"

namespace Network
{
/** @class Handler
 *  @brief interface for handler class for message preprocessing.
 */
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
    virtual void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) = 0;
};

/** @class AbstractHandler
 *  @brief handler class for message preprocessing
 */
class AbstractHandler : public Handler
{
protected:
    Handler* nextHandler;

public:
    AbstractHandler() : nextHandler(nullptr) {}

    virtual ~AbstractHandler() { delete nextHandler; }
    
    /**
     * @brief Method for setting next handler fot message preprocessing.
     * @param handler - next handler fot message preprocessing.
     * @return handler - next handler fot message preprocessing.
     */
    Handler* setNext(Handler* handler) override
    {
        this->nextHandler = handler;
        return this->nextHandler;
    }

    /**
     * @brief Method for preprocessing of outcoming messages.
     * @param message - buffer that contains data that should be preprocessed.
     * @param headerBuffer - buffer that will contain preprocessed header.
     * @param bodyBuffer - buffer that will contain preprocessed body.
     */
    void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    /**
     * @brief Method for preprocessing of incoming message headers.
     * @param buffer - buffer that contains data that should be preprocessed.
     * @param messageHeader - variable that will contain preprocessed message header data.
     */
    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    /**
     * @brief Method for preprocessing of incoming message bodies.
     * @param buffer - buffer that contains data that should be preprocessed.
     * @param messageHeader - variable that will contain preprocessed message body.
     */
    void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
}  // namespace Network
