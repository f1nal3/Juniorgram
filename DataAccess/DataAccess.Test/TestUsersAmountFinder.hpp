#pragma once
#include <DataAccess.Static/UsersAmountFinder.hpp>
#include <DataAccess.Static/DataBaseOptions.hpp>

class TestUsersAmountFinder : public UsersAmountFinder
{
public:
    TestUsersAmountFinder() 
        : UsersAmountFinder(new Table("users", DBOptions::test))
    { }
};
