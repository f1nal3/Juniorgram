#pragma once 

#include <string_view>
#include "Cryptography.hpp"

namespace TestUtility
{
constexpr std::string_view testProperties
{
	"hostaddr=127.0.0.1 port=5432 dbname=mockdb user=tester"
};

struct MockRepositoryData
{
    MockRepositoryData() = default;
    ~MockRepositoryData() = default;

    std::vector<std::string>& getStorageData() { return _testStorageData; }

private:
    std::vector<std::string> _testStorageData;

    std::string _testLogin;
    std::string _testPSWD;
    std::string _testPWDHash{Base::Hashing::SHA_256(_testPSWD, _testLogin)};

    std::string _testEmail;
    std::string _testChannelName;
    std::string _testMessage;

    std::string _testReactionID;
    std::string _testMsgID;
    std::string _testRecipientID;
    std::string _testUserID;
    std::string _testChannelID;
    std::string _testCreatorID;
    std::string _testSenderID;
    std::string _testMsgIDOwner;
};

struct MockRepositoryInstance : public MockRepositoryData
{
    explicit MockRepositoryInstance(const std::string_view& options)
        : _repoProperties(options) {}

private:
    std::string_view _repoProperties;
};
} // namespace TestDBOptions
