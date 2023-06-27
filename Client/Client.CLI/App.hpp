#include <Client.hpp>
#include <string>

/**
 * @class App(Client.CLI)
 * @brief App(Client.CLI) class which allows
 *        you to work with the console
 *        and process messages from the kernel.
 */
class App : public Network::Client
{
public:
    /**
     * @brief App(Client.CLI) constructor.
     * @details Connects to the kernel by address and port.
     */
    App(/* args */);

    /**
     * @brief App(Client.CLI) destructor.
     * @details Disconnect client connection.
     */
    ~App();

protected:
    /// Kernel accepted handler
    void onKernelAccepted() override;
    /// Kernel ping handler
    void onKernelPing(double timestamp) override;
    /// Kernel message handler
    void onKernelMessage(const uint64_t clientId) override;
    /// Channel list request handler
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) override;
    /// Message store answer handler
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;
    /// Message delete answer handler
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) override;
    /// Message history answer handler
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) override;
    /// Registration answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
};
