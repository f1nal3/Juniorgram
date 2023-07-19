#pragma once

#include <map>
#include <cryptopp/secblock.h>

#include "FileLogger.hpp"
#include "Utility/Utility.hpp"

namespace Base
{
using Utility::GeneralCodes;
using Base::Logger::FileLogger;
using Base::Logger::LogLevel;
using CryptoPP::SecByteBlock;
using Pair_UserKey = std::pair<uint64_t, SecByteBlock>;
using Map_UserKey  = std::map<uint64_t, SecByteBlock>;

 /** @class SessionKeyHolder
 * @brief Class contains session key on client-side or map of users session key on server-side
 * @details This is Meyer's singleton, so, according to the standard C++11, it's thread-safe.
 */
class SessionKeyHolder
{
public:
    /** @brief Method for returning singleton object
     * @details Every access to sigleton object begins with a call to this method.
     * @code
     *   SessionKeyHolder::Instance().desiredMethod
     * @endcode
     */
    static SessionKeyHolder& Instance()
    {
        static SessionKeyHolder singleton;
        return singleton;
    }

    /* @brief Method to set key
    * @param newKey key is result of key agreement protocol
    * @param userId id, which is contained in Connection class
    */
    GeneralCodes setKey(SecByteBlock&& newKey, const uint64_t& userId)
    {
        auto result    = _keysContainer.insert(Pair_UserKey(userId, std::move(newKey)));
        bool isSuccess = result.second;

        if (!isSuccess)
        {
            FileLogger::getInstance().log(
                std::string("Cannot set session key for userId = ") + std::to_string(userId),
                LogLevel::ERR);
            return GeneralCodes::FAILED;
        }

        return GeneralCodes::SUCCESS;
    }

    /// @brief Method of getting session key by user ID
    SecByteBlock getKey(const uint64_t& userId) const
    {
        if (auto findedUser = _keysContainer.find(userId);
            findedUser != _keysContainer.end())
        {
            return findedUser->second;
        }
        else
        {
            FileLogger::getInstance().log(
                std::string("Cannot get session key for userId = ") + std::to_string(userId),
                LogLevel::ERR);
        }
        return SecByteBlock(0);
    }

    /// @brief Method of removing session key by user ID
    void removeKey(const uint64_t& userId)
    {
        auto countOfRemovedElem = _keysContainer.erase(userId);
        if (!countOfRemovedElem)
        {
            FileLogger::getInstance().log(
                std::string("Cannot remove session key for userId = ") + std::to_string(userId),
                LogLevel::ERR);
        }
    }

    /** @brief Method for refreshing user's key
    * @details Method finds already connected user and change the key without the need to add a new user-key pair.
    */
    GeneralCodes refreshKey(SecByteBlock&& newKey, const uint64_t& userId)
    {
        if (auto findedUser = _keysContainer.find(userId);
            findedUser !=_keysContainer.end())
        {
            findedUser->second = std::move(newKey);
            return GeneralCodes::SUCCESS;
        }
        else
        {
            FileLogger::getInstance().log(
                std::string("Cannot refresh session key for userId = ") + std::to_string(userId),
                LogLevel::ERR);
        }
        return GeneralCodes::FAILED;
    }

private:
    Map_UserKey _keysContainer;
    SessionKeyHolder()                        = default;
    ~SessionKeyHolder()                       = default;
    SessionKeyHolder(const SessionKeyHolder&) = delete;
    SessionKeyHolder& operator=(const SessionKeyHolder&) = delete;
};
}  // namespace Base
