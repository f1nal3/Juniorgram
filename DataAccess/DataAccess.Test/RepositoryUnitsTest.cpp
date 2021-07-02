#include <DataAccess.Static/PostgreTable.hpp>
#include <DataAccess.Static/RepositoryUnits.hpp>
#include <Utility.Static/Cryptography.hpp>
#include <catch2/catch.hpp>
#include <iostream>

static DataAccess::PostgreTable usersTable("users");

static void deleteUser(const Network::RegistrationInfo& ri)
{
    usersTable.Delete()->Where("email='" + ri.email + "'")->execute();
}

static std::uint16_t getUsersAmount(const Network::RegistrationInfo& ri)
{
    auto recordsRowAmount = usersTable.Select()
                           ->columns({"COUNT(*)"})
                           ->Where("email='" + ri.email + "', login='" + ri.login + "', '" + ri.passwordHash + "'")
                           ->execute();

    return recordsRowAmount.value()[0][0].as<std::uint16_t>();
};

TEST_CASE("Registration user")
{
    SECTION("RegistrationCode: SUCCESS")
    {
        const std::string EMAIL         = "unichtojator3000@gmail.com";
        const std::string LOGIN         = "kastrul9";
        const std::string PASSWORD      = "pfwDB-rd9";
        const std::string PASSWORD_HASH = Hashing::SHA_256(PASSWORD, LOGIN);

        const Network::RegistrationInfo ri(EMAIL, LOGIN, PASSWORD_HASH);
        auto registrationState = RegistrationUnit::instance().registerUser(ri);

        REQUIRE(getUsersAmount(ri) > 0);
        deleteUser(ri);
        
        REQUIRE(registrationState == Utility::RegistrationCodes::SUCCESS);
    }

    SECTION("RegistrationCode: EMAIL_ALREADY_EXISTS")
    {
        const std::string EMAIL = "andropovka64@mail.ru";
        const Network::RegistrationInfo firstUserData(EMAIL, "jigustator2",
                                                      Hashing::SHA_256("barBaRisKa68", "jigustator2"));

        const Network::RegistrationInfo secondUserData(EMAIL, "barbariska007",
                                                      Hashing::SHA_256("2002RDcool-man!", "barbariska007"));
        // 1
        RegistrationUnit::instance().registerUser(firstUserData);
        // 2
        auto registrationState = RegistrationUnit::instance().registerUser(secondUserData);
        deleteUser(firstUserData);

        if (registrationState != Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS)
        {
            deleteUser(secondUserData);
        }
        REQUIRE(registrationState == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS);
    }

    SECTION("RegistrationCode: LOGIN_ALREADY_EXISTS")
    {
        const std::string LOGIN         = "kastrul9";
        const Network::RegistrationInfo firstUserData("unichtojator3000@gmail.com", LOGIN,
                                                      Hashing::SHA_256("barBaRisKa68", LOGIN));

        const Network::RegistrationInfo secondUserData("andropovka34@gmail.com", LOGIN,
                                                      Hashing::SHA_256("2002RDcool-man!", LOGIN));
        // 1
        RegistrationUnit::instance().registerUser(firstUserData);
        // 2
        auto registrationState = RegistrationUnit::instance().registerUser(secondUserData);
        deleteUser(firstUserData);

        if (registrationState != Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS)
        {
            deleteUser(secondUserData);
        }
        REQUIRE(registrationState == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS);
    }
}
