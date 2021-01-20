#pragma once

#include "common.h"
#include "connection.h"
#include "message.h"
#include "safeQueue.h"

namespace network {
class Client {
    asio::io_context mContext;
    std::thread mContextThread;

    std::unique_ptr<Connection> mConnection;

    SafeQueue<Message> mIncomingMessagesQueue;

public:
    ~Client() { disconnect(); }

    bool connect(const std::string& host, const uint16_t& port) {
        try {
            asio::ip::tcp::resolver resolver(mContext);
            asio::ip::tcp::resolver::results_type endpoints =
                resolver.resolve(host, std::to_string(port));

            mConnection = std::make_unique<Connection>(Connection::OwnerType::CLIENT, mContext,
                                                       asio::ip::tcp::socket(mContext),
                                                       mIncomingMessagesQueue);

            mConnection->connectToServer(endpoints);

            mContextThread = std::thread([this]() { mContext.run(); });

            return true;
        } catch (std::exception& exception) {
            std::cerr << "Client Exception: " << exception.what() << "\n";
            return false;
        }
    }

    void disconnect() {
        if (isConnected()) { mConnection->disconnect(); }

        mContext.stop();

        if (mContextThread.joinable()) { mContextThread.join(); }

        mConnection.release();
    }

    bool isConnected() const {
        if (mConnection != nullptr) { return mConnection->isConnected(); }
        else {
            return false;
        }
    }

    void send(const Message& message) const {
        if (isConnected()) { mConnection->send(message); }
    }

    SafeQueue<Message>& incoming() { return mIncomingMessagesQueue; }

    void pingServer() const {
        network::Message message;
        message.mHeader.mID = network::Message::MessageType::ServerPing;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        message << timeNow;
        send(message);
    }

    void messageAll() const {
        network::Message message;
        message.mHeader.mID = network::Message::MessageType::MessageAll;
        send(message);
    }
};
}  // namespace network
