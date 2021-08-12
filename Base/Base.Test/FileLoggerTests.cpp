#include <catch2/catch.hpp>

#include "Base.Static/FileLogger.hpp"

using namespace Logger;

TEST_CASE("stringify()")
{
    FileLogger& log = FileLogger::getInstance();

    REQUIRE(log.stringify(Logger::LogLevel::ERROR) == "ERROR");
    REQUIRE(log.stringify(Logger::LogLevel::WARNING) == "WARNING");
    REQUIRE(log.stringify(Logger::LogLevel::INFO) == "INFO");
    REQUIRE(log.stringify(Logger::LogLevel::DEBUG) == "DEBUG");
}
