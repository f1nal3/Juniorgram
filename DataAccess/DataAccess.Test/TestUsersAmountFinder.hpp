#pragma once
#include <DataAccess.Static/UsersAmountFinder.hpp>

class TestUsersAmountFinder : public UsersAmountFinder
{
public:
    TestUsersAmountFinder()
        : UsersAmountFinder(new Table("users",
              "hostaddr=104.40.239.183 port=5432 dbname=test user=userfortests password=123"))
    {}
};
