#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include <type_traits>

namespace Utility
{
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


    template <typename T = char*>
    struct is_c_string
        : public std::disjunction<
            std::is_same<char*, typename std::decay_t<T>>,
            std::is_same<const char*, typename std::decay_t<T>>,
            std::is_same<wchar_t*, typename std::decay_t<T>>,
            std::is_same<const wchar_t*, typename std::decay_t<T>>
        >
    {};

    template <typename T = std::string>
    struct is_cpp_string
        : public std::disjunction<
            std::is_same<std::string, typename std::decay_t<T>>,
            std::is_same<const std::string, typename std::decay_t<T>>,
            std::is_same<std::wstring, typename std::decay_t<T>>,
            std::is_same<const std::wstring, typename std::decay_t<T>>
        >
    {};

    template <typename T = std::string>
    struct is_string 
        : public std::disjunction<
            is_c_string<T>,
            is_cpp_string<T>
        >
    {};

    std::string& CheckingForInnerSingleQuotes(std::string& arg)
    {
        std::size_t pos = arg.find('\'');
        while (static_cast<long unsigned int>(pos) != -1)
        {
            arg.insert(pos, "'");

            pos = arg.find("'", pos + 2);
        }

        return arg;
    }

    template <typename T>
    const T& CheckForSQLSingleQuotesProblem(T& arg)
    {
        if (is_string<decltype(arg)>::value)
        {
            std::string tempStr = arg;

            // Checking for inner quotes and after wrapping whole string with quotes.
            CheckingForInnerSingleQuotes(tempStr).insert(0, "'").push_back('\'');

            arg = tempStr.c_str();
        }

        return arg;
    }
    }  // namespace Utility
