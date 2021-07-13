#include "Client.hpp"

#include <Network/Primitives.hpp>
#include <Utility/WarningSuppression.hpp>
#include <Utility.Static/Cryptography.hpp>

#include "ServerInfo.hpp"

namespace Network
{
Client::~Client() { disconnect(); }

bool Client::connect(const std::string_view& host, const uint16_t port)
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

    try
    {
        asio::ip::tcp::resolver resolver(mContext);
        asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve(host, std::to_string(port));

        mConnection =
            std::make_unique<Connection>(Connection::OwnerType::CLIENT, mContext,
                                         asio::ip::tcp::socket(mContext), mIncomingMessagesQueue);

        mConnection->connectToServer(endpoints);

        mContextThread = std::thread([this]() { mContext.run(); });

        return true;
    }
    catch (std::exception& exception)
    {
        std::cerr << "Client Exception: " << exception.what() << "\n";
        return false;
    }
}

void Client::disconnect()
{
    if (isConnected())
    {
        mConnection->disconnect();
    }

    mContext.stop();

    if (mContextThread.joinable())
    {
        mContextThread.join();
    }

    mConnection.release();
}

bool Client::isConnected() const
{
    if (mConnection != nullptr)
    {
        return mConnection->isConnected();
    }

    else
    {
        return false;
    }
}

void Client::send(const Message& message) const
{
    if (isConnected())
    {
        mConnection->send(message);
    }
}

void Client::pingServer() const
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::ServerPing;

    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

    timeNow = message.mHeader.mTimestamp;
    send(message);
}

void Client::askForChannelList() const
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;
    send(message);
}

void Client::askForMessageHistory() const
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryRequest;
    send(message);
}

void Client::storeMessages(const std::vector<std::string>& messagesList) const
{
    for (auto&& msg : messagesList)
    {
        Network::Message message;
        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreRequest;

        Network::MessageInfo info;
        info.userID  = mConnection->getID();
        info.message = msg;

        message.mBody = std::make_any<Network::MessageInfo>(info);

        send(message);
    }
}

void Client::userRegistration(const std::string& email, const std::string& login,
                      const std::string& password) const
{
    // Generating password's hash which are based on login. It lets us to insert different users
    // with the same passwords.
    const std::string PASSWORD_HASH = Hashing::SHA_256(password, login);
    Network::RegistrationInfo ri(email, login, PASSWORD_HASH);

    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::RegistrationRequest;
    message.mBody = std::make_any<RegistrationInfo>(ri);
    
    send(message);
}

void Client::messageAll() const
{
    Network::Message message;
    message.mHeader.mMessageType = Network::Message::MessageType::MessageAll;
    send(message);
}
}  // namespace Network
