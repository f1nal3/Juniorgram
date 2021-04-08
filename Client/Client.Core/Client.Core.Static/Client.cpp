#include "Client.hpp"
#include <Utility/WarningSuppression.hpp>
#include <Network/MessageWrapper.hpp>

namespace Network
{
    Client::~Client() { disconnect(); }

    bool Client::connect(const std::string& host, const uint16_t& port)
    {
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
        message.mHeader.mID = Network::Message::MessageType::ServerPing;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        message << timeNow;
        send(message);
    }
    void Client::askForChannelList() const
    {
        Network::Message message;
        message.mHeader.mID = Network::Message::MessageType::ChannelListRequest;
        send(message);
    }
    void Client::askForMessageHistory() const
    { 
        Network::Message message;
        message.mHeader.mID = Network::Message::MessageType::MessageHistoryRequest;
        send(message);
    }
    void Client::storeMessages(const std::vector<std::string>& messagesList) const
    {
        for (auto&& msg : messagesList)
        {
            Network::Message message;
            message.mHeader.mID = Network::Message::MessageType::MessageStoreRequest;

            MessageWrapper mw;
            
            mw.userID = mConnection->getID();
            mw.messageText = msg;
            mw.timestamp = message.mHeader.mTimestamp;
            
            message << mw;
            send(message);
        }
    }
    void Client::messageAll() const 
    {
        Network::Message message;
        message.mHeader.mID = Network::Message::MessageType::MessageAll;
        send(message);
    }
}
