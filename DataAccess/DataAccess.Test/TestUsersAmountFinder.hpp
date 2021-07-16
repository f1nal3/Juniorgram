#pragma once
#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess.Postgre/DataBaseOptions.hpp>

class TestUsersAmountFinder : public UsersAmountFinder
{
public:
    TestUsersAmountFinder() 
        : UsersAmountFinder(new Table("users", DBOptions::test))
    { }
};
