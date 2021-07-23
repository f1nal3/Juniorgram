#pragma once
#include <DataAccess.Postgre/PostgreRepository.hpp>

class TestPostgreRepository : public DataAccess::PostgreRepository
{
public:
    TestPostgreRepository() : PostgreRepository(new DataAccess::PostgreTable("", DBOptions::test)) {}
    
    void deleteUsers(const std::unordered_map<std::string, Network::RegistrationInfo>& usersRegInfo) const
    {
        pTable->changeTable("users");

        for (auto&& user : usersRegInfo)
        {
            pTable->Delete()->Where("email='" + user.second.email + "' or login='" + user.second.login + "'")->execute();
        }
    }
};
