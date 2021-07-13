#pragma once
#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <DataAccess.Static/DataBaseOptions.hpp>
#include <Network/Primitives.hpp>
#include <string>
#include <unordered_map>

class TestRegistrationUnit : public RegistrationUnit
{
public:
    TestRegistrationUnit()
        : RegistrationUnit(new Table("users", DBOptions::test))
    { }

    void rollback(const std::unordered_map<std::string, Network::RegistrationInfo>& usersData)
    {
        for (auto&& user : usersData)
        {
            pTable->Delete()
                ->Where("email='" + user.second.email + 
                        "' or login='" + user.second.login + 
                        "'")
                ->execute();
        }
    }
};
