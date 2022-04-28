#pragma once

#include <algorithm>
#include <cctype>
#include <ctime>
#include <mutex>
#include <string>
#include <type_traits>

namespace Utility
{
/**
 * @brief Enum for tracking registration status.
 * @details RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const /
 *    return one of this codes.
 */
enum class RegistrationCodes : std::uint8_t
{
    EMAIL_ALREADY_EXISTS,
    LOGIN_ALREADY_EXISTS,
    SUCCESS,
};

/**
 * @brief Enum for tracking message's storing status.
 * @details Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) /
 *    return one of this codes.
 */
enum class StoringMessageCodes : std::uint8_t
{
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking replies storing status.
 * @details Utility::StoringReplyCodes storeReply(const Network::ReplyInfo& ri) /
 *    return one of this codes.
 */
enum class StoringReplyCodes : std::uint8_t
{
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking message's deleting status.
 * @details Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) /
 *    return one of this codes.
 */
enum class DeletingMessageCodes : std::uint8_t
{
    SUCCESS,
    FAILED,
};

/**  @brief Enum for tracking message's editing status.
 *   @details Utility::EditingMessageCodes editMessage(const Network::EditMessageInfo& emi) /
 *    return one of this codes.
 */
enum class EditingMessageCodes : std::uint8_t
{
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking channel's subscribe status.
 * @details Utility::ChannelSubscribeCodes.
 */
enum class ChannelSubscribingCodes : std::uint8_t
{
    CHANNEL_HAS_ALREADY_BEEN_SIGNED,
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking channel leave status.
 * @details Utility::ChannelLeaveCodes leaveChannel(const std::string channelName) /
 *    return one of this codes.
 */
enum class ChannelLeaveCodes : std::uint8_t
{
    CHANNEL_NOT_FOUND,
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking channel leave status.
 * @details Utility::ChannelDeleteCode deleteChannel(const std::string channelName) /
 *    return one of this codes.
 */
enum class ChannelDeleteCode : std::uint8_t
{
    CHANNEL_NOT_FOUND,
    CHANNEL_IS_NOT_USER,
    SUCCESS,
    FAILED,
};

/**
 * @brief Enum for tracking channel creating status.
 * @details Utility::ChannelCreateCodes createChannel(const Network::ChannelInfo& channel) /
 *    return one of this codes.
 */
enum class ChannelCreateCodes : std::uint8_t
{
    CHANNEL_ALREADY_CREATED,
    SUCCESS,
    FAILED,
};

/**
 * @brief Direct message chat creation status
 */
enum class DirectMessageStatus : std::uint8_t
{
    SUCCESS,
    FAILED,
};

/**
 * @brief enum for tracking message's reaction status
 * @details Utility::ReactionMessageCodes updateMessageReactions(const Network::MessageInfo& mi) /
 *    returns one of these codes.
 */
enum class ReactionMessageCodes : std::uint8_t
{
    SUCCESS,
    FAILED
};

/**
 * @brief Wrapper for OS implementations of thread-safe std::localtime
 * @details std::localtime return static internal std::tm internally, so it is not thread-safe
 */
inline std::tm safe_localtime(const std::time_t& time)
{
    // std::localtime is not thread safe, so we use platform-dependant versions

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

inline std::string getTimeNow()
{
    std::string timeStampStr(20, '\0');

    std::time_t time        = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm     timeConvert = Utility::safe_localtime(time);
    std::strftime(timeStampStr.data(), timeStampStr.size(), "%Y-%m-%d %H:%M:%S", &timeConvert);

    return timeStampStr;
}

/**
 * @brief   Removes redundant whitespaces
 * @param   Input string as input
 * @details Removes whitespaces at the beginning and the end of input string, replaces whitespace
 *          sequences with a single space or newline (if sequence contains a newline)
 */
inline std::string removeSpaces(const std::string& input)
{
    auto isSpace = [](char c) -> bool { return std::isspace(static_cast<unsigned char>(c)); };

    std::string result;
    result.reserve(input.size());

    auto sequenceStart = std::find_if_not(input.cbegin(), input.cend(), isSpace);
    auto sequenceEnd   = sequenceStart;

    while (sequenceStart < input.end())
    {
        bool spaceSequence = isSpace(*sequenceStart);

        if (spaceSequence)
        {
            char singleSpace = ' ';

            for (; (sequenceEnd < input.end()) && isSpace(*sequenceEnd); ++sequenceEnd)
            {
                if (*sequenceEnd == '\n')
                {
                    singleSpace = '\n';
                }
            }

            result.push_back(singleSpace);
        }
        else
        {
            sequenceEnd = std::find_if(sequenceStart, input.cend(), isSpace);
            result.append(sequenceStart, sequenceEnd);
        }

        sequenceStart = sequenceEnd;
    }

    if (!result.empty() && isSpace(result.back()))
    {
        result.pop_back();
    }

    return result;
}

/**
 * @brief Returns time in milliseconds since Epoch (1970-01-01 00:00:00 UTC)
 */
inline std::uint64_t millisecondsSinceEpoch()
{
    auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch).count();
}

}  /// namespace Utility
