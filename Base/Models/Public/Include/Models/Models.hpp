#pragma once

#include "UnifiedModel.hpp"

#include <map>
#include <utility>

#include <FileLogger.hpp>

namespace Models::V2
{
enum class UserInfo : uint16_t
{
    ID = 0,
    EMAIL,
    LOGIN,
    PASSHASH
};

template<typename TEnum = UserInfo>
class User : public UnifiedModel<TEnum>
{
public:
    User(std::string_view modelName = "users", const Models::V2::FieldNames& names = { "id", "email", "login", "password_hash" })
        : UnifiedModel<TEnum>(modelName, names.size())
    {     
        this->init(names);
    }

    explicit User(const std::vector<std::pair<TEnum, std::string>>& insertData) : User()
    {
        this->fillStartFields(insertData);
    }

protected:
    UserInfo getNumEnum(size_t num) const final
    {
        switch (num)
        {
            case 0:
                return UserInfo::ID;
            case 1:
                return UserInfo::EMAIL;
            case 2:
                return UserInfo::LOGIN;
            case 3:
                return UserInfo::PASSHASH;
            default:
            {
                Base::Logger::FileLogger::getInstance().
                    error("Overrided method does not deal with this amount of fields given in constructor");
                throw std::invalid_argument("Overriding default params in constructor was unsuccessful");
            }
                
        }
    }
};

enum class ChannelData : uint16_t
{
    CHANNEL_ID = 0,
    CHANNEL_NAME,
    CREATOR_ID,
    CHANNEL_USER_LIMIT
};

template<typename TEnum = ChannelData>
class Channel: public UnifiedModel<TEnum>
{
public:
    Channel(std::string_view modelName = "channels", const Models::V2::FieldNames& names = { "id", "channel_name", "creator_id", "user_limit" })
        :UnifiedModel<TEnum>(modelName, names.size())
    {
        this->init(names);
    }

    explicit Channel(const std::vector<std::pair<TEnum, std::string>>& insertData) : Channel()
    {
        this->fillStartFields(insertData);
    }

protected:
    ChannelData getNumEnum(size_t num) const final
    {
        switch (num)
        {
            case 0: 
                return ChannelData::CHANNEL_ID;
            case 1:
                return ChannelData::CHANNEL_NAME;
            case 2:
                return ChannelData::CREATOR_ID;
            case 3:
                return ChannelData::CHANNEL_USER_LIMIT;
            default:          
            {
                Base::Logger::FileLogger::getInstance().
                    error("Overrided method does not deal with this amount of fields given in constructor");
                throw std::invalid_argument("Overriding default params in constructor was unsuccessful");
            }
        }
    }
};

enum class UserChannelsData
{
    USER_ID,
    CHANNEL_ID
};

template<typename TEnum = UserChannelsData>
class UserChannels: public UnifiedModel<TEnum>
{
public:
    UserChannels(std::string_view modelName = "user_channels", const Models::V2::FieldNames& names = { "user_id", "channel_id" })
        :UnifiedModel<TEnum>(modelName, names.size())
    {
        this->init(names);
    }

    explicit UserChannels(const std::vector<std::pair<TEnum, std::string>>& insertData) : UserChannels()
    {
        this->fillStartFields(insertData);
    }

protected:
    UserChannelsData getNumEnum(size_t num) const final
    {
        switch (num)
        {
            case 0: 
                return UserChannelsData::USER_ID;
            case 1: 
                return UserChannelsData::CHANNEL_ID;
            default:
            {
                Base::Logger::FileLogger::getInstance().
                    error("Overrided method does not deal with this amount of fields given in constructor");
                throw std::invalid_argument("Overriding default params in constructor was unsuccessful");
            }
        }
    }
};
}  // namespace Models
