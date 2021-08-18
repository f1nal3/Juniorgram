#pragma once

#include <algorithm>
#include "Logger/ILogger.hpp"
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include "Utility/Utility.hpp"
#include <vector>

namespace Logger
{
class FileLogger : ILogger
{
public:
    using BlockWrapper = std::pair<std::string, std::string>;

    /**
     * @brief Gets an object of the current class
     * @return object Logger
     */
    static FileLogger& getInstance();

    /**
     * @brief Get required storage period for log files
     * @param basic file storage 7 days
     * @return storage period in days
     */
    unsigned getPeriodLife(unsigned periodLife) override;

    /**
     * @brief Initialization of the current class object
     * @param file name to write data
     * @param the place where the data will be displayed
     */
    void init(const std::string& filename, const LogOutput output) override;

    /**
     * @brief Opening the current class file
     */
    void open() override;

    /**
     * @brief Closing the current class file
     */
    void close() override;

    /**
     * @brief Log entry
     * @param required log message
     * @param log level
     */
    void log(const std::string& msg, const LogLevel severity = LogLevel::DEBUG) override;

    /**
     * @brief Log entry
     * @param required log message
     * @param log level
     */
    template <class T>
    void log(const T& t, const LogLevel level = LogLevel::DEBUG)
    {
        std::stringstream ss;
        ss << t;
        log(ss.str(), level);
    }

    /**
     * @brief Fun to stop the thread
     */
    void stop();

    /**
     * @brief Stringify the log level to its name
     * @param required LogLevel
     */
    std::string stringifyLogLvl(const LogLevel level);

private:
    FileLogger();
    ~FileLogger();

    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    /**
     * @brief Syncs the number of log files in the folder
     */
    void fileSync();

    /**
     * @brief Gets current data
     * @return "dd.mm.yy"
     */
    std::string getCurrentDate();

    /**
     * @brief Get the name of the logger file
     * @return "Log-dd.mm.yy.txt"
     */
    std::string getFileName();

    /**
     * @brief Get the name of the logger folder
     * @return "Log\\"
     */
    std::string getFldName();


    /**
     * @brief Loop to synchronize all threads
     */
    void run();

    /**
     * @brief Marks to current time
     * @return time format [yyyy.mm.dd. hh.mm.ss.ms]
     */
    std::string timestamp();
    /**
     * @brief Marks to current thread
     * @return curent thread in string format
     */
    static std::string threadID();

    /**
     * @brief Converts the std::filesystem::last_write_time to time_t
     * @return the time in the format time_t
     */
    template <typename TP>
    std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    /**
     * @brief Wraps the value of a variable
     * @return [value]
     */
    static std::string wrapValue(const std::string& value, const BlockWrapper& blockWerapper);

private:
    std::string _fileName;
    LogOutput _output = LogOutput::EVERYWHERE;
    std::fstream _file;

    BlockWrapper _blockWrapper = {"[", "]"};

    bool _isOpened = false;

    std::thread _Thread;
    std::condition_variable _CV;
    std::mutex _mMutex;
    std::queue<std::map<std::string, LogLevel>> _Queue;
    bool _Stop = false;
};
}
