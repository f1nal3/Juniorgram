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
    void onServerAccepted() override;
    void onServerPing(double timestamp) override;
    void onServerMessage(const uint64_t clientId) override;
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) override;
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) override;
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
};
