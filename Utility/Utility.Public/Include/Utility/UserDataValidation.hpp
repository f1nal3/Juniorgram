#pragma once
#include <regex>
#include <string>

namespace UserDataValidation
{
    /** @brief Check user's login.
     * @param string login which contains user's login.
     * @return true if login is valid.
     * @details Login must have: /
     *   -First char is a letter; /
     *   -Lowercase letters; /
     *   -Minimum 5 chars. /
     * Example: login = 'some_nick34'.
     */
    inline bool isLoginValid(const std::string& login)
    {
        const std::regex pattern("([a-z]{1})([a-z0-9_-]{4,})");
        return std::regex_match(login, pattern);
    }

    /** @brief Check user's email on valid.
     * @param string email which contains user's email.
     * @return true if email is valid.
     * @details Email must: /
     *   -have first char like letter; /
     *   -maintain the following rule: /
     *     Some chars + '@' + some chars(mail provider) + '.' + lowercase 
     *     lettets (miminmum - 2; maximum - 4). /
     * Example: email = 'Anton3_antonov4@epam.com'.
     */
    inline bool isEmailValid(const std::string& email)
    {
        const std::regex pattern(R"(([a-zA-Z]{1})(\w+@\w+\.)([a-z]{2,4}))");
        return std::regex_match(email, pattern);
    }

    /** @brief Check user's password.
     * @param string password which contains user's password.
     * @return true if password is valid.
     * @details Password must have: /
     *   -At least one uppercase letter; /
     *   -at least one lowercase letter; /
     *   -at least one number; /
     *   -minimum 8 characters. /
     * Example: password = '1996a*nton-Anton%ov_34'.
     */
    inline bool isPasswordValid(const std::string& password)
    {
        const std::regex pattern("(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9]).{8,}");
        return std::regex_match(password, pattern);
    }

}  // namespace UserDataValidation
