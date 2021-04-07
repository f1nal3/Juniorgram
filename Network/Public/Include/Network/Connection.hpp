#pragma once

#include <asio.hpp>
#include <iostream>

#include "Message.hpp"
#include "SafeQueue.hpp"
#include "Utility/Utility.hpp"

namespace Network
{
/** @class Connection
 *  @brief Connection managment class
 *  @details see https://github.com/f1nal3/Juniorgram/wiki/Legacy-Frontend-network \
 *  for additional details
 */
class Connection : public std::enable_shared_from_this<Connection>
{
public:
    /** @enum OwnerType
     *  @brief A connection is "owned" by either a server or a client. /
        And its behaviour is slightly different bewteen the two.
     */
    enum class OwnerType
    {
        SERVER,  /// owner is server
        CLIENT   /// owner is client
    };

private:
    /// Connection "owner"
    OwnerType mOwner = OwnerType::SERVER;
    /// Connection id
    uint64_t mID = uint64_t();

    /// Unique socket to remote
    asio::ip::tcp::socket mSocket;
    /// Overall context for the whole asio instance
    asio::io_context& mContextLink;

    /// References to the incoming queue of the parent object
    SafeQueue<Message>& mIncomingMessagesQueueLink;
    /// Queue all messages to be sent to the remote side of this connection
    SafeQueue<Message> mOutcomingMessagesQueue;

    /// Buffer to store the part of incoming message while it is read
    Message mMessageBuffer;

    /// Alias for the error handler functor
    typedef std::function<void(std::error_code)> handler;

    /**
     * @brief Method for sending message header.
     * @details Function asio::async_write is used to write the header of the message /
     * to the socket. /
     * If the writing header to the socket is successful and the message body isn't empty, /
     * the body of the message is written (method writeBody()). If it is empty, /
     * the current message is removed from message queue to send and next message header /
     * from the message queue is sent. If the writing header to the socket failed, the error /
     * message - "[connection id] Write Header Fail." is displayed.
     */
    void writeHeader()
    {
        const handler writeHeaderHandler = [this](std::error_code error) {
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
        };

        asio::async_write(
            mSocket,
            asio::buffer(&mOutcomingMessagesQueue.front().mHeader, sizeof(Message::MessageHeader)),
            bind(writeHeaderHandler, std::placeholders::_1));
    }

    /**
     * @brief Method for sending message body.
     * @details Function asio::async_write is used to write the body of the message /
     * to the socket. /
     * If the writing header to the socket is successful, the body of the message /
     * is written and the current message is removed from message queue to send. /
     * Then next message header from the message queue is sent (method writeHeader()). /
     * If the writing message body to the socket failed, the error message - /
     * "[connection id] Write Body Fail." is displayed.
     */
    void writeBody()
    {
        const handler writeBodyHandler = [this](std::error_code error) {
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
        };

        asio::async_write(mSocket,
                          asio::buffer(mOutcomingMessagesQueue.front().mBody.data(),
                                       mOutcomingMessagesQueue.front().mBody.size()),
                          bind(writeBodyHandler, std::placeholders::_1));
    }

    /**
     * @brief Method for getting message header.
     * @details Function asio::async_read is used to read the header of the message /
     * from the socket. /
     * If the reading header from the socket is successful and the received /
     * size of message body isn't null, the message body vector is resized according to /
     * received body size and the body of the message is reading (methon readBody()). /
     * If the received size of message body is null, this message is added to /
     * the connection incoming message queue.
     * If the reading message header from the socket failed, the error message - /
     * "[connection id] Read Header Fail." is displayed.
     */
    void readHeader()
    {
        const handler readHeaderHandler = [this](std::error_code error) {
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
        };

        asio::async_read(mSocket,
                         asio::buffer(&mMessageBuffer.mHeader, sizeof(Message::MessageHeader)),
                         bind(readHeaderHandler, std::placeholders::_1));
    }

    /**
     * @brief Method for getting message body.
     * @details Function asio::async_read is used to read the body of the message /
     * from the socket. /
     * If the reading body from the socket is successful, this message /
     * is added to the connection incoming message queue.
     * If the reading message body from the socket failed, the error message - /
     * "[connection id] Read Body Fail." is displayed.
     */
    void readBody()
    {
        const handler readBodyHandler = [this](std::error_code error) {
            if (!error)
            {
                addToIncomingMessageQueue();
            }
            else
            {
                std::cout << "[" << mID << "] Read Body Fail.\n";
                mSocket.close();
            }
        };

        asio::async_read(mSocket,
                         asio::buffer(mMessageBuffer.mBody.data(), mMessageBuffer.mBody.size()),
                         bind(readBodyHandler, std::placeholders::_1));
    }

    /**
     * @brief Method for adding to the connection incoming message queue.
     * @details When afull message is received, it is added to the incoming queue. /
     * It is shoved in the queue, converting to an "owned message", by initializing with /
     * the a shared pointer from this connection object for Server side /
     * and without shared pointer from this for Client side. /
     * Next method readHeader() is called to read other messages.
     */
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
    /**
     * @brief Constructor for class Connection
     * @details It specifies owner, connects to context, transfers the socket and /
     * provides reference to incoming message queue.
     * @param owner - connection "owner"
     * @param contextLink - context for the whole asio instance
     * @param socket - unique socket to remote
     * @param incomingMessagesQueueLink - reference to the incoming message queue
     */
    Connection(const OwnerType& owner, asio::io_context& contextLink, asio::ip::tcp::socket socket,
               SafeQueue<Message>& incomingMessagesQueueLink)
        : mOwner(owner),
          mSocket(std::move(socket)),
          mContextLink(contextLink),
          mIncomingMessagesQueueLink(incomingMessagesQueueLink)
    {
    }

    /**
     * @brief Method getting connection id.
     * @details This id is used system wide - it is how clients will understand other clients /
     * whole system.
     * @return mId - connection id.
     */
    uint64_t getID() const { return mID; }

    /**
     * @brief Method for connection to clients from server side.
     * @details Only server side is allowed to connect to other clients.
     * @param uid - connection id
     */
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

    suppressWarning(4100, -Wunused - parameter)
    /**
     * @brief Method for connection to server from client side.
     * @details Only clients can connect to servers and make a request asio attempts \
     * to connect to an endpoint.
     * @param endpoint - result type returned by resolver
     */
    void connectToServer(const asio::ip::tcp::resolver::results_type& endpoint)
    {
        if (mOwner == OwnerType::CLIENT)
        {
            asio::async_connect(mSocket, endpoint,
                                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
                                    if (!ec)
                                    {
                                        readHeader();
                                    }
                                });
        }
    }
    restoreWarning

    /**
     * @brief Method for closing connection if it is opened.
     * @details It checks if there is a connection with smb/smth. \
     * If the connection is present, function asio::post is called, \
     * because the current context is holding locks and \
     * the function should be called after they have been released. This would allow \
     * the function to acquire those locks itself without causing a deadlock.
     */
    void disconnect()
    {
        if (isConnected())
        {
            asio::post(mContextLink, [this]() { mSocket.close(); });
        }
    }

    /**
     * @brief Method for checking if current socket is open.
     * @return if current socket is open (true/false).
     */
    bool isConnected() const { return mSocket.is_open(); }

    /**
     * @brief Method for sending messages.
     * @details The message is added to the queue of outcoming messages. If there are no messages /
     * available to be written, then the process of writing the message at the front of the queue /
     * is started.
     * @param message - link on message for sending
     */
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
}  // namespace Network
