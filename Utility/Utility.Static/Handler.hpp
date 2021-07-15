#pragma once
#include "IHandler.hpp"
#include "YasSerializer.hpp"

#include "Network/Message.hpp"
#include "Utility/Exception.hpp"


namespace Utility
{
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
        MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                         Network::Message& message) override;
    };
}  // namespace Utility
