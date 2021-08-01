#pragma once

#include <ctime>
#include <mutex>
#include <regex>
#include <string>
#include <type_traits>

namespace Utility
{
    /**  @brief Enum for tracking registration status.
     *   @details RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const /
     *    return one of this codes.
     */
    enum class RegistrationCodes : std::uint8_t
    {
        EMAIL_ALREADY_EXISTS = 0,
        LOGIN_ALREADY_EXISTS,
        SUCCESS,
    };

    enum class SessionCodes : std::uint8_t
    {
        SUCCESS
    };


    struct WithoutQuotes
    {
        WithoutQuotes(const std::string& externalStr)
          : internalStr{externalStr}
        {}

       const std::string internalStr;
    };

    inline std::ostream& operator<< (std::ostream &out, const WithoutQuotes& obj)
    {
    out << obj.internalStr;
    return out;
    }


	inline std::string extractPayload(const std::string& currentToken)
    { 
        const std::regex regex("(\\.([A-Za-z0-9-_=]+)\\.)");
        std::smatch regex_iterator;
        if (std::regex_search(currentToken, regex_iterator, regex))
        {
            return regex_iterator.str().substr(1, regex_iterator.str().size() - 2);
           
        }
        return std::string("");
    }

    inline std::tm safe_localtime(const std::time_t& time)
    {
        // std::localtime is not thread safe, so we use platform-dependant versions
    
        std::tm formatted_time{};
    
    #if defined(_MSC_VER)
        localtime_s(&formatted_time, &time);
    #elif defined(__unix__)
        localtime_r(&time, &formatted_time);
    #else
        static std::mutex mu;
        std::lock_guard<std::mutex> lock(mu);
    
        formatted_time = *std::localtime(&time);
    #endif
    
        return formatted_time;
    }
}  // namespace Utility
