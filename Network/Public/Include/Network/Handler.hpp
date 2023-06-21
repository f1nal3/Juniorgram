#pragma once
#include "Network/YasSerializer.hpp"
#include "Network/Message.hpp"

namespace Network
{
/** 
* @enum MessageProcessingState
* @brief Flag showing the result of message processing.
* @details If the message processing result is successful /
* SUCCESS will be returned. /
* Else will be returned FAILURE.
*/
enum class MessageProcessingState
{
    SUCCESS,  /// successful message processing
    FAILURE   /// unsuccessful message processing
};

/** 
* @class Handler
* @brief interface for handler class for message preprocessing.
*/
class Handler
{
public:
    /// Default Handler constructor
    Handler()               = default;
    /// Delete Handler copy constructor
    Handler(const Handler&) = delete;
    /// Delete Handler move constructor
    Handler(Handler&&)      = delete;
    /// Delete Handler copy assignment constructor
    Handler& operator=(const Handler&) = delete;
    /// Delete Handler move assignment constructor
    Handler& operator=(Handler&&) = delete;
    /// Virtual Handler destructor
    virtual ~Handler()            = default;

    /// Virtual Handler method for preprocessing next handler
    virtual void setNext(Handler* handler)                                                      = 0;

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

/** 
* @class AbstractHandler
* @brief handler class for message preprocessing
*/
class AbstractHandler : public Handler
{
protected:
    Handler* nextHandler = nullptr;

public:
    /**
    * @brief Method for setting next handler fot message preprocessing.
    * @param handler - next handler fot message preprocessing.
    * @return handler - next handler fot message preprocessing.
    */
    void setNext(Handler* handler) override
    {
        this->nextHandler = handler;
    }
};
}  /// namespace Network
