#include <Client.hpp>
#include <string>

/**
 * @class App(Client.CLI)
 * @brief App(Client.CLI) class which allows
 *        you to work with the console
 *        and process messages from the server.
 */
class App : public Network::Client
{
public:
    /**
     * @brief App(Client.CLI) constructor.
     * @details Connects to the server by address and port.
     */
    App(/* args */);

    /**
     * @brief App(Client.CLI) destructor.
     * @details Disconnect client connection.
     */
    ~App();

protected:
    /// Server accepted handler
    void onServerAccepted() override;
    /// Server ping handler
    void onServerPing(double timestamp) override;
    /// Server message handler
    void onServerMessage(const uint64_t clientId) override;
    /// Channel list request handler
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) override;
    /// Message store answer handler
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;
    /// Message delete answer handler
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) override;
    /// Message history answer handler
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) override;
    /// Registration answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
};
