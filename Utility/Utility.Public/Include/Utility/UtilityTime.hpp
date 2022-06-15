#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <type_traits>
#include <cctype>
#include <mutex>

namespace UtilityTime
{
/**
* @brief RTC - real time clock
*/
using RTC = std::chrono::system_clock; 

/**
 * @brief timestamp_t - alias that make more understantable format of keeping server-client timestamp represantation.
 */
using timestamp_t = std::uint64_t;

/**
 * @brief Wrapper for OS implementations of thread-safe std::localtime.
 * @details std::localtime return static internal std::tm internally, /
 * so it is not thread-safe and we use platform-dependant versions.
 */
inline std::tm safe_localtime(const std::time_t& time)
{
    std::tm formatted_time{};

#if defined(_MSC_VER)
    localtime_s(&formatted_time, &time);
#elif defined(__unix__)
    localtime_r(&time, &formatted_time);
#else
    static std::mutex           mu;
    std::lock_guard<std::mutex> lock(mu);

    formatted_time = *std::localtime(&time);
#endif

    return formatted_time;
}

/**
* @brief Current time display function.
*/
inline void consoleLogTimestamp()
{
    RTC::time_point currentTime = RTC::now();

    time_t rawTime = std::chrono::system_clock::to_time_t(currentTime);

    std::tm  timeFormat = safe_localtime(rawTime);

    /// MinGW will warning if we put this string directly
    std::cout << "[" << std::put_time(&timeFormat, "%F %T%z") << "]\n";
}

/**
 * @brief Returns the current time in formatted form.
 * @details std::strftime converts the date and time information /
 * from a given calendar time time to a null-terminated multibyte /
 * character string timeStampStr according to format string format.
 */
inline std::string getTimeNow()
{
    std::string timeStampStr(20, '\0');

    std::time_t t    = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm     time = safe_localtime(t);
    std::strftime(timeStampStr.data(), timeStampStr.size(), "%Y-%m-%d %H:%M:%S", &time);

    return timeStampStr;
}

/**
 * @brief Returns time in milliseconds since Epoch (1970-01-01 00:00:00 UTC).
 */
inline timestamp_t millisecondsSinceEpoch() noexcept
{
    auto timeSinceEpoch = std::chrono::time_point_cast<std::chrono::milliseconds>(RTC::now()).time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch).count();
}

/**
* @brief Converts and returns the current time to the date format ("%d.%m.%Y").
* @details std::strftime converts the date and time information /
* from a given calendar time time to a null-terminated multibyte /
* character string stringifyedDate according to format string format.
*/
inline std::string getStringifiedCurrentDate()
{
    using std::chrono::system_clock;

    std::string stringifyedDate(20, '\0');
    std::time_t raw_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time    = safe_localtime(raw_time);

    std::strftime(stringifyedDate.data(), stringifyedDate.size(), "%d.%m.%Y", &time);

    return stringifyedDate;
}
}  // namespace UtilityTime

