#include <Client.hpp>
#include <string>

/**
 * @class App(Client.CLI)
 * @brief App(Client.CLI) class which allows
 *        you to work with the console
 *        and process messages from the server.
 */
class App
{
private:
    Network::Client client;

public:
    /**
     * @brief App(Client.CLI) constructor.
     * @details Connects to the server by address and port.
     */
    App(/* args */);

   /**
    * @brief App(Client.CLI) destructor.
    * @details Disconnect client connection.
    */
    ~App();

public:
    /**
     * @brief Method for returning client.
     * @return Network::Client*.
     */
    Network::Client* shell();

    /**
     * @brief Loop method which allows you to process requests from the server,
     *        list of requests look at Network::Message::MessageType.
     * @return Return true if server is down correctly and false if not.
     */
    bool loop();
};
