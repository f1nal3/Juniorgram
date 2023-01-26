#include <catch2/catch.hpp>

#include "Logger.Static/FileLogger.hpp"
#include "Utility/UtilityTime.hpp"
#include <Utility/Utility.hpp>
#include <Utility/JGExceptions.hpp>

using namespace Base::Logger;
using UtilityTime::safe_localtime;

static std::string __getFileName(const std::string& Date)
{
    return "Log-" + Date;
}

static bool logFileExists(std::string_view Date)
{
    std::filesystem::path path = "Log" + std::string{"\\"} + __getFileName(Date.data());
    return std::filesystem::exists(path);
}

static void pauseForRecording(uint64_t ms = 1000) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

static void cleanUp() { std::filesystem::remove_all(Utility::getFldPath()); }

static bool checkNumberCurrentFiles()
{
    std::size_t count              = 0;
    std::size_t requiredCountFiles = 7;
    std::string path               = Utility::getFldPath();
    for ([[maybe_unused]]const auto& p : std::filesystem::directory_iterator(path))
    {
        count++;
    }
    return count == requiredCountFiles;
}

static bool getCurrentName(std::string_view LogLvl)
{
    auto path = Utility::getFldPath() + "\\" + __getFileName(UtilityTime::getStringifiedCurrentDate());
    std::ifstream ifs (path, std::ios::in);
    std::string buf;
    while (std::getline(ifs, buf))
    {
        if (buf.find(LogLvl) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

TEST_CASE("Checking the corresponding lvl log")
{
    SECTION("Create log file")
    {
        FileLogger& logger = FileLogger::getInstance();
        logger.log("Test: create new log", LogLevel::DEBUG);
        pauseForRecording();
    }

    SECTION("Record request") { REQUIRE(getCurrentName("DEBUG") == true); }
}

TEST_CASE("Log in debug creates log file")
{
    SECTION("Create some log")
    {
        {
            FileLogger& logger = FileLogger::getInstance();
            logger.log("Test: create new log", LogLevel::DEBUG);
            pauseForRecording();
        }
    }

    SECTION("Check created log file") { REQUIRE(logFileExists(UtilityTime::getStringifiedCurrentDate()) == true); }

    SECTION("Clear folder") { cleanUp(); }
}

TEST_CASE("Checking the number of log files in a directory")
{
    SECTION("Create ten Test.txt files")
    {
        std::size_t counter = 1;
        while (counter < 8)
        {
            std::filesystem::path path{Utility::getFldPath()};
            path /= __getFileName(std::to_string(counter));
            std::filesystem::create_directory(path.parent_path());
            std::ofstream ofs(path);
            ofs.close();
            ++counter;
        }
        pauseForRecording();
    }

    SECTION("Add another log file")
    {
        FileLogger& log = FileLogger::getInstance();
        log.init("Test-", LogOutput::EVERYWHERE);
        log.log("Test: create new log", LogLevel::DEBUG);
        pauseForRecording();
    }

    SECTION("Checking the number of files") { REQUIRE(checkNumberCurrentFiles() == true); }

    SECTION("Clear folder") { cleanUp(); }
}

TEST_CASE("Change the valid log levels")
{
    const FileLogger& log = FileLogger::getInstance();

    REQUIRE(log.stringifyLogLvl(LogLevel::ERR) == "ERROR");
    REQUIRE(log.stringifyLogLvl(LogLevel::WARNING) == "WARNING");
    REQUIRE(log.stringifyLogLvl(LogLevel::INFO) == "INFO");
    REQUIRE(log.stringifyLogLvl(LogLevel::DEBUG) == "DEBUG");
}

TEST_CASE("Special methods for improved logging")
{
    auto& testLogger = FileLogger::getInstance();

    REQUIRE_NOTHROW(testLogger.debug("Test debug message\n"));
    REQUIRE_NOTHROW(testLogger.error("Test error message\n"));
    REQUIRE_NOTHROW(testLogger.info("Test info message\n"));
    REQUIRE_NOTHROW(testLogger.warning("Test warning message\n"));
}

TEST_CASE("Logging and throw exception method")
{
    auto& testLogger         = FileLogger::getInstance();
    const char* filename     = "FileLoggerTest.cpp";
    std::unique_ptr<Utility::JuniorgramException> someException;

    someException = std::make_unique<Utility::JuniorgramException>("Test Utility::JuniorgramException", filename, __LINE__);
    
    REQUIRE_THROWS(testLogger.logAndThrow(filename, __LINE__, *someException));
    
    someException = std::make_unique<Utility::NotImplementedException>("Test Utility::NotImplementedException", filename, __LINE__);
    REQUIRE_THROWS(testLogger.logAndThrow(filename, __LINE__, *someException));
    
    someException = std::make_unique<Utility::OperationDBException>("Test Utility::OperationDBException", filename, __LINE__);
    REQUIRE_THROWS(testLogger.logAndThrow(filename, __LINE__, *someException));
 }
