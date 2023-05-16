#pragma once 

namespace MockObject
{
enum class TableChannelRepository
{
    UserID = 1,
    ChanneName,
    ChannelID
};

enum class TableRepliesRepository
{
    ChannelID = 1,
    Message,
    MessageID,
    UserLogin
};

enum class TableMessagesRepository
{
    UserID = 1,
    ChannelID,
    Message,
    MessageID,
    Reactions,
    RecipientID,
    UserLogin
};

enum class TableRegistrationRepository
{
    UserLogin = 1,
    Email,
    Password
};

enum class TableLoginRepository
{
    UserLogin = 1,
    PasswordHash
};
}  /// namespace MockObject
