#pragma once 
#include <string>
#include <vector>

#include "Cryptography.hpp"
#include <Models/Primitives.hpp>

namespace MockObject
{
using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelInfo             = Models::ChannelInfo;

/**
* @brief MockRepositoryStorage class.
* @details Designed to initialize tables (data strings) that /
*          emulate database storage for a specific repository.
*/
struct MockRepositoryStorage
{
    MockRepositoryStorage()  = default;
    ~MockRepositoryStorage() = default;

    ChannelInfo&                     getChannelsRepoData() { return _channelData; }
    ReplyInfo&                       getRepliesRepoData() { return _repliesData; }
    MessageInfo&                     getMessageRepoData() { return _messgData; }
    RegistrationInfo&                getRegisterRepoData() { return _regData; }
    LoginInfo&                       getLoginRepoData() { return _loginData; }

private:
    ChannelInfo                     _channelData;
    ReplyInfo                       _repliesData;
    MessageInfo                     _messgData;
    RegistrationInfo                _regData;
    LoginInfo                       _loginData;
};
}  /// namespace MockObject
