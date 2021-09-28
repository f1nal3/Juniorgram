#include "Client.hpp"

#include <Network/Primitives.hpp>
#include <Crypto.Static/Cryptography.hpp>

#include "ServerInfo.hpp"

namespace Network
{
using MessageType = Network::Message::MessageType;

Client::~Client() { disconnectFromServer(); }

bool Client::connectToServer(const std::string_view& host, const uint16_t port)
{
    if (host != ServerInfo::address)
    {
        std::cerr << "Bad server address" << std::endl;
        return false;
    }
    if (port != ServerInfo::port)
    {
        std::cerr << "Bad port value" << std::endl;
        return false;
    }

    asio::ip::tcp::resolver resolver(_context);

    using OwnerType = Connection::OwnerType;
    _connection     = std::make_unique<Connection>(OwnerType::CLIENT, _context, asio::ip::tcp::socket(_context), _incomingMessagesQueue);

    try
    {
        /// Auto = asio::ip::tcp::resolver::results_type
        auto endpoints = resolver.resolve(host, std::to_string(port));
        _connection->connectToServer(endpoints);
        _contextThread = std::thread([=]() {
            while (_context.run_one())
            {
                loop();
            }
            _serverAccept = false;
            onDisconnect();
        });
    }
    catch (std::exception& exception)
    {
        std::cerr << "Client Exception: " << exception.what() << "\n";
        return false;
    }
    return true;
}

void Client::disconnectFromServer()
{
    if (isConnected())
    {
        _connection->disconnect();
    }

    _context.stop();

    if (_contextThread.get_id() != std::this_thread::get_id() && _contextThread.joinable())
    {
        _contextThread.join();
    }

    _connection.reset();
}

bool Client::isConnected() const
{
    if (_connection != nullptr)
    {
        return _connection->isConnected() && _serverAccept;
    }
    return false;
}

void Client::send(const Message& message) const
{
    if (isConnected())
    {
        _connection->send(message);
    }
    else
    {
        onMessageSendFailed(message);
    }
}
void Client::pingServer() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ServerPing;

    auto timeNow = std::chrono::system_clock::now();
    timeNow      = message.mHeader.mTimestamp;
    send(message);
}

void Client::askForChannelList() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ChannelListRequest;
    send(message);
}

void Client::deleteChannel(const std::string channelName) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ChannelDeleteRequest;

    std::string ri;
    ri                   = channelName;
    networkMessage.mBody = std::make_any<std::string>(ri);
    send(networkMessage);
}

void Client::askForMessageHistory(const std::uint64_t channelID) const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageHistoryRequest;

    message.mBody = std::make_any<std::uint64_t>(channelID);
    send(message);
}

void Client::askForReplyHistory(uint64_t channelID) const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::ReplyHistoryRequest;

    message.mBody = std::make_any<std::uint64_t>(channelID);
    send(message);
}

void Client::storeMessage(const std::string& message, const uint64_t channelID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::MessageStoreRequest;

    Network::MessageInfo mi;
    mi.message   = message;
    mi.channelID = channelID;

    networkMessage.mBody = std::make_any<Network::MessageInfo>(mi);
    send(networkMessage);
}

void Client::storeReply(const std::string &message, uint64_t channelID, uint64_t msgID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ReplyStoreRequest;

    Network::ReplyInfo ri;
    ri.message = message;
    ri.channelID = channelID;
    ri.msgID = msgID;

    networkMessage.mBody = std::make_any<Network::ReplyInfo>(ri);
    send(networkMessage);
}

void Client::userRegistration(const std::string& email, const std::string& login, const std::string& password) const
{
    // Generating password's hash which are based on login. It lets us to insert different users
    // with the same passwords.
    const std::string         pwdHash = Base::Hashing::SHA_256(password, login);
    Network::RegistrationInfo ri(email, login, pwdHash);

    Network::Message message;
    message.mHeader.mMessageType = MessageType::RegistrationRequest;
    message.mBody                = std::make_any<RegistrationInfo>(ri);

    send(message);
}

void Client::userAuthorization(const std::string& login, const std::string& password) const
{
    const std::string pwdHash = Base::Hashing::SHA_256(password, login);
    LoginInfo         loginInfo(login, pwdHash);

    Message message;
    message.mHeader.mMessageType = MessageType::LoginRequest;
    message.mBody                = std::make_any<LoginInfo>(loginInfo);

    send(message);
}

void Client::messageAll() const
{
    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageAll;
    send(message);
}

void Client::userMessageDelete(const uint64_t messageID) const
{
    Network::MessageInfo mi;
    mi.msgID = messageID;

    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageDeleteRequest;
    message.mBody                = std::make_any<Network::MessageInfo>(mi);
    send(message);
}

void Client::userMessageDelete(const std::string& messageText) const
{
    Network::MessageInfo mi;
    mi.message = messageText;

    Network::Message message;
    message.mHeader.mMessageType = MessageType::MessageDeleteRequest;
    message.mBody                = std::make_any<Network::MessageInfo>(mi);
    send(message);
}

