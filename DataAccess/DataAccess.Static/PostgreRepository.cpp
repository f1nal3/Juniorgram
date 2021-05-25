#include "PostgreRepository.hpp"
#include <random>

using namespace DataAccess;

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;

    auto channelListRow = std::get<0>(PTable("channels").Select()->columns({"channel_name"})->execute());

    if (channelListRow.has_value())
    {
        for (auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() << '\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }
    }

    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    std::vector<std::string> result;

    auto messageHistoryRow = 
        std::get<0>(PTable("channel_msgs").Select()->columns({"msg"})->Where("channel_id = " + std::to_string(channelID))->execute());


    if (messageHistoryRow.has_value())
    {
        for (auto message : messageHistoryRow.value())
        {
            std::cout << message.at(0).c_str() << '\n';
            result.emplace_back(std::string(message.at(0).c_str()));
        }
    }

    return result;
}

void PostgreRepository::storeMessage(const Network::MessageInfo& message, const std::uint64_t channelID)
{
    char timeStampStr[35];

    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(timeStampStr, 35, "%Y-%m-%d %H:%M:%S.0+00", &time);

    std::tuple messageToDatabase
    {
        std::pair{"channel_id", channelID},
        std::pair{"sender_id", message.userID},
        std::pair{"send_time", timeStampStr}, 
        std::pair{"msg", message.message}
    };
    PTable("channel_msgs").Insert()->columns(messageToDatabase)->execute();
}

class Response
{
private:
    const uint16_t TOKEN_RECEIPT_TIME = 7;
    const uint16_t TOKEN_LENGTH       = 32;

public:
    enum class ResponseCodes : std::uint8_t
    {
        SUCCESS,
        INCORRECT_AUTH_DATA,
        EMAIL_ALREADY_EXISTS,
        LOGIN_ALREADY_EXISTS,
        INVALID_TOKEN,
        SESSION_EXPIRED,
    };

public:
    Response()                = default;
    Response(const Response&) = delete;
    ~Response()               = default;

    ResponseCodes registerUser(const std::string& email, const std::string& login,
                               const std::string& passwordHash)
    {
        // Check on existing of login and email in repository.
        auto checkExistingUsers = [&](const std::string& WHERE, const ResponseCodes code) 
        {
            auto recordsRowAmount = std::get<0>(PTable("users")
                                                 .Select()
                                                 ->columns({"COUNT(*)"})
                                                 ->Where(WHERE)
                                                 ->execute());

            std::size_t usersAmount = recordsRowAmount.value()[0][0].as<std::size_t>();
            
            if (usersAmount > 0) { throw code; }
        };

        checkExistingUsers("email = '" + email + "'", ResponseCodes::EMAIL_ALREADY_EXISTS);
        checkExistingUsers("login = '" + login + "'", ResponseCodes::LOGIN_ALREADY_EXISTS);

        std::tuple userData
        {
            std::pair{ "email", email },
            std::pair{ "login", login },
            std::pair{ "password_hash", passwordHash }
        };

        PTable("users").Insert()->columns(userData)->execute();

        return ResponseCodes::SUCCESS;
    }

    std::string createToken(const std::uint64_t userID) const
    {
        std::string userIDstr = std::to_string(userID);

        std::string possibleCharacters =
            "!@#$%^&*()<>-=/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        std::mt19937 generator(std::random_device{}());

        std::shuffle(possibleCharacters.begin(), possibleCharacters.end(), generator);

        for (size_t i = 0; i < userIDstr.size(); ++i)
        {
            possibleCharacters[i] = userIDstr[i];
        }

        return possibleCharacters.substr(0, TOKEN_LENGTH);
    }
};
