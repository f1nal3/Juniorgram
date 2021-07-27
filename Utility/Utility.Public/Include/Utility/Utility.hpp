#pragma once

#include <ctime>
#include <mutex>
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

    };

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
