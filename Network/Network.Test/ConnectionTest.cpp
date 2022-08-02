

#include "Network/Connection.hpp"
#include "Network/SafeQueue.hpp"



TEST_CASE("Connection test"){


    asio::io_context ioContext;
    asio::ip::tcp::socket socket(ioContext);
    Network::SafeQueue<Network::Message> safeQueue;

    SECTION("Constructor test"){

        REQUIRE_NOTHROW(Network::Connection(Network::Connection::OwnerType::CLIENT, ioContext,
                                            asio::ip::tcp::socket(ioContext), safeQueue)
                        );

        REQUIRE_NOTHROW(Network::Connection(Network::Connection::OwnerType::SERVER, ioContext,
                                            asio::ip::tcp::socket(ioContext), safeQueue)
        );
    }

    SECTION("getUserID and setUserID methods"){
        Network::Connection _connection(Network::Connection::OwnerType::SERVER,ioContext,
                                       asio::ip::tcp::socket(ioContext),safeQueue);
        _connection.setUserID(12345);
        REQUIRE(_connection.getUserID() == 12345);
    }

}