#pragma once

#include "DataAccess/IRepository.hpp"

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace Network
{
class Client
{
    asio::io_context mContext{};
    std::thread mContextThread{};
    std::unique_ptr<Connection> mConnection{};

    std::unique_ptr<DataAccess::IRepository> mSQLCipherRepo;

    SafeQueue<Message> mIncomingMessagesQueue{};

public:
    ~Client();

    Client();

    bool checkTokenExistance();

    bool connect(const std::string& host, const uint16_t& port);

    void disconnect();

    bool isConnected() const;

    void send(const Message& message) const;

    SafeQueue<Message>& incoming() { return mIncomingMessagesQueue; }

    void pingServer() const;

    void askForChannelList() const;

    void askForMessageHistory() const;

    void storeMessages(const std::vector<std::string>& messagesList) const;

    void userRegistration(const std::string& email, const std::string& login,
                          const std::string& password) const;

    void messageAll() const;
};
}  // namespace Network
