#pragma once
#include "PostgreModel.hpp"
#include <FileLogger.hpp>
#include <map>
#include <utility>

namespace Models
{
using Base::Logger::FileLogger;


enum class UserInfo : uint16_t
{
    ID = 0,
    EMAIL,
    LOGIN,
    PASSHASH
};

template<template<typename ...TArgs> class RepositoryType, class TEnum = UserInfo, typename... Args>
class User : public RepositoryType<TEnum, Args...>
{
public:
    User(const std::string& modelName = "users", const Models::FieldNames& names = { "id", "email", "login", "password_hash" })
        : RepositoryType<TEnum>(modelName, names.size())
    {
        this->init(names);
    }

    User(const std::vector<std::pair<TEnum, std::string>>& insertData) : User()
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          this->_data[pair.first] = pair.second;
                      });
    }

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
                FileLogger::getInstance().error("Undefined integer income");
                return {};
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

template<template<typename ...TArgs> class RepositoryType, class TEnum = ChannelData, typename... Args>
class Channel : public RepositoryType<TEnum, Args...>
{
public:
    Channel(const std::string& modelName = "channels", const Models::FieldNames& names = { "id", "channel_name", "creator_id", "user_limit" })
        : RepositoryType<TEnum>(modelName, names.size())
    {
        this->init(names);
    }

    /*
    * @details Possibly will be moved into sepate method in UnifyedModel or smth like that because it consists only of templates, so it may be used by any model
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
            default:
            {
                FileLogger::getInstance().error("Undefined integer income");
                return {};
            }
        }
    }
};
}  // namespace Models
