#include <catch2/catch.hpp>

#include "Base.Static/FileLogger.hpp"

using namespace Logger;

std::string getFldName()
{
    std::filesystem::path path = "Log";
    std::filesystem::create_directory(path);
    return path.string();
}

std::string getFileName(std::string Date)
{
    std::string fileName = "Log-" + Date + ".txt";
    return fileName;
}

bool logFileExists(std::string Date)
{
    std::string           fileName = getFileName(Date);
    std::filesystem::path path = "Log" +  std::string{"\\"} + fileName;
    
    return std::filesystem::exists(path);
}

std::string getCurrentDate()
{
    using std::chrono::system_clock;

    system_clock::time_point tp = system_clock::now();

    time_t raw_time = system_clock::to_time_t(tp);

    std::tm    tt       = Utility::safe_localtime(raw_time);
    struct tm* timeinfo = &tt;

    char buf[24] = {0};

    strftime(buf, 24, "%d.%m.%Y", timeinfo);

    return std::string(buf);
}

void check()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void cleanUp()
{
    std::filesystem::remove_all(getFldName());
}

bool getCurrentFilesInFile()
{
    size_t count = 0;
    for (auto p : std::filesystem::directory_iterator(getFldName()))
    {
        count++;
    }

    if (count == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool getCurrentName(std::string LogLvl)
{
    std::ifstream ifs("Log" + std::string{"\\"} + getFileName(getCurrentDate()));

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
        FileLogger& log = FileLogger::getInstance();
        log.log("Test: creat new log", LogLevel::DEBUG);
        // time delay for file recording
        check();
    }

    SECTION("Record request")
    {
        REQUIRE(getCurrentName("DEBUG") == true);
    }
}

TEST_CASE("Log in debug creates log file")
{
    SECTION("Create some log")
    {
        {
            FileLogger& log = FileLogger::getInstance();
            log.log("Test: creat new log", LogLevel::DEBUG);
            // time delay for file recording
            check();
        }
    }

    SECTION("Check created log file")
    {
        REQUIRE(logFileExists(getCurrentDate()) == true);
    }

    SECTION("Clear folder")
    {
        cleanUp();
    }
}

TEST_CASE("Checking the number of log files in a directory")
{
    SECTION("Create ten Test.txt files")
    {
        size_t counter = 1;
        std::fstream fileName;

        while (counter < 8)
        {
            fileName.open(getFldName() + std::string{"\\"} + "Test" + std::to_string(counter) + ".txt", std::ios::app);
            fileName.close();
            ++counter;
        }
        check();
    }

    SECTION("Add another log file")
    {
        FileLogger& log = FileLogger::getInstance();
        log.init("Test-", LogOutput::EVERYWHERE);
        log.log("Test: creat new log", LogLevel::DEBUG);
        check();
    }

    SECTION("Checking the number of files")
    {
        REQUIRE(getCurrentFilesInFile() == true);
    }

    SECTION("Clear folder")
    {
        cleanUp();
    }
}

TEST_CASE("Change the valid log levels")
{
    FileLogger& log = FileLogger::getInstance();

    REQUIRE(log.stringifyLogLvl(LogLevel::ERROR) == "ERROR");
    REQUIRE(log.stringifyLogLvl(LogLevel::WARNING) == "WARNING");
    REQUIRE(log.stringifyLogLvl(LogLevel::INFO) == "INFO");
    REQUIRE(log.stringifyLogLvl(LogLevel::DEBUG) == "DEBUG");
}
