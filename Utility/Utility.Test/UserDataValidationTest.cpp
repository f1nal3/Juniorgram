#include <catch2/catch.hpp>

#include "Utility/UserDataValidation.hpp"

TEST_CASE("Login validation test")
{
    SECTION("Input valid login")
    {
        std::string rightLogin("some_nick34");
        REQUIRE(UserDataValidation::isLoginValid(rightLogin) == true);

        std::string upcaseLetterLogin("ABCDXF");
        REQUIRE(UserDataValidation::isLoginValid(upcaseLetterLogin) == true);

        std::string mixedLettersLogin("abcdXF");
        REQUIRE(UserDataValidation::isLoginValid(mixedLettersLogin) == true);
    }

    SECTION("Input invalid login")
    {
        std::string firstNumberLogin("5fgbo");
        REQUIRE(UserDataValidation::isLoginValid(firstNumberLogin) == false);

        std::string shortLogin("keke");
        REQUIRE(UserDataValidation::isLoginValid(shortLogin) == false);

        std::string emptyLogin("");
        REQUIRE(UserDataValidation::isLoginValid(emptyLogin) == false);

        std::string specSymbolsLogin("#$%^&*+-!");
        REQUIRE(UserDataValidation::isLoginValid(specSymbolsLogin) == false);
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(UserDataValidation::isLoginValid("test"));

        REQUIRE_NOTHROW(UserDataValidation::isLoginValid("Test123"));

        REQUIRE_NOTHROW(UserDataValidation::isLoginValid(""));
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

        std::string emptyEmail("");
        REQUIRE(UserDataValidation::isEmailValid(emptyEmail) == false);

        std::string specSymbolsEmail("#$%^&*+-!@!#$&^.#^$");
        REQUIRE(UserDataValidation::isEmailValid(specSymbolsEmail) == false);
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(UserDataValidation::isEmailValid("Anton3_antonov4@epam.com"));

        REQUIRE_NOTHROW(UserDataValidation::isEmailValid("65sfksdfsdlf@epam.com"));

        REQUIRE_NOTHROW(UserDataValidation::isEmailValid(""));
    }
}

TEST_CASE("Password validation test")
{
    SECTION("Input valid password")
    {
        std::string rightPassword("dgdfgAFS5623-sgsd");
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

        std::string emptyPass("");
        REQUIRE(UserDataValidation::isPasswordValid(emptyPass) == false);

        std::string specSymbolsPass("#$%^&*+-!#$&^#^$");
        REQUIRE(UserDataValidation::isPasswordValid(specSymbolsPass) == false);
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(UserDataValidation::isPasswordValid("dgdfgAFS5623-sgsd"));

        REQUIRE_NOTHROW(UserDataValidation::isPasswordValid("AAAfgdfABBB"));

        REQUIRE_NOTHROW(UserDataValidation::isPasswordValid(""));
    }
}
