#pragma once

#include "common.h"
#include "connection.h"
#include "message.h"
#include "safeQueue.h"

namespace network {
    class Server {
        uint64_t mIDCounter = 10000, mCriticalQueueSize = 100, mNewThreadsCount = std::thread::hardware_concurrency();
        
        asio::io_context mContext;
        
        asio::ip::tcp::acceptor mAcceptor;

        std::deque<std::shared_ptr<Connection>> mConnectionsPointers;

        SafeQueue<Message> mIncomingMessagesQueue;

        std::deque<std::thread> mThreads;

        bool onClientConnect(const std::shared_ptr<Connection> &client) {
            network::Message message;
            message.mHeader.mID = network::Message::MessageType::ServerAccept;
            client->send(message);
            return true;
        }
        
        void onClientDisconnect(const std::shared_ptr<Connection> &client) {
            std::cout << "Removing client [" << client->getID() << "]\n";
        }
        
        void onMessage(const std::shared_ptr<Connection> &client, Message &message) {
            switch (message.mHeader.mID) {
                case network::Message::MessageType::ServerPing: {
                    std::cout << "[" << client->getID() << "]: Server Ping\n";
                    
                    client->send(message);
                }
                    break;
                
                case network::Message::MessageType::MessageAll: {
                    std::cout << "[" << client->getID() << "]: Message All\n";
                    
                    network::Message message;
                    message.mHeader.mID = network::Message::MessageType::ServerMessage;
                    message << client->getID();
                    messageAllClients(message, client);
                }
                    break;
            }
        }
    
    public:
        explicit Server(const uint16_t &port)
                : mAcceptor(mContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
        }
        
        ~Server() {
            stop();
        }
        
        bool start() {
            try {
                waitForClientConnection();
                
                size_t threadsCount = std::thread::hardware_concurrency();
                threadsCount > 1 ? --threadsCount : threadsCount = 1;

                for (size_t i = 0; i < threadsCount; ++i) {
                    mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
                }
                
                std::cout << "[SERVER] Started!\n";
                return true;
            }
            catch (std::exception &exception) {
                std::cerr << "[SERVER] Exception: " << exception.what() << "\n";
                return false;
            }
        }
        
        void stop() {
            mContext.stop();
            
            for (std::thread& thread : mThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
            
            mThreads.clear();

            std::cout << "[SERVER] Stopped!\n";
        }
        
        void waitForClientConnection()
		{
			mAcceptor.async_accept([this](std::error_code error, asio::ip::tcp::socket socket)
				{
					if (!error)
					{
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

						std::shared_ptr<Connection> newConnection = std::make_shared<Connection>(Connection::OwnerType::SERVER,
							mContext, std::move(socket), mIncomingMessagesQueue);

						if (onClientConnect(newConnection))
						{
							mConnectionsPointers.push_back(std::move(newConnection));

							mConnectionsPointers.back()->connectToClient(mIDCounter++);

							std::cout << "[" << mConnectionsPointers.back()->getID() << "] Connection Approved\n";
                        } else {
                            std::cout << "[-----] Connection Denied\n";
                        }
                    } else {
                        std::cout << "[SERVER] New Connection Error: " << error.message() << "\n";
                    }
                    
                    waitForClientConnection();
                });
        }
        
        void messageClient(std::shared_ptr<Connection> client, const Message &message) {
            if (client != nullptr && client->isConnected()) {
                client->send(message);
            } else {
                onClientDisconnect(client);
                
                client.reset();
                
                mConnectionsPointers.erase(
                        std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), client),
                        mConnectionsPointers.end());
            }
        }
        
        void messageAllClients(const Message &message, const std::shared_ptr<Connection> &exceptionClient = nullptr) {
            bool deadConnectionExist = false;
            
            for (auto &client : mConnectionsPointers) {
                if (client != nullptr && client->isConnected()) {
                    if (client != exceptionClient) {
                        client->send(message);
                    }
                } else {
                    onClientDisconnect(client);
                    
                    client.reset();
                    
                    deadConnectionExist = true;
                }
            }
            
            if (deadConnectionExist) {
                mConnectionsPointers.erase(
                        std::remove(mConnectionsPointers.begin(), mConnectionsPointers.end(), nullptr),
                        mConnectionsPointers.end());
            }
        }
        
        void update(size_t maxMessages = MAXSIZE_T, bool wait = true) {
			if (wait)
			{
				mIncomingMessagesQueue.wait();
			}

            if (mIncomingMessagesQueue.size() > mCriticalQueueSize)
            {
                for (size_t i = 0; i < mNewThreadsCount; ++i) {
                    mThreads.emplace_back(std::thread([this]() { mContext.run(); }));
                }
            }

			size_t messagesCount = size_t();

			while (messagesCount < maxMessages && !mIncomingMessagesQueue.empty())
			{
				Message message = mIncomingMessagesQueue.pop_front();

				onMessage(message.mRemote, message);

				messagesCount++;
			}
		}
	};
}
