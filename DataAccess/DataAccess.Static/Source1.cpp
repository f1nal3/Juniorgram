#include <iostream>

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

#include "DataAccess/IRepository.hpp"
#include "PostgreAbstractionLayout.hpp"

class Response
{
public:
    enum class ResponseCodes : std::uint8_t
    {
        SUCCESS,
        INCORRECT_AUTH_DATA,
        USER_ALREADY_EXIST,
        INVALID_TOKEN,
        SESSION_EXPIRED,
    };

public:
    Response()                = default;
    Response(const Response&) = delete;
    ~Response()               = default;

    /*ResponseCodes authorizeUser(const std::string& loginOrEmail, const std::string& passwordHash,
                                std::string& token, std::string& refreshToken);*/

    ResponseCodes registerUser(const std::string& email, const std::string& login,
                               const std::string& passwordHash, const std::string& token,
                               const std::string& refreshToken)
    {
        // Check on login and email existing in repository.
        auto recordsAmountRow = DataAccess::Table("users")
                                    .Select()
                                    ->columns({"COUNT(*)"})
                                    ->where("email = '" + email + "'")
                                    ->Or("login = '" + login + "'")
                                    ->execute();

        std::size_t recordsAmount = recordsAmountRow.value()[0][0].as<std::size_t>();
        if (recordsAmount > 0)
        {
            return ResponseCodes::USER_ALREADY_EXIST;
        }

        // User registration.
        std::tuple messageToDatabase
        {
            std::pair{"login", login},
            std::pair{"email", email},
            std::pair{"password_hash", passwordHash},
            std::pair{"token", token},
            std::pair{"refresh_token", refreshToken},
            std::pair{"roken_receipt_time", "tok_rec_t"},
        };

        DataAccess::Table("users").Update()->fields(messageToDatabase)->execute();
        return ResponseCodes::SUCCESS;
    }

    /*ResponseCodes validateToken(std::string& authToken){}

    ResponseCodes logout(std::string& token) {}

    ResponseCodes refresh(const std::string& RefreshTokenDB, std::string& newToken,
                          std::string& newRefreshToken)
    {
    }*/
};

int main() 
{
	/*Response r;
    
    Response::ResponseCodes code = r.registerUser("lol@gmail.com", "test_1", "asd", "fgsdfg", "sdfgsdfhsdhsgdhgf");
    if (code == Response::ResponseCodes::SUCCESS)
    {
        std::cout << "SUccesSS" << std::endl;
    }*/

    return 0;
}