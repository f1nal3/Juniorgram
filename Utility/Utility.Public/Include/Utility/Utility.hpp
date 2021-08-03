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
        EMAIL_ALREADY_EXISTS,
        LOGIN_ALREADY_EXISTS,
        SUCCESS,
    };

    /**  @brief Enum for tracking message's storing status.
     *   @details Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) /
     *    return one of this codes.
     */
    enum class StoringMessageCodes : std::uint8_t
    {
        SUCCESS,
        FAILED,
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

    inline std::string getTimeNow()
    {
        std::string timeStampStr(20, '\0');

        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm time  = Utility::safe_localtime(t);
        std::strftime(timeStampStr.data(), timeStampStr.size(), "%Y-%m-%d %H:%M:%S", &time);

        return timeStampStr;
    }

}  // namespace Utility
