#pragma once

#include <Logger/ILogger.hpp>

#include <sstream>
#include <fstream>
#include <filesystem>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <queue>
#include <string>

namespace Base::Logger
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
     * @brief Initialization of the current class object
     * @param filename std::string name to write data
     * @param output LogOutput - the place where the data will be displayed
     */
    void init(const std::string& filename, LogOutput output);

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
     * @param msg std::string - message that should be written at log
     * @param severity LogLevel - level of message
     */
    void log(const std::string& msg, LogLevel severity = LogLevel::DEBUG) override;

    /**
     * @brief Log entry
     * @param t T& - templated type of log message
     * @param level LogLevel - level of message
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
     * @param level LogLevel - element of LogLevel enumeration that should be stringify
     */
    static std::string stringifyLogLvl(LogLevel level);

    /**
     * @brief Deleted copy constructor in singleton pattern purpose
     */
    FileLogger(const FileLogger&) = delete;

    /**
     * @brief Deleted assigning operator in singleton pattern purpose
     */
    FileLogger& operator=(const FileLogger&) = delete;
private:
    FileLogger();
    ~FileLogger() override;

    /**
     * @brief Syncs the number of log files in the folder
     */
    void fileSync() const;

    /**
     * @brief Get the name of the logger file
     * @return "Log-dd.mm.yy.txt"
     */
    std::string genDateTimeFileName() const;

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
    static std::time_t to_time_t(std::filesystem::file_time_type timeType);

    /**
     * @brief Wraps the value of a variable
     * @return [value]
     */
    static std::string wrapValue(const std::string& value, const BlockWrapper& blockWrapper);
    
private:
    std::string  _fileNamePrefix{"Log-"};
    LogOutput    _output = LogOutput::EVERYWHERE;
    std::fstream _file;

    BlockWrapper _blockWrapper = {"[", "]"};

    bool _isOpened = false;

    std::thread             _loggerThread;
    std::condition_variable _inputWait;
    std::mutex              _mutex;
    std::queue<std::string> _msgQueue;
    bool                    _stop = false;
};
}  // namespace Base::Logger
