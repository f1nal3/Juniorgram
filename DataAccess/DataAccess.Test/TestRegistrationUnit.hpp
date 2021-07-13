#pragma once
#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <Network/Primitives.hpp>
#include <string>
#include <unordered_map>

class TestRegistrationUnit : public RegistrationUnit
{
public:
    TestRegistrationUnit()
        : RegistrationUnit(new Table("users",
              "hostaddr=104.40.239.183 port=5432 dbname=test user=userfortests password=123"))
    {}

    void rollback(const std::unordered_map
        <std::string, Network::RegistrationInfo>& usersData)
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
