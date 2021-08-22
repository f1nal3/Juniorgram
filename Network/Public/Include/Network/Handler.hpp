#pragma once
#include "Network/Message.hpp"
#include "YasSerializer.hpp"

namespace Network
{
/** @enum MessageProcessingState
 *  @brief Successful or not result of message preprocessing
 */
enum class MessageProcessingState
{
    SUCCESS,  /// successful message processing
    FAILURE   /// unsuccessful message processing
};

/** @class Handler
 *  @brief interface for handler class for message preprocessing.
 */
class Handler
{
public:
    // Default Handler constructor
    Handler()               = default;
    // Delete Handler copy constructor
    Handler(const Handler&) = delete;
    // Delete Handler move constructor
    Handler(Handler&&)      = delete;
    // Delete Handler copy assigment constructor
    Handler& operator=(const Handler&) = delete;
    // Delete Handler move assigment constructor
    Handler& operator=(Handler&&) = delete;
    // Virtual Handler destructor
    virtual ~Handler()            = default;

    // Virtual Handler method for preprocessing next handler
    virtual Handler*               setNext(Handler* handler)                                                      = 0;

    /**
     * @brief Virtual method for preprocessing of outcoming messages.
     * @param message - buffer that contains data that should be preprocessed.
     * @param bodyBuffer - buffer that will contain preprocessed body.
     */
    virtual MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) = 0;

    /**
     * @brief Virtual method for preprocessing of incoming message bodies.
     * @param buffer - buffer that contains data that should be preprocessed.
     * @param message - variable that will contain preprocessed message body.
     */
    virtual MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message)   = 0;
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
     * @param bodyBuffer - buffer that will contain preprocessed body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
     * @brief Method for preprocessing of incoming message bodies.
     * @param buffer - buffer that contains data that should be preprocessed.
     * @param message - variable that will contain preprocessed message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  // namespace Network
