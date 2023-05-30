#pragma once

#include "UnifyedModel.hpp"

#include <map>
#include <utility>

namespace Models
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
    User(std::string_view modelName = "users", const Models::FieldNames& names = { "id", "email", "login", "password_hash" })
        : UnifiedModel<TEnum>(modelName, names.size())
    {     
        this->init(names);
    }

    explicit User(const std::vector<std::pair<TEnum, std::string>>& insertData) : User()
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          this->_data[pair.first] = pair.second;
                      });
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
class Channel : public UnifiedModel<ChannelData>
{
public:
    Channel(std::string_view modelName = "channels", const Models::FieldNames& names = { "id", "channel_name", "creator_id", "user_limit" })
        :UnifiedModel<ChannelData>(modelName, names.size())
    {
        this->init(names);
    }

    /*
    * @details Possibly will be moved into sepate method in UnifiedModel or smth like that because it consists only of templates, so it may be used by any model
    */
    explicit Channel(const std::vector<std::pair<TEnum, std::string>>& insertData) : Channel()
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          this->_data[pair.first] = pair.second;
                      });
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
        }
    }
};
}  // namespace Models
