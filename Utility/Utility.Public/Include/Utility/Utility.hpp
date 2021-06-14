#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include <type_traits>

namespace Utility
{
    /**  @brief Enum for tracking registration status.
     *   @details RegistrationCodes registerUser(const Network::RegistrationInfo& rm) const /
     *    return one of this codes.
     */
    enum class RegistrationCodes : std::uint8_t
    {
        EMAIL_ALREADY_EXISTS = 0,
        LOGIN_ALREADY_EXISTS,
        SUCCESS,
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

    template <typename T>
    struct is_char
        : public std::disjunction<
            std::is_same<char,                    typename std::decay_t<T>>,
            std::is_same<const char,              typename std::decay_t<T>>,
            std::is_same<wchar_t,                 typename std::decay_t<T>>,
            std::is_same<const wchar_t,           typename std::decay_t<T>>
        >
    {};

    template <typename T>
    struct is_c_string
        : public std::disjunction<
            std::is_same<char*,                   typename std::decay_t<T>>,
            std::is_same<const char*,             typename std::decay_t<T>>,
            std::is_same<wchar_t*,                typename std::decay_t<T>>,
            std::is_same<const wchar_t*,          typename std::decay_t<T>>
        >
    {};

    template <typename T>
    struct is_std_string
        : public std::disjunction<
            std::is_same<std::string,             typename std::decay_t<T>>,
            std::is_same<const std::string,       typename std::decay_t<T>>,
            std::is_same<std::wstring,            typename std::decay_t<T>>,
            std::is_same<const std::wstring,      typename std::decay_t<T>>,
            std::is_same<std::string_view,        typename std::decay_t<T>>,
            std::is_same<const std::string_view,  typename std::decay_t<T>>,
            std::is_same<std::wstring_view,       typename std::decay_t<T>>,
            std::is_same<const std::wstring_view, typename std::decay_t<T>>
        >
    {};

    template <typename T>
    struct is_string 
        : public std::disjunction<
            is_c_string<T>,
            is_std_string<T>
        >
    {};

    template <typename T>
    struct is_char_or_string
        : public std::disjunction<
            is_char<T>,
            is_string<T>
        >
    {};

    template <typename T>
    inline constexpr bool is_char_v = is_char<T>::value;
    template <typename T>
    inline constexpr bool is_string_v = is_string<T>::value;
    template <typename T>
    inline constexpr bool is_char_or_string_v = is_char_or_string<T>::value;

    template <typename T>
    auto CheckForSQLSingleQuotesProblem(T&& arg)
    {
        if constexpr (is_char_or_string_v<decltype(arg)>)
        {
            std::string tempStr;
            tempStr += std::forward<T>(arg);

            // Checking for inner quotes.
            std::size_t pos = tempStr.find('\'');
            while (pos != std::string::npos)
            {
                tempStr.insert(pos, "'");

                pos = tempStr.find('\'', pos + 2);
            }


            // Wrapping whole string with quotes.
            tempStr.insert(0, "'").push_back('\'');
            
            return tempStr;
        }
        else
        {
            return arg;
        }
    }
}  // namespace Utility
