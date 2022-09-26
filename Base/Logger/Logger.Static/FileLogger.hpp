#pragma once

#include <Logger/ILogger.hpp>
#include "Utility/SafeQueue.hpp"

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
     * @brief Stringify the log level to its name
     * @param level LogLevel - element of LogLevel enumeration that should be stringify
     */
    static std::string stringifyLogLvl(LogLevel level);

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
     * @brief Deleted copy constructor in singleton pattern purpose
     */
    FileLogger(const FileLogger&) = delete;

    /**
     * @brief Deleted assigning operator in singleton pattern purpose
     */
    FileLogger& operator=(const FileLogger&) = delete;

    /**
     * @brief Method used to say that's going to be an error
     * @param const std::string& -> incoming message, that should describe situation or progress
     */
    void error(const std::string& message);

    /**
     * @brief Method used to say that's going to be a possible error
     * @param const std::string& -> incoming message, that should describe situation or progress
     */
    void warning(const std::string& message);

    /**
     * @brief Method used to say general information
     * @param const std::string& -> incoming message, that should describe situation or progress
     */
    void info(const std::string& message);

    /**
     * @brief Method used to say general information detailing debug progress
     * @param const std::string& -> incoming message, that should describe situation or progress
     */
    void debug(const std::string& message);

    /**
     * @brief Method used to say some information and to throw an exception
     * @param const char* -> name of file where the event took place
     * @param std::uint16_t -> line of the file where event took place
     * @param T -> std::exception based object to next throw
     * @param LogLevel - level of message
     */
    template <class T>
    typename std::enable_if_t<std::is_base_of_v<std::exception, T>, void>
    logAndThrow(const char* filename, std::uint16_t line, const T& exception, LogLevel severity = LogLevel::DEBUG)
    {
        log('"' + std::string(exception.what()) + "\" on line " + std::to_string(line) + " in the file " + filename, severity);
        throw exception;
    }

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

    /**
     * @brief Opening the current class file
     */
    void open();

    /**
     * @brief Closing the current class file
     */
    void close();

    /**
     * @brief Fun to stop the thread
     */
    void stop();
    
private:
    std::string  _fileNamePrefix{"Log-"};
    LogOutput    _output = LogOutput::EVERYWHERE;
    std::fstream _file;

    BlockWrapper _blockWrapper = {"[", "]"};

    bool _isOpened = false;

    std::thread                          _loggerThread;
    std::condition_variable              _inputWait;
    std::mutex                           _mutex;
    Utility::SafeQueue<std::string>      _msgQueue;
    bool                                 _stop = false;
};
}  // namespace Base::Logger
