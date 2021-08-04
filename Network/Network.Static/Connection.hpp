#pragma once

#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#endif
#define _WIN32_WINNT 0x0601

#include <asio.hpp>
#include <chrono>
#include <iostream>
#include <functional>

#include "Network/SafeQueue.hpp"
#include "Message.hpp"

#include "Utility/Utility.hpp"
#include "Utility/WarningSuppression.hpp"

// clang-format off
suppressWarning(4996, -Wdeprecated-declarations)
suppressWarning(4458, -Wshadow)
#include <yas/buffers.hpp>
restoreWarning
restoreWarning
// clang-format on

namespace Utility
{
class  KeyDestributor;
class SignerAndVerifier;
class  Handler;
class  EncryptionHandler;
class  CompressionHandler;
class  SerializationHandler;
}

namespace Network
{
    /** @class Connection
 *  @brief Connection managment class
 *  @details see https://github.com/f1nal3/Juniorgram/wiki/Legacy-Frontend-network \
 *  for additional details
 */
// using Message = Network::Message;

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

    std::unique_ptr<Utility::KeyDestributor> mKeyDestributor;

    std::unique_ptr<Utility::SignerAndVerifier> mSignerAndVerifier;

    /// Connection "owner"
    OwnerType mOwner = OwnerType::SERVER;
    /// Connection id
    uint64_t mConnectionID = uint64_t();

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
    void writeHeader();

    /**
     * @brief Method for sending message body.
     * @details Function asio::async_write is used to write the body of the message /
     * to the socket. /
     * If the writing header to the socket is successful, the body of the message /
     * is written and the current message is removed from message queue to send. /
     * Then next message header from the message queue is sent (method writeHeader()). /
     * If the writing message body to the socket failed, the error message - /
     * "[connection id] Write Body Fail." is displayed.
     * @param buffer - buffer that contatins sent message body
     */
    void writeBody(yas::shared_buffer buffer);

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
    void readHeader();

    /**
     * @brief Method for getting message body.
     * @details Function asio::async_read is used to read the body of the message /
     * from the socket. /
     * If the reading body from the socket is successful, this message /
     * is added to the connection incoming message queue.
     * If the reading message body from the socket failed, the error message - /
     * "[connection id] Read Body Fail." is displayed.
     * @param bodySize - size of messege body
     */
    void readBody(Message::MessageHeader& header);

    /**
     * @brief Method for adding to the connection incoming message queue.
     * @details When afull message is received, it is added to the incoming queue. /
     * It is shoved in the queue, converting to an "owned message", by initializing with /
     * the a shared pointer from this connection object for Server side /
     * and without shared pointer from this for Client side. /
     * Next method readHeader() is called to read other messages.
     */
    void addToIncomingMessageQueue();
   

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
               SafeQueue<Message>& incomingMessagesQueueLink);
  

    /**
     * @brief Method getting connection id.
     * @details This id is used system wide - it is how clients will understand other clients /
     * whole system.
     * @return mId - connection id.
     */
    uint64_t getID() const;

    /**
     * @brief Method for connection to clients from server side.
     * @details Only server side is allowed to connect to other clients.
     * @param uid - connection id
     */
    void connectToClient(const uint64_t uid = uint64_t());
 

    // clang-format off
    suppressWarning(4100, -Wunused-parameter)
    /**
    * @brief Method for connection to server from client side.
    * @details Only clients can connect to servers and make a request asio attempts \
    * to connect to an endpoint.
    * @param endpoint - result type returned by resolver
    */
    void connectToServer(const asio::ip::tcp::resolver::results_type& endpoint);
    restoreWarning

    /**
    * @brief Method for closing connection if it is opened.
    * @details It checks if there is a connection with smb/smth. \
    * If the connection is present, function asio::post is called, \
    * because the current context is holding locks and \
    * the function should be called after they have been released. This would allow \
    * the function to acquire those locks itself without causing a deadlock.
    */
    void disconnect();
    // clang-format on

    /**
     * @brief Method for checking if current socket is open.
     * @return if current socket is open (true/false).
     */
    bool isConnected() const;
    /**
     * @brief Method for sending messages.
     * @details The message is added to the queue of outcoming messages. If there are no messages /
     * available to be written, then the process of writing the message at the front of the queue /
     * is started.
     * @param message - link on message for sending
     */
    void send(const Message& message);

    std::unique_ptr<Utility::KeyDestributor>& getKeyDestributorInstance();
  
    std::unique_ptr<Utility::SignerAndVerifier>& getSignerAndVerifierInstance();
    
    bool checkSignerAndVerifierExistance();

    std::string getIP();

};
}  // namespace Network
