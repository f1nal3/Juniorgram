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
class IHandler
{
public:
    /// Default Handler constructor
    IHandler()               = default;
    /// Delete Handler copy constructor
    IHandler(const IHandler&) = delete;
    /// Delete Handler move constructor
    IHandler(IHandler&&)      = delete;
    /// Delete Handler copy assignment constructor
    IHandler& operator=(const IHandler&) = delete;
    /// Delete Handler move assignment constructor
    IHandler& operator=(IHandler&&) = delete;
    /// Virtual Handler destructor
    virtual ~IHandler()            = default;

    /// Virtual Handler method for preprocessing next handler
    virtual IHandler*               setNext(IHandler* handler)                                                      = 0;

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
 *  @brief handler class for message preprocessing.
 */
class AbstractHandler : public IHandler
{
protected:
    IHandler* _nextHandler;

public:
    AbstractHandler() : _nextHandler(nullptr) {}

    virtual ~AbstractHandler() { delete _nextHandler; }

    /**
     * @brief Method for setting next handler fot message preprocessing.
     * @param handler - next handler fot message preprocessing.
     * @return handler - next handler fot message preprocessing.
     */
    IHandler* setNext(IHandler* handler) override
    {
        this->_nextHandler = handler;
        return this->_nextHandler;
    }

    /**
     * @brief Method for preprocessing of outcoming messages.
     * @param message - buffer that contains data that should be preprocessed.
     * @param bodyBuffer - buffer that will contain preprocessed body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        if (this->_nextHandler)
        {
            this->_nextHandler->handleOutcomingMessage(message, bodyBuffer);
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
        if (this->_nextHandler)
        {
            this->_nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  /// namespace Network
