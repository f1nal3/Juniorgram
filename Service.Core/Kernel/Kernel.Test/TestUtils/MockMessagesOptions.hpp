#pragma once

#include "Cryptography.hpp"
/**
* @brief namespace with data for testing. 
* @details Used in TestUtility.hpp to emulate user data. 
*/
namespace TestUtility
{
const std::string testLogin{"tester"};
const std::string testPSWD{"password"};
const std::string testPWDHash{Base::Hashing::SHA_256(testPSWD, testLogin)};

constexpr std::string_view testEmail{"test@epam.com"};
constexpr std::string_view testChannelName{"testKernel"};
constexpr std::string_view testMessage{"Hello, juniorgram!"};

constexpr uint16_t    testReactionID{1};
constexpr uint16_t    testMsgID{2};
constexpr uint16_t    testRecipientID{1};
constexpr uint16_t    testUserID{1};
constexpr uint16_t    testChannelID{1};
constexpr uint16_t    testCreatorID{1};
constexpr uint16_t    testSenderID{1};
constexpr uint16_t    testMsgIDOwner{1};
}  /// namespace TestUtility
