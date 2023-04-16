#include <any>
#include <map>
#include <cryptopp/secblock.h>

#include "FileLogger.hpp"
#include "Utility/Utility.hpp"

namespace Base
{
using CryptoPP::SecByteBlock;
using Pair_UserKey = std::pair<uint64_t, SecByteBlock>;
using Map_UserKey  = std::map<uint64_t, SecByteBlock>;

 /** @enum Owner
 * @brief Determines content in SessionKeyHolder class
 */
enum class Owner
{
    SERVER,
    CLIENT
};

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
    /** @brief Method of setting current side
     * @detail The method determines the functionality that will work
     * @warning On client-side you must call this method to determine side
     */
    void setOwnerType(Owner owner_) { _owner = owner_; };

    /** @brief Client's method of setting key
     * @detail This method is used to set the key when connecting or updating the key
     */
    void setKey(SecByteBlock newKey) { _keyHolder = std::make_any<SecByteBlock>(newKey); };

    /// @brief Client's method for getting key
    SecByteBlock getKey() const { return std::any_cast<SecByteBlock>(_keyHolder); };

    /// @brief Server's method to set client key on connection
    void addUserKey(SecByteBlock newKey, uint64_t userId)
    {
        std::any_cast<Map_UserKey>(&_keyHolder)->insert(Pair_UserKey(userId, newKey));
    };
    /** @brief Server's method for refreshing user's key
    * @details Method finds already connected user and change the key without the need to add a new user-key pair \
    * This is draft method. It requires reaction to unsuccessful key update.
    */
    Utility::GeneralCodes refreshUserKey(SecByteBlock newKey, const uint64_t& userId)
    {
        if (auto findedUser = std::any_cast<Map_UserKey>(&_keyHolder)->find(userId);
            findedUser != std::any_cast<Map_UserKey>(&_keyHolder)->end())
        {
            findedUser->second = newKey;
            return Utility::GeneralCodes::SUCCESS;
        }
        else
        {
            Base::Logger::FileLogger::getInstance().log(
                std::string("Cannot refresh session key for userId = ") + std::to_string(userId),
                Base::Logger::LogLevel::ERR);
        }
        return Utility::GeneralCodes::FAILED;
    };
    /// @brief Server's method of removing user session key by user ID
    void removeUserKey(const uint64_t& userId) { std::any_cast<Map_UserKey>(&_keyHolder)->erase(userId); };

    SecByteBlock getUserKey(const uint64_t& userId) { return std::any_cast<Map_UserKey>(_keyHolder).find(userId)->second; };

    Owner getOwner() { return _owner; };

private:
    /** @brief _keyHolder contains session key(s)
    * @detail Depending on the party _keyHolder can contain:
    * - map<User,Key> on server-side
    * - Key on client-side
    */
    std::any _keyHolder;
    /// @brief The owner determines which functionality will be available and what the _keyHolder will contain
    Owner _owner                              = Owner::SERVER;
    SessionKeyHolder()                        = default;
    ~SessionKeyHolder()                       = default;
    SessionKeyHolder(const SessionKeyHolder&) = delete;
    SessionKeyHolder& operator=(const SessionKeyHolder&) = delete;
};
};  // namespace Base
