#pragma once

#include "Cryptography.hpp"

namespace TestMessagesOptions
{
const std::string testLogin{"memorisecodead"};
const std::string testPSWD{"12juniorgramMargroinuj"};
const std::string testPWDHash{Base::Hashing::SHA_256(testPSWD, testLogin)};
constexpr std::string_view testEmail{"demonstakingoverme@epam.co"};
constexpr std::string_view testChannelName{"testServer"};
constexpr std::string_view testMessage{"Hello, juniorgram!!"};
constexpr uint16_t    testReactionID{1};
constexpr uint16_t    testMsgID{2};
constexpr uint16_t    testRecipientID{1};
constexpr uint16_t    testUserID{1};
constexpr uint16_t    testChannelID{1};
constexpr uint16_t    testCreatorID{1};
constexpr uint16_t    testSenderID{1};
constexpr uint16_t    testMsgIDOwner{1};
}  // namespace TestMessagesOptions
