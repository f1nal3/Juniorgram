#pragma once
#include "PostgreModel.hpp"
#include <FileLogger.hpp>

namespace DataAccess
{
using Base::Logger::FileLogger;

enum class RegInfo : uint16_t
{
    EMAIL = 0,
    LOGIN,
    PASSHASH
};

class UserRegistration : public PostgreModel<RegInfo>
{
public:
    UserRegistration(const std::string& modelName = "users", 
                        Models::FieldNames names = {"email,login,password_hash"},
                        size_t amountFields = 3):
                        PostgreModel<RegInfo>(modelName, names, amountFields)
    {
        this->init();
    }

    RegInfo getNumEnum(size_t num) const override
    {
        switch (num)
        {
            case 0:
                return RegInfo::EMAIL;
            case 1:
                return RegInfo::LOGIN;
            case 2: 
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
    User(const std::string& modelName = "users",
        Models::FieldNames names = { "id", "email", "login", "password_hash" },
        size_t amountFields = 4) :
        PostgreModel(modelName, names, amountFields)
    {
        this->init();
    }

    UserInfo getNumEnum(size_t num)const override
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
}
