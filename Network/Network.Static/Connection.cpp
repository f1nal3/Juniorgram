#include "Connection.hpp"

#include <Utility.Static/YasSerializer.hpp>
#include <Utility.Static/CompressionHandler.hpp>
#include <Utility.Static/SerializationHandler.hpp>
#include <Utility.Static/KeyDestributor.hpp>
#include <Utility.Static/EncryptionHandler.hpp>

namespace Network
{
    Connection::Connection(const OwnerType& owner, asio::io_context& contextLink,
                    asio::ip::tcp::socket socket,
                       SafeQueue<Message>& incomingMessagesQueueLink)
    : mOwner(owner),
      mSocket(std::move(socket)),
      mContextLink(contextLink),
      mIncomingMessagesQueueLink(incomingMessagesQueueLink)
    {
    }
    
    uint64_t Connection::getID() const { return mConnectionID; }

    void Connection::connectToClient(const uint64_t uid)
    {
        if (mOwner == OwnerType::SERVER)
        {
            if (mSocket.is_open())
            {
                mConnectionID = uid;
                readHeader();
            }
        }
    }

    void Connection::connectToServer(const asio::ip::tcp::resolver::results_type& endpoint)
    {
        if (mOwner == OwnerType::CLIENT)
        {
            asio::async_connect(mSocket, endpoint,
            [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
                if (!ec)
                {
                    endpoint;
                    readHeader();
                }
            });
}
    }

    void Connection::disconnect()
    {
        if (isConnected())
        {
            asio::post(mContextLink, [this]() { mSocket.close(); });
        }
    }

    bool Connection::isConnected() const { return mSocket.is_open(); }


    void Connection::send(const Message& message)
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

    std::unique_ptr<Utility::KeyDestributor>& Connection::getKeyDestributor()
    {
        if (mKeyDestributor == nullptr)
        {
            mKeyDestributor = std::make_unique<Utility::KeyDestributor>();
        }
        return mKeyDestributor;
    }

    std::string Connection::getIP()
    { 
        return mSocket.remote_endpoint().address().to_string(); 
    }

    void Connection::writeHeader()
    {
        yas::shared_buffer bodyBuffer;

        Utility::SerializationHandler handler;

        // Needs improvements in this place
        if (mOutcomingMessagesQueue.front().mHeader.mMessageType ==
                Message::MessageType::SetEncryptedConnection ||
            mOutcomingMessagesQueue.front().mHeader.mMessageType ==
                Message::MessageType::ServerAccept ||
            mOutcomingMessagesQueue.front().mHeader.mMessageType ==
                Message::MessageType::SendIV)
        {
            handler.setNext(new Utility::CompressionHandler());
        }
        else
        {
            handler.setNext(new Utility::EncryptionHandler())
                ->setNext(new Utility::CompressionHandler());
        }
        // Needs improvements in this place~

        Utility::MessageProcessingState result = Utility::MessageProcessingState::SUCCESS;

        if (mOutcomingMessagesQueue.front().mBody.has_value())
        {
            result = handler.handleOutcomingMessage(
                const_cast<Network::Message&>(mOutcomingMessagesQueue.front()), bodyBuffer);
        }

     Message::MessageHeader outcomingMessageHeader =
            mOutcomingMessagesQueue.front().mHeader;
        outcomingMessageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);

        if (result == Utility::MessageProcessingState::SUCCESS)
        {
            const auto writeHeaderHandler = [this, bodyBuffer](std::error_code error) {
                if (!error)
                {
                    if (mOutcomingMessagesQueue.front().mBody.has_value())
                    {
                        writeBody(bodyBuffer);
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
                    std::cout << "[" << mConnectionID << "] Write Header Fail.\n";
                    mSocket.close();
                }
            };

            asio::async_write(mSocket,
                asio::buffer(&outcomingMessageHeader, sizeof(Message::MessageHeader)),
                              std::bind(writeHeaderHandler, std::placeholders::_1));
        }
    }

    void Connection::writeBody(yas::shared_buffer buffer)
    {
        const auto writeBodyHandler = [this](std::error_code error) {
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
                std::cout << "[" << mConnectionID << "] Write Body Fail.\n";
                mSocket.close();
            }
        };

        asio::async_write(mSocket, asio::buffer(buffer.data.get(), buffer.size),
                          std::bind(writeBodyHandler, std::placeholders::_1));
    }

    void Connection::readHeader()
    {
        const auto readHeaderHandler = [this](std::error_code error) {
            if (!error)
            {
                if (mMessageBuffer.mHeader.mBodySize > 0)
                {
                    readBody(mMessageBuffer.mHeader);
                }
                else
                {
                    addToIncomingMessageQueue();
                }
            }
            else
            {
                std::cout << "[" << mConnectionID << "] Read Header Fail.\n";
                mSocket.close();
            }
        };

        asio::async_read(mSocket, asio::buffer(&mMessageBuffer.mHeader, sizeof(Message::MessageHeader)),
                         std::bind(readHeaderHandler, std::placeholders::_1));
    }

    void Connection::readBody(Message::MessageHeader& header)
    {
        yas::shared_buffer buffer;
        buffer.resize(header.mBodySize);

        const auto readBodyHandler = [this, buffer, header](std::error_code error) {
            if (!error)
            {
                Utility::CompressionHandler handler;

                if (header.mMessageType == Message::MessageType::SetEncryptedConnection ||
                    header.mMessageType == Message::MessageType::ServerAccept ||
                    header.mMessageType == Message::MessageType::SendIV)
                {
                    handler.setNext(new Utility::SerializationHandler());
                }
                else
                {
                    handler.setNext(new Utility::EncryptionHandler())
                        ->setNext(new Utility::SerializationHandler());
                }

                Utility::MessageProcessingState result =
                    handler.handleIncomingMessageBody(buffer, mMessageBuffer);

                if (result == Utility::MessageProcessingState::SUCCESS)
                {
                    addToIncomingMessageQueue();
                }
            }
            else
            {
                std::cout << "[" << mConnectionID << "] Read Body Fail.\n";
                mSocket.close();
            }
        };

        asio::async_read(mSocket, asio::buffer(buffer.data.get(), buffer.size),
                         std::bind(readBodyHandler, std::placeholders::_1));
    }

    void Connection::addToIncomingMessageQueue()
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
   
}
