#include "Client.hpp"

namespace network
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
        network::Message message;
        message.mHeader.mID = network::Message::MessageType::ServerPing;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        message << timeNow;
        send(message);
    }

    void Client::messageAll() const 
    {
        network::Message message;
        message.mHeader.mID = network::Message::MessageType::MessageAll;
        send(message);
    }
}