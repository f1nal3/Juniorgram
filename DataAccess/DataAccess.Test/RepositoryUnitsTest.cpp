#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <catch2/catch.hpp>
#include <iostream>
#include <unordered_map>
#include <DataAccess.Static/UsersAmountFunctions.hpp>

// WARNING!
// Be carefull. Maybe user with some data already exists in DB before tests running.
// When you create new test, you must delete all test-users from DB calling deleteUsersFromDB function! 

const std::unordered_map<std::string, Network::RegistrationInfo> USERS_DATA{
    std::pair("user_1", Network::RegistrationInfo
                        {
                            "funichtojator3000@gmail.com", 
                            "akastrul9",
                            "ED5B722A5298A75A5825825D5C8DBF69BEA1A5ABE4BAA1C3295ACF83F02B9BAE"
                        }),

    std::pair("user_2", Network::RegistrationInfo
                        {
                            "funichtojator3000@gmail.com",
                            "akastrul9",
                            "ED5B722A5298A75A5825825D5C8DBF69BEA1A5ABE4BAA1C3295ACF83F02B9BAE"
                        }),

    std::pair("user_3", Network::RegistrationInfo
                        {
                            "funichtojator3000@gmail.com",
                            "baskov007",
                            "fd38ddb3fa9d7a2f8cf83cbf5d2af1a6600c264ab4dead51df94f5f0b7393c38"
                        }),

    std::pair("user_4", Network::RegistrationInfo
                        {
                            "andropovka64@mail.ru",
                            "akastrul9",
                            "c5ecfdec38b527af45540af95f0543587917fb457ccc56989ba55a601fca3fd5"
                        }),

};

void deleteUsersFromDB()
{
    DataAccess::PostgreTable tableOfUsers("users");
    
    for (auto&& user : USERS_DATA)
    {
        tableOfUsers.Delete()
            ->Where("email='" + user.second.email + "' or login='" + user.second.login + "'")
            ->execute();
    }
}

TEST_CASE("Registration user")
{
    RegistrationUnit registrator;

    SECTION("RegistrationCode: SUCCESS")
    {
        deleteUsersFromDB();

        const auto USER_1 = USERS_DATA.at("user_1");
        const auto REGISTRATION_CODE = registrator.registerUser(USER_1);
                
        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::SUCCESS);
        REQUIRE(findUsersAmountWithAllSameData(USER_1) == 1);
    }

    SECTION("RegistrationCode: EMAIL_ALREADY_EXISTS")
    {
        deleteUsersFromDB();
        
        const auto USER_2 = USERS_DATA.at("user_2");
        registrator.registerUser(USER_2);
        
        const auto USER_3            = USERS_DATA.at("user_3");
        const auto REGISTRATION_CODE = registrator.registerUser(USER_3);

        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS);
        REQUIRE(findUsersAmountWithSameEmail(USER_2.email) == 1);
    }

    SECTION("RegistrationCode: LOGIN_ALREADY_EXISTS")
    {
        deleteUsersFromDB();
     
        const auto USER_1 = USERS_DATA.at("user_1");
        registrator.registerUser(USER_1);

        const auto USER_4            = USERS_DATA.at("user_4");
        const auto REGISTRATION_CODE = registrator.registerUser(USER_4);

        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS);
        REQUIRE(findUsersAmountWithSameLogin(USER_1.login) == 1);
    }
}
