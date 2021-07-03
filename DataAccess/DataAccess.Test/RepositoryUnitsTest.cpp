#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <Utility.Static/Cryptography.hpp>
#include <catch2/catch.hpp>
#include <iostream>
#include <unordered_map>
// WARNING!
// Be carefull. Maybe user with this data already exists in DB before tests running!
// When you create new test, you must delete all test-users from DB calling deleteUsersFromDB function! 

std::uint16_t findUsersAmountWithSameTableAttribute(const std::string& condition);
std::uint16_t findUsersAmountWithSameLogin(const std::string& login);
std::uint16_t findUsersAmountWithSameEmail(const std::string& email);
std::uint16_t findUsersAmountWithAllSameData(const Network::RegistrationInfo& ri);

static const std::unordered_map<std::string, Network::RegistrationInfo> USERS_DATA{
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
                            "andropovka64@mail.ru",
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

static void deleteUser(const Network::RegistrationInfo& ri)
{
    static DataAccess::PostgreTable tableOfUsers("users");
    tableOfUsers.Delete()->Where("email='" + ri.email + "' or login='" + ri.login + "'")->execute();
}

static void deleteUsersFromDB()
{
    for (auto&& user : USERS_DATA)
    {
        deleteUser(user.second);
    }
}

TEST_CASE("Registration user")
{
    SECTION("RegistrationCode: SUCCESS")
    {
        //deleteUsersFromDB();

        const auto USER_1 = USERS_DATA.at("user_1");
        const auto REGISTRATION_CODE = RegistrationUnit::instance().registerUser(USER_1);
                
        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::SUCCESS);
        REQUIRE(findUsersAmountWithAllSameData(USER_1) == 1);
    }

    SECTION("RegistrationCode: EMAIL_ALREADY_EXISTS")
    {
        deleteUsersFromDB();
        
        const auto USER_2 = USERS_DATA.at("user_2");
        RegistrationUnit::instance().registerUser(USER_2);
        
        const auto USER_3      = USERS_DATA.at("user_3");
        const auto REGISTRATION_CODE = RegistrationUnit::instance().registerUser(USER_3);

        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS);
        REQUIRE(findUsersAmountWithSameEmail(USER_3.email) == 0);
    }

    SECTION("RegistrationCode: LOGIN_ALREADY_EXISTS")
    {
        deleteUsersFromDB();
     
        const auto USER_1 = USERS_DATA.at("user_1");
        RegistrationUnit::instance().registerUser(USER_1);

        const auto USER_4            = USERS_DATA.at("user_4");
        const auto REGISTRATION_CODE = RegistrationUnit::instance().registerUser(USER_4);

        REQUIRE(REGISTRATION_CODE == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS);
        REQUIRE(findUsersAmountWithSameLogin(USER_4.email));
    }
}

std::uint16_t findUsersAmountWithSameTableAttribute(const std::string& condition)
{
    const auto RECORDS_AMOUNT = DataAccess::PostgreTable("users")
                                    .Select()
                                    ->columns({"COUNT(*)"})
                                    ->Where(condition)
                                    ->execute();

    return RECORDS_AMOUNT.value()[0][0].as<std::uint16_t>();
};

std::uint16_t findUsersAmountWithSameLogin(const std::string& login)
{
    return findUsersAmountWithSameTableAttribute("login='" + login + "'");
};

std::uint16_t findUsersAmountWithSameEmail(const std::string& email)
{
    return findUsersAmountWithSameTableAttribute("email='" + email + "'");
};

std::uint16_t findUsersAmountWithAllSameData(const Network::RegistrationInfo& ri)
{
    return findUsersAmountWithSameTableAttribute("email='" + ri.email + 
                                                 "' and login='" + ri.login +
                                                 "' and password_hash='" + ri.passwordHash + 
                                                 "'");
};
