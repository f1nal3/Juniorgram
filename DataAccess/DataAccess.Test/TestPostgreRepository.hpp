#pragma once
//#include <DataAccess.Postgre/PostgreRepository.hpp>

/** @class TestUsersAmountFinder /
 *   @brief This class need for DBOptons replacement.
 *   @details Inheriting from PostgreRepository, lets use the same functions of /
 *   PostgreRepository, but with test DB.
 */
//class TestPostgreRepository : public DataAccess::PostgreRepository
//{
//public:
//    TestPostgreRepository() : PostgreRepository(new DataAccess::PostgreTable("", DBOptions::test)) {}
//    
//    /** @brief Delete all test-users from DB. /
//     *   @details We must call this method before every /
//     *   test_case in RepositoryFunctionsTest.cpp.
//     */
//    void deleteUsers(const std::unordered_map<std::string, Network::RegistrationInfo>& usersRegInfo) const
//    {
//        pTable->changeTable("users");
//
//        for (auto&& user : usersRegInfo)
//        {
//            pTable->Delete()->Where("email='" + user.second.email + "' or login='" + user.second.login + "'")->execute();
//        }
//    }
//};
