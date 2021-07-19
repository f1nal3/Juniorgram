#pragma once
#include <asio.hpp>

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include <Network.Static/Message.hpp>
#include <Network.Static/SafeQueue.hpp>

namespace Network
{
class Connection;

//struct Message;

//template <typename T> 
//class SafeQueue;

}

namespace DataAccess
{
class IRepository;
}

namespace Server
{

class Server
{
    uint64_t mIDCounter = 10000, mCriticalQueueSize = 100,
             mNewThreadsCount = std::thread::hardware_concurrency();

    asio::io_context mContext;
    asio::ip::tcp::acceptor mAcceptor;
    std::deque<std::shared_ptr<Network::Connection>> mConnectionsPointers;
    Network::SafeQueue<Network::Message> mIncomingMessagesQueue;
    std::deque<std::thread> mThreads;
    std::unique_ptr<DataAccess::IRepository> _postgreRepo;
    

    bool onClientConnect(const std::shared_ptr<Network::Connection>& client);

    void onClientDisconnect(const std::shared_ptr<Network::Connection>& client);

    void onMessage(const std::shared_ptr<Network::Connection>& client, Network::Message& message);

    std::string getToken(const std::shared_ptr<Network::Connection>& client);

public:
    explicit Server(const uint16_t& port);

    ~Server();

  
    bool start();

    void stop();

    void waitForClientConnection();

    void messageClient(std::shared_ptr<Network::Connection> client,
                       const Network::Message& message);

    void messageAllClients(const Network::Message& message,
                           const std::shared_ptr<Network::Connection>& exceptionClient = nullptr);
    
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);

};
}  // namespace Network
