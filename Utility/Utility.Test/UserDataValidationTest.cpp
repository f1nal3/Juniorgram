#include <catch2/catch.hpp>

#include "Utility/UserDataValidation.hpp"

TEST_CASE("Login validation test")
{
    SECTION("Input valid login")
    {
        std::string rightLogin("some_nick34");
        REQUIRE(UserDataValidation::isLoginValid(rightLogin) == true);
    }

    SECTION("Input invalid login")
    {
        std::string firstNumberLogin("5fgbo");
        REQUIRE(UserDataValidation::isLoginValid(firstNumberLogin) == false);

        std::string upcaseLetterLogin("abcdXF");
        REQUIRE(UserDataValidation::isLoginValid(upcaseLetterLogin) == false);

        std::string shortLogin("keke");
        REQUIRE(UserDataValidation::isLoginValid(shortLogin) == false);
    }
}

TEST_CASE("Email validation test")
{
    SECTION("Input valid email")
    {
        std::string rightEmail("Anton3_antonov4@epam.com");
        REQUIRE(UserDataValidation::isEmailValid(rightEmail) == true);
    }

    SECTION("Input invalid email")
    {
        std::string firstNumberEmail("65sfksdfsdlf@epam.com");
        REQUIRE(UserDataValidation::isEmailValid(firstNumberEmail) == false);

        std::string emailWithoutAt("65sfksdfsdlfepam.com");
        REQUIRE(UserDataValidation::isEmailValid(emailWithoutAt) == false);

        std::string noDotEmail("65sfksdfsdlf@epamcom");
        REQUIRE(UserDataValidation::isEmailValid(noDotEmail) == false);

        std::string longDomainEmail("65sfksdfsdlf@epam.comru");
        REQUIRE(UserDataValidation::isEmailValid(longDomainEmail) == false);

        std::string shortDomainEmail("65sfksdfsdlf@epam.c");
        REQUIRE(UserDataValidation::isEmailValid(shortDomainEmail) == false);
    }
}


TEST_CASE("Password validation test")
{
    SECTION("Input valid password")
    {
        std::string rightPassword("1996a*nton-Anton%ov_34");
        REQUIRE(UserDataValidation::isPasswordValid(rightPassword) == true);
    }

    SECTION("Input invalid password")
    {
        std::string noUpcaseLetterPass("fdgd56-fgd5g");
        REQUIRE(UserDataValidation::isPasswordValid(noUpcaseLetterPass) == false);

        std::string noLowcaseLetterPass("FGFF56-FSD665B");
        REQUIRE(UserDataValidation::isPasswordValid(noLowcaseLetterPass) == false);

        std::string noNumbersPass("AAAfgdfABBB");
        REQUIRE(UserDataValidation::isPasswordValid(noNumbersPass) == false);

        std::string shortPass("53AAbe");
        REQUIRE(UserDataValidation::isPasswordValid(shortPass) == false);
    }
}