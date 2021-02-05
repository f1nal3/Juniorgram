#pragma once

#include "common.h"
#include "message.h"
#include "safeQueue.h"

namespace network
{
class Connection : public std::enable_shared_from_this<Connection>
{
public:
    enum class OwnerType
    {
        SERVER,
        CLIENT
    };

private:
    OwnerType mOwner = OwnerType::SERVER;
    uint64_t mID     = uint64_t();

    asio::ip::tcp::socket mSocket;
    asio::io_context& mContextLink;

    SafeQueue<Message>& mIncomingMessagesQueueLink;
    SafeQueue<Message> mOutcomingMessagesQueue;

    Message mMessageBuffer;

    void writeHeader()
    {
        asio::async_write(
            mSocket,
            asio::buffer(&mOutcomingMessagesQueue.front().mHeader, sizeof(Message::MessageHeader)),
            [this](std::error_code error, [[maybe_unused]] std::size_t length) {
                if (!error)
                {
                    if (!mOutcomingMessagesQueue.front().mBody.empty())
                    {
                        writeBody();
                    }
                    else
                    {
                        mOutcomingMessagesQueue.pop_front();

                        if (!mOutcomingMessagesQueue.empty())
                        {
                            writeHeader();
                        }
                    }
                }
                else
                {
                    std::cout << "[" << mID << "] Write Header Fail.\n";
                    mSocket.close();
                }
            });
    }

    void writeBody()
    {
        asio::async_write(mSocket,
                          asio::buffer(mOutcomingMessagesQueue.front().mBody.data(),
                                       mOutcomingMessagesQueue.front().mBody.size()),
                          [this](std::error_code error, [[maybe_unused]] std::size_t length) {
                              if (!error)
                              {
                                  mOutcomingMessagesQueue.pop_front();

                                  if (!mOutcomingMessagesQueue.empty())
                                  {
                                      writeHeader();
                                  }
                              }
                              else
                              {
                                  std::cout << "[" << mID << "] Write Body Fail.\n";
                                  mSocket.close();
                              }
                          });
    }

    void readHeader()
    {
        asio::async_read(mSocket,
                         asio::buffer(&mMessageBuffer.mHeader, sizeof(Message::MessageHeader)),
                         [this](std::error_code error, [[maybe_unused]] std::size_t length) {
                             if (!error)
                             {
                                 if (mMessageBuffer.mHeader.mBodySize > 0)
                                 {
                                     mMessageBuffer.mBody.resize(mMessageBuffer.mHeader.mBodySize);
                                     readBody();
                                 }
                                 else
                                 {
                                     addToIncomingMessageQueue();
                                 }
                             }
                             else
                             {
                                 std::cout << "[" << mID << "] Read Header Fail.\n";
                                 mSocket.close();
                             }
                         });
    }

    void readBody()
    {
        asio::async_read(mSocket,
                         asio::buffer(mMessageBuffer.mBody.data(), mMessageBuffer.mBody.size()),
                         [this](std::error_code error, [[maybe_unused]] std::size_t length) {
                             if (!error)
                             {
                                 addToIncomingMessageQueue();
                             }
                             else
                             {
                                 std::cout << "[" << mID << "] Read Body Fail.\n";
                                 mSocket.close();
                             }
                         });
    }

    void addToIncomingMessageQueue()
    {
        if (mOwner == OwnerType::SERVER)
        {
            mMessageBuffer.mRemote = this->shared_from_this();
            mIncomingMessagesQueueLink.push_back(mMessageBuffer);
        }
        else
        {
            mIncomingMessagesQueueLink.push_back(mMessageBuffer);
        }

        readHeader();
    }

public:
    Connection(const OwnerType& owner, asio::io_context& contextLink, asio::ip::tcp::socket socket,
               SafeQueue<Message>& incomingMessagesQueueLink)
        : mOwner(owner),
          mSocket(std::move(socket)),
          mContextLink(contextLink),
          mIncomingMessagesQueueLink(incomingMessagesQueueLink)
    {
    }

    uint64_t getID() const { return mID; }

    void connectToClient(const uint64_t uid = uint64_t())
    {
        if (mOwner == OwnerType::SERVER)
        {
            if (mSocket.is_open())
            {
                mID = uid;
                readHeader();
            }
        }
    }
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#elif __GNUC__
#define UNUSED(x) //in order to remove warning: unused-variables
#endif
    void connectToServer(const asio::ip::tcp::resolver::results_type& endpoint)
    {
        if (mOwner == OwnerType::CLIENT)
        {
            asio::async_connect(mSocket, endpoint,
                                [this](std::error_code ec, asio::ip::tcp::endpoint UNUSED(endpoint)) {
                                    if (!ec)
                                    {
                                        readHeader();
                                    }
                                });
        }
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    void disconnect()
    {
        if (isConnected())
        {
            asio::post(mContextLink, [this]() { mSocket.close(); });
        }
    }

    bool isConnected() const { return mSocket.is_open(); }

    void send(const Message& message)
    {
        asio::post(mContextLink, [this, message]() {
            bool isMessageExist = !mOutcomingMessagesQueue.empty();

            mOutcomingMessagesQueue.push_back(message);

            if (!isMessageExist)
            {
                writeHeader();
            }
        });
    }
};
}  // namespace network
