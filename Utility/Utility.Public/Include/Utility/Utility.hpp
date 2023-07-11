#pragma once

#include <algorithm>
#include <string>
#include <cctype>
#include <ctime>
#include <mutex>
#include <type_traits>
#include <filesystem>
#include <random>

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
    SUCCESS
};

/**
 * @brief Template enum
 */
enum class GeneralCodes : std::uint8_t
{
    SUCCESS,
    FAILED
};


/**
 * @brief Enum for tracking message's storing status.
 * @details Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) /
 *    return one of this codes.
 */
using  StoringMessageCodes = GeneralCodes;

/**
 * @brief Enum for tracking replies storing status.
 * @details Utility::StoringReplyCodes storeReply(const Network::ReplyInfo& ri) /
 *    return one of this codes.
 */
using StoringReplyCodes = GeneralCodes;

/**
 * @brief Enum for tracking message's deleting status.
 * @details Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) /
 *    return one of this codes.
 */
using DeletingMessageCodes  = GeneralCodes;

/**  @brief Enum for tracking message's editing status.
 *   @details Utility::EditingMessageCodes editMessage(const Network::EditMessageInfo& emi) /
 *    return one of this codes.
 */
using EditingMessageCodes  = GeneralCodes;

/**
 * @brief Enum for tracking channel's subscribe status.
 * @details Utility::ChannelSubscribeCodes.
 */
enum class ChannelSubscribingCodes : std::uint8_t
{
    CHANNEL_HAS_ALREADY_BEEN_SIGNED,
    SUCCESS,
    FAILED
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
    FAILED
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
    FAILED
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
    FAILED
};

/**
 * @brief Direct message chat creation status
 */
using  DirectMessageStatus  = GeneralCodes;

/**
 * @brief enum for tracking message's reaction status
 * @details Utility::ReactionMessageCodes updateMessageReactions(const Network::MessageInfo& mi) /
 *    returns one of these codes.
 */
using ReactionMessageCodes  = GeneralCodes;

/**
 * @brief   Removes redundant whitespaces
 * @param   input std::string - bunch of text that should be trimmed
 * @details Removes whitespaces at the beginning and the end of input string, replaces whitespace
 *          sequences with a single space or newline (if sequence contains a newline)
 */
inline std::string removeSpaces(std::string_view input)
{
    auto isSpace = [](char c) -> bool { return std::isspace(static_cast<unsigned char>(c)); };

    std::string result;
    result.reserve(input.size());

    auto sequenceStart = std::find_if_not(input.cbegin(), input.cend(), isSpace);
    auto sequenceEnd   = sequenceStart;

    while (sequenceStart < input.end())
    {
        if (bool spaceSequence = isSpace(*sequenceStart))
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
 * @brief Get the name of the logger folder
 * @return "Log\\"
 */
inline std::string getFldPath(std::string_view folderPath = "Log")
{
    std::filesystem::path _path = folderPath;
    if (! std::filesystem::exists(_path))
        std::filesystem::create_directory(_path);
    return std::filesystem::absolute(_path).string();
}

/*@class UniformIntGenerator
* @brief Generator of random integer numbers with uniform distribution, based on mersenne_twister_engine
*/

template <typename IntType>
class UniformIntGenerator
{
public:
    IntType getRandomInt(IntType min, IntType max)
    {
        std::uniform_int_distribution<IntType> _distance(min, max);
        return _distance(_randGenerator);
    }

private:
    std::random_device _randDevice;
    std::mt19937 _randGenerator = std::mt19937(_randDevice());
};

}  // namespace Utility
