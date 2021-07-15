#include "Network/Message.hpp"
#include "Utility/WarningSuppression.hpp"
// clang-format off
suppressWarning(4996, -Wdeprecated - declarations)
suppressWarning(4458, -Wshadow)
#include <yas/buffers.hpp>
restoreWarning
restoreWarning
    // clang-format on

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
        // Handler()               = default;
        // Handler(const Handler&) = delete;
        // Handler(Handler&&)      = delete;
        // Handler& operator=(const Handler&) = delete;
        // Handler& operator=(Handler&&) = delete;
        virtual ~Handler() = default;

        virtual Handler* setNext(Handler* handler)                                            = 0;
        virtual MessageProcessingState handleOutcomingMessage(Network::Message& message,
                                                              yas::shared_buffer& bodyBuffer) = 0;
        virtual MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                                 Network::Message& message)   = 0;
    };
}