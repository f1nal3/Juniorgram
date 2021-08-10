#pragma once

#include <algorithm>
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
#include <vector>

#include "../Utility.Public/Include/Utility/Utility.hpp"

class Logger final  // Singolton class
{
public:
    /**
    * @brief Valid log levels
    * ERROR -   Task cannot continue and component cannot function
    * WARNING - Potential error or impending error
    * INFO -    General information outlining overall task progress
    * DEBUG -   General information detailing debug progress
    */
    enum class LogLevel
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    /**
    * @brief Place of output of logged information
    */
    enum class LogOutput
    {
        CONSOLE,
        FILE,
        EVERYWHERE
    };

    using BlockWrapper = std::pair<std::string, std::string>;

    /**
     * @brief Gets an object of the current class
     * @return object Logger
     */
    static Logger& getInstance();

    /**
     * @brief Initialization of the current class object
     * @param file name to write data
     * @param the place where the data will be displayed
     */
    void init(const std::string& filename, const LogOutput output);

    /**
     * @brief Opening the current class file
     */
    void open();

    /**
     * @brief Closing the current class file
     */
    void close();

    /**
     * @brief Log entry
     * @param required log message
     * @param log level
     */
    void log(const std::string& msg, const LogLevel severity = LogLevel::DEBUG);

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
    inline std::string stringify(const LogLevel level);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

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
    static std::string timestamp();
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

    std::thread _mThread;
    std::condition_variable _mCV;
    std::mutex _mMutex;
    std::queue<std::map<std::string, LogLevel>> _mQueue;
    bool _mStop = false;
};
