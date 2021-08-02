#pragma once
#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess.Postgre/DataBaseOptions.hpp>

/** @class TestUsersAmountFinder
 *   @brief This class need for DBOptons replacement. /
 *   @details Inheriting from UsersAmountFinder, lets use the same functions of /
 *   UsersAmountFinder, but with test-DB.
 */
class TestUsersAmountFinder : public UsersAmountFinder
{
public:
    TestUsersAmountFinder() 
        : UsersAmountFinder(new Table("users", DBOptions::test))
    { }
};
