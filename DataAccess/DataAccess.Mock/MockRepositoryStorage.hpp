#pragma once 
#include <string>
#include <vector>

#include "Cryptography.hpp"

namespace MockObject
{
/**
* @brief A table for storing channel data in the repository.
*/
struct MockChannelsRepositoryData
{
    MockChannelsRepositoryData()  = default;
    ~MockChannelsRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testChannelName;
    std::string _testChannelID;
    std::string _testCreatorID;
};

/**
 * @brief A table for storing replies data in the repository.
 */
struct MockRepliesRepositoryData
{
    MockRepliesRepositoryData()  = default;
    ~MockRepliesRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testChannelID;
    std::string _testMessage;
    std::string _testMsgID;
    std::string _testMsgIDOwner;
    std::string _testSenderID;
    std::string _testLogin;
};

/**
 * @brief A table for storing messages data in the repository.
 */
struct MockDirectMessageRepositoryData
{
    MockDirectMessageRepositoryData()  = default;
    ~MockDirectMessageRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testUserID;
    std::string _testSenderID;
};

/**
 * @brief A table for storing channel messages data in the repository.
 */
struct MockMessagesRepositoryData
{
    MockMessagesRepositoryData()  = default;
    ~MockMessagesRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testChannelID;
    std::string _testMessage;
    std::string _testMsgID;
    std::string _testReactionID;
    std::string _testRecipientID;
    std::string _testSenderID;
    std::string _testLogin;
};

/**
 * @brief A table for storing registration data in the repository.
 */
struct MockRegisterRepositoryData
{
    MockRegisterRepositoryData()  = default;
    ~MockRegisterRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testLogin;
    std::string _testPSWD;
    std::string _testPWDHash{Base::Hashing::SHA_256(_testPSWD, _testLogin)};
};

/**
 * @brief A table for storing login data in the repository.
 */
struct MockLoginRepositoryData
{
    MockLoginRepositoryData()  = default;
    ~MockLoginRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testEmail;
    std::string _testLogin;
    std::string _testPSWD;
};

/**
* @brief MockRepositoryStorage class.
* @details Designed to initialize tables (data strings) that /
*          emulate database storage for a specific repository.
*/
struct MockRepositoryStorage
{
    MockRepositoryStorage()  = default;
    ~MockRepositoryStorage() = default;

    MockChannelsRepositoryData&      getChannelsRepoData() { return _repoData; }
    MockRepliesRepositoryData&       getRepliesRepoData() { return _repliesData; }
    MockDirectMessageRepositoryData& getDirectMessageRepoData() { return _directMessgData; }
    MockMessagesRepositoryData&      getMessageRepoData() { return _messgData; }
    MockRegisterRepositoryData&      getRegisterRepoData() { return _regData; }
    MockLoginRepositoryData&         getLoginRepoData() { return _loginData; }

private:
    MockChannelsRepositoryData      _repoData;
    MockRepliesRepositoryData       _repliesData;
    MockDirectMessageRepositoryData _directMessgData;
    MockMessagesRepositoryData      _messgData;
    MockRegisterRepositoryData      _regData;
    MockLoginRepositoryData         _loginData;
};
}  /// namespace MockObject
