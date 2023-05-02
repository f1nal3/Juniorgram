#pragma once
#include "PostgreModel.hpp"
#include <FileLogger.hpp>
#include <map>
#include <utility>

namespace DataAccess
{
using Base::Logger::FileLogger;

enum class RegInfo : uint16_t
{
    ID= 0,
    EMAIL,
    LOGIN,
    PASSHASH
};

class UserRegistration : public PostgreModel<RegInfo>
{
public:
    UserRegistration(const std::string& modelName = "users", Models::FieldNames names = { "id","email","login","password_hash" })
        : PostgreModel(modelName, names.size())
    {
        this->init(names);
    }

    RegInfo getNumEnum(size_t num) const override
    {
        switch (num)
        {
            case 0:
                return RegInfo::ID;
            case 1:
                return RegInfo::EMAIL;
            case 2:
                return RegInfo::LOGIN;
            case 3:
                return RegInfo::PASSHASH;
            default:
            {
                FileLogger::getInstance().error("Undefined integer income");
                return {};
            }
        }
    }
};

enum class UserInfo : uint16_t
{
    ID = 0,
    EMAIL,
    LOGIN,
    PASSHASH
};

class User : public PostgreModel<UserInfo>
{
public:
    User(const std::string& modelName = "users", const Models::FieldNames& names = { "id", "email", "login", "password_hash" })
        : PostgreModel(modelName, names.size())
    {
        this->init(names);
    }

    explicit User(std::vector<std::string> fillFields) : User()
    {
        auto fieldIter = this->_fieldData.begin();
        for (auto fillIter = fillFields.begin(); fillIter != fillFields.end(); ++fieldIter, ++fillIter)
            this->_data[fieldIter->first] = *fillIter;
    }

    UserInfo getNumEnum(size_t num) const override
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

enum class ChannelInfo : uint16_t
{
    CHANNEL_ID = 0,
    CHANNEL_NAME,
    CREATOR_ID,
    CHANNEL_USER_LIMIT
};

class Channel : public PostgreModel<ChannelInfo>
{
public:
    Channel(const std::string& modelName = "channels", const Models::FieldNames& names = { "id", "channel_name", "creator_id", "user_limit" })
        : PostgreModel(modelName, names.size())
    {
        this->init(names);
    }

    /*
    * @details Possibly will be moved into sepate method in UnifyedModel or smth like that because it consists only of templates, so it may be used by any model
    */
    explicit Channel(const std::initializer_list<std::pair<ChannelInfo, std::string>>& insertData) : Channel()
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          _data[pair.first] = pair.second;
                      });
    }

protected:
    ChannelInfo getNumEnum(size_t num) const override
    {
        switch (num)
        {
            case 0: 
                return ChannelInfo::CHANNEL_ID;
            case 1:
                return ChannelInfo::CHANNEL_NAME;
            case 2:
                return ChannelInfo::CREATOR_ID;
            case 3:
                return ChannelInfo::CHANNEL_USER_LIMIT;
            default:
            {
                FileLogger::getInstance().error("Undefined integer income");
                return {};
            }
        }
    }
};
}  // namespace DataAccess
