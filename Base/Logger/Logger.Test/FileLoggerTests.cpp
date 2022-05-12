#include <catch2/catch.hpp>

#include "Logger.Static/FileLogger.hpp"
#include "Utility/UtilityTime.hpp"
#include <Utility/Utility.hpp>

using namespace Base::Logger;
using UtilityTime::safe_localtime;

static std::string __getFileName(const std::string& Date)
{
    std::string fileName = "Log-" + Date + ".txt";
    return fileName;
}

static bool logFileExists(const std::string& Date)
{
    std::filesystem::path path = "Log" + std::string{"\\"} + __getFileName(Date);
    return std::filesystem::exists(path);
}

static void pauseForRecording() { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }

static void cleanUp() { std::filesystem::remove_all(Utility::getFldName()); }

static bool checkNumberCurrentFiles()
{
    std::size_t count              = 0;
    std::size_t requiredCountFiles = 7;
    std::string path               = Utility::getFldName();
    for (auto p : std::filesystem::directory_iterator(path))
    {
        count++;
    }
    return count == requiredCountFiles;
}

static bool getCurrentName(std::string LogLvl)
{
    std::ifstream ifs("Log" + std::string{"\\"} + __getFileName(UtilityTime::getStringifiedCurrentDate()));

    while (std::getline(ifs, LogLvl))
    {
        if (LogLvl.find(LogLvl) != std::string::npos)
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
        FileLogger& loger = FileLogger::getInstance();
        loger.log("Test: creat new log", LogLevel::DEBUG);
        pauseForRecording();
    }

    SECTION("Record request") { REQUIRE(getCurrentName("DEBUG") == true); }
}

TEST_CASE("Log in debug creates log file")
{
    SECTION("Create some log")
    {
        {
            FileLogger& loger = FileLogger::getInstance();
            loger.log("Test: creat new log", LogLevel::DEBUG);
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
            std::filesystem::path path{Utility::getFldName()};
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
        log.log("Test: creat new log", LogLevel::DEBUG);
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