void Client::subscriptionChannel(const std::uint64_t channelID) const
{
    Network::Message networkMessage;
    networkMessage.mHeader.mMessageType = MessageType::ReplyStoreRequest;

    Network::SubscriptionChannelInfo ri;
    ri.channelID         = channelID;
    networkMessage.mBody = std::make_any<Network::SubscriptionChannelInfo>(ri);
    //send(networkMessage); temporarily commented out before implementation on the server side
}

void Client::loop()
{
    while (!_incomingMessagesQueue.empty())
    {
        const Message message = _incomingMessagesQueue.pop_front();
        std::string   output  = "[" + std::to_string(message.mHeader.mTimestamp.time_since_epoch().count()) + "]\n";
        std::cout << output;
        switch (message.mHeader.mMessageType)
        {
            case MessageType::LoginAnswer:
            {
                auto loginSuccessful = std::any_cast<bool>(message.mBody);
                onLoginAnswer(loginSuccessful);
            }
            break;

            case MessageType::ServerAccept:
            {
                _serverAccept = true;
                onServerAccepted();
            }
            break;

            case MessageType::ServerPing:
            {
                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                std::chrono::system_clock::time_point timeThen;
                timeThen = message.mHeader.mTimestamp;
                onServerPing(std::chrono::duration<double>(timeNow - timeThen).count());
            }
            break;

            case MessageType::ServerMessage:
            {
                // TODO: add handling
                uint64_t clientID = 0;
                onServerMessage(clientID);
            }
            break;

            case MessageType::ChannelListRequest:
            {
                auto channels = std::any_cast<std::vector<Network::ChannelInfo>>(message.mBody);
                onChannelListRequest(channels);
            }
            break;

            case MessageType::MessageHistoryAnswer:
            {
                auto messages = std::any_cast<std::vector<Network::MessageInfo>>(message.mBody);
                onMessageHistoryAnswer(messages);
            }
            break;

            case MessageType::MessageStoreAnswer:
            {
                auto code = std::any_cast<Utility::StoringMessageCodes>(message.mBody);
                onMessageStoreAnswer(code);
            }
            break;

            case MessageType::RegistrationAnswer:
            {
                auto code = std::any_cast<Utility::RegistrationCodes>(message.mBody);

                onRegistrationAnswer(code);
            }
            break;

            case MessageType::MessageDeleteAnswer:
            {
                auto messageInfo = std::any_cast<Utility::DeletingMessageCodes>(message.mBody);
                onUserMessageDeleteAnswer(messageInfo);
            }
            break;

            case MessageType::ReplyHistoryAnswer:
            {
                auto replies = std::any_cast<std::vector<Network::ReplyInfo>>(message.mBody);
                onReplyHistoryAnswer(replies);
            }
            break;

            case MessageType::ReplyStoreAnswer:
            {
                auto code = std::any_cast<Utility::StoringReplyCodes>(message.mBody);
                onReplyStoreAnswer(code);
            }
            break;

            case MessageType::ChannelDeleteAnswer:
            {
                auto channelDeleteCode = std::any_cast<Utility::ChannelDeleteCode>(message.mBody);
                onChannelDeleteAnswer(channelDeleteCode);
            }
            break;

            default:
                std::cerr << "[Client][Warning] unimplemented[" << uint32_t(message.mHeader.mMessageType) << "]\n";
        }
    }
}

void Client::onLoginAnswer(bool success)
{
    (void)(success);
    std::cerr << "[Client][Warning] login answer is not implemented\n";
}

void Client::onServerAccepted() { std::cerr << "[Client][Warning] server accepted is not implemented\n"; }

void Client::onServerPing(double timestamp)
{
    (void)(timestamp);
    std::cerr << "[Client][Warning] server ping is not implemented\n";
}

void Client::onServerMessage(const uint64_t clientId)
{
    (void)(clientId);
    std::cerr << "[Client][Warning] server message is not implemented\n";
}

void Client::onChannelListRequest(const std::vector<Network::ChannelInfo>& channels)
{
    (void)(channels);
    std::cerr << "[Client][Warning] channel list request is not implemented\n";
}

void Client::onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
{
    (void)(messages);
    std::cerr << "[Client][Warning] message history answer is not implemented\n";
}

void Client::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    (void)(storingMessageCode);
    std::cerr << "[Client][Warning] message store answer is not implemented\n";
}

void Client::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
    (void)(registrationCode);
    std::cerr << "[Client][Warning] registration answer is not implemented\n";
}

void Client::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) 
{
    (void)(deletingState);
    std::cerr << "[Client][Warning] onUserMessageDeleteAnswer answer is not implemented\n";
}

void Client::onDisconnect() { std::cerr << "[Client][Warning] onDisconnect is not implemented\n"; }

void Client::onMessageSendFailed(const Message& message) const
{
    (void)(message);
    std::cerr << "[Client][Warning] onMessageSendFailed is not implemented\n";
}

void Client::onReplyHistoryAnswer(const std::vector<Network::ReplyInfo>& replies)
{
    (void)(replies);
    std::cerr << "[Client][Warning] reply answer is not implemented\n";
}

void Client::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    (void)(storingReplyCode);
    std::cerr << "[Client][Warning] reply store answer is not implemented\n";
}

void Client::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode)
{
    (void)(channelDeleteCode);
    std::cerr << "[Client][Warning] leave channel answer is not implemented\n";
}

}  // namespace Network
