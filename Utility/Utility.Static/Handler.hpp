#pragma once
#include "YasSerializer.hpp"

namespace Network
{
struct Message;
}

namespace Utility
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
    Handler()               = default;
    Handler(const Handler&) = delete;
    Handler(Handler&&)      = delete;
    Handler& operator=(const Handler&) = delete;
    Handler& operator=(Handler&&) = delete;
    virtual ~Handler()            = default;

    virtual Handler* setNext(Handler* handler)                                            = 0;
    virtual MessageProcessingState handleOutcomingMessage(Network::Message& message,
                                                          yas::shared_buffer& bodyBuffer) = 0;
    virtual MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                             Network::Message& message)   = 0;
};

/** @class AbstractHandler
 *  @brief handler class for message preprocessing
 */
class AbstractHandler : public Handler
{
protected:
    Handler* nextHandler;

public:
    AbstractHandler();

    virtual ~AbstractHandler();
    /**
     * @brief Method for setting next handler fot message preprocessing.
     * @param handler - next handler fot message preprocessing.
     * @return handler - next handler fot message preprocessing.
     */
    Handler* setNext(Handler* handler) override;

    /**
     * @brief Method for preprocessing of outcoming messages.
     * @param message - buffer that contains data that should be preprocessed.
     * @param bodyBuffer - buffer that will contain preprocessed body.
     */
    MessageProcessingState handleOutcomingMessage(Network::Message& message,
                                                  yas::shared_buffer& bodyBuffer) override;

    /**
     * @brief Method for preprocessing of incoming message bodies.
     * @param buffer - buffer that contains data that should be preprocessed.
     * @param messageHeader - variable that will contain preprocessed message body.
     */
    MessageProcessingState handleIncomingMessageBody( yas::shared_buffer buffer,
                                                     Network::Message& message) override;
};
}  // namespace Network