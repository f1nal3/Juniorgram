#include <catch2/catch.hpp>

#include "Utility/Exception.hpp"

using Catch::Contains;

[[noreturn]] static void JuniorgramExceptionThrowerMock()
{
    throw Utility::JuniorgramException("JuniorgramException throw", __FILE__, __LINE__);
}

void JuniorgramExceptionNoExceptionWithConstCharConstructorMock()
{
    Utility::JuniorgramException("JuniorgramException no throw", __FILE__, __LINE__);
}

void JuniorgramExceptionNoExceptionWithStringConstructorMock()
{
    Utility::JuniorgramException(std::string("JuniorgramException no throw"), __FILE__, __LINE__);
}

void JuniorgramExceptionNoExceptionWithEmptyStringConstructorMock()
{
    Utility::JuniorgramException("", "", 0);
}

[[noreturn]] static void ExceptionThrowerMock()
{
    throw Utility::NotImplementedException("Not implemented", __FILE__, __LINE__);
}

void NoExceptionMock()
{
    Utility::NotImplementedException("Implemented", __FILE__, __LINE__);
}

[[noreturn]] static void OperationDBExceptionMock()
{
    throw Utility::OperationDBException("DB error operation", __FILE__, __LINE__);
}

void OperationDBNoExceptionMock()
{
    Utility::OperationDBException("DB success operation", __FILE__, __LINE__);
}

[[noreturn]] static void LoggingExceptionMock()
{
    throw Utility::LoggingException("Logging and throw", __FILE__, __LINE__);
}

void LoggingNoExceptionMock()
{
    Utility::LoggingException("Logging without throw", __FILE__, __LINE__);
}

TEST_CASE("Test JuniorgramException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(JuniorgramExceptionThrowerMock(), Contains("JuniorgramException throw") && Contains("ExceptionTest.cpp"));
    }

    SECTION("No exception expected, constructor runs with \"const char*\" parameters.")
    {
        REQUIRE_NOTHROW(JuniorgramExceptionNoExceptionWithConstCharConstructorMock());
    }

    SECTION("No exception expected, constructor runs with \"std::string\" parameters.")
    {
        REQUIRE_NOTHROW(JuniorgramExceptionNoExceptionWithStringConstructorMock());
    }

    SECTION("No exception expected, constructor string parameters are empty.")
    {
        REQUIRE_NOTHROW(JuniorgramExceptionNoExceptionWithStringConstructorMock());
    }
}

TEST_CASE("Test NotImplementedException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(ExceptionThrowerMock(), Contains("Not implemented") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(NoExceptionMock());
    }
}

TEST_CASE("Test OperationDBException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(OperationDBExceptionMock(), Contains("DB error operation") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(OperationDBNoExceptionMock());
    }
}

TEST_CASE("Test LoggingException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(LoggingExceptionMock(), Contains("Logging and throw") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(LoggingNoExceptionMock());
    }
}

TEST_CASE("Test formatExceptionMessage")
{
    std::string message  = "Test exception";
    std::string filename = "ExceptionTest.txt";
    uint16_t    line     = 3;

    REQUIRE_THAT(Utility::formatExceptionMessage(message, filename, line),
                 Contains(message.data()) && Contains(filename.data()) && Contains(std::to_string(line).data()));

    REQUIRE_NOTHROW(Utility::formatExceptionMessage("", "", 0));

    REQUIRE_NOTHROW(Utility::formatExceptionMessage(message, filename, line));
}
