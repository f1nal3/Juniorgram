#pragma once

#include <chrono>
#include <ctime>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <queue>

class Logger final
{
public:
    using BlockWrapper = std::pair<std::string, std::string>;

public:
    // Valid log levels
    // ERROR - Task cannot continue and component cannot function
    // WARNING - Potential error or impending error
    // INFO - General information outlining overall task progress
    // DEBUG - General information detailing debug progress
    enum class LogLevel
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    // Place of output of logged information
    enum class LogOutput
    {
        CONSOLE,
        FILE,
        EVERYWHERE
    };

    // Stringify the log level to its name
    // @param required LogLevel
    inline std::string stringify(const LogLevel level);

public:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    // Marks to current time
    // @return time format [yyyy.mm.dd. hh.mm.ss.ms]
    static std::string timestamp() const;
    // Marks to current thread
    // @return curent thread
    static std::string threadID() const;
    // Wraps the value of a variable
    // @return [value]
    static std::string wrapValue(const std::string& value, const BlockWrapper& blockWerapper) const;
    // Loop to synchronize all threads
    void run();

public:
    // Gets an object of the current class
    // @return object Logger
    static Logger& getInstance() const;

    // Initialization of the current class object
    // @param file name to write data
    // @param the place where the data will be displayed
    void init(const std::string& filename, const LogOutput output) const;

    // Opening the current class file
    void open() const;

    // Closing the current class file
    void close() const;

    // Fun to stop the thread 
    void stop();

    // Log entry
    // @param required log message
    // @param log level
    void log(const std::string& msg, const LogLevel severity = LogLevel::DEBUG) const;

    // Log entry
    // @param required log message
    // @param log level
    template <class T>
    void log(const T& t, const LogLevel level = LogLevel::DEBUG) const
    {
        std::stringstream ss;
        ss << t;
        log(ss.str(), level);
    }



private:
    std::string m_fileName = "Log.txt";
    LogOutput m_output = LogOutput::EVERYWHERE;
    std::fstream m_file;

    BlockWrapper m_blockWrapper = {"[", "]"};

    bool m_isOpened = false;

    std::thread mThread;
    std::condition_variable mCV;
    std::mutex mMutex;
    std::queue<std::map<std::string, LogLevel>> mQueue;
    bool mStop;
};
