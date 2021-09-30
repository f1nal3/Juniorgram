#include <catch.hpp>

#include "Logger.Static/FileLogger.hpp"

using namespace Base::Logger;

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

void pauseForRecording()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void cleanUp()
{
    std::filesystem::remove_all(getFldName());
}

bool checkNumberCurrentFiles()
{
    std::size_t count = 0;
    std::size_t requirerequiredNumberFiles = 7;
    for (auto p : std::filesystem::directory_iterator(getFldName()))
    {
        count++;
    }
    return count == requirerequiredNumberFiles;
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
        pauseForRecording();
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
            pauseForRecording();
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
        std::size_t           counter = 1;
        while (counter < 8)
        {
            std::filesystem::path path{getFldName()};
            path /= getFileName(std::to_string(counter));
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

    SECTION("Checking the number of files")
    {
        REQUIRE(checkNumberCurrentFiles() == true);
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
