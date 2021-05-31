#pragma once

#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>

#include <string>
#include <thread>

namespace LOG
{
    class Logger final
    {
    public:
        using BlockWrapper = std::pair<std::string /*prefix*/, std::string /*sufix*/>;

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

    private:
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger()
        {
        }
        ~Logger()
        {

        }

    private:
        // Marks to current time
        // @return time format [yyyy.mm.dd. hh.mm.ss.ms]
        static std::string timestamp();
        // Marks to current thread
        // @return curent thread
        static std::string threadID();
        // Wraps the value of a variable
        // @return [value]
        static std::string wrapValue(const std::string& value, const BlockWrapper& blockWerapper);


    public:
        // Gets an object of the current class
        // @return object Logger
        static Logger& get();
        
        // Initialization of the current class object
        // @param file name to write data
        // @param the place where the data will be displayed
        void init(const std::string& filename, const LogOutput output);
        
        // Opening the current class file
        void open();
        
        // Closing the current class file
        void close();

        // Log entry
        // @param required log message
        // @param log level
        void log(const std::string& msg, const LogLevel severity = LogLevel::DEBUG);

        // Log entry
        // @param required log message
        // @param log level
        template<class T>
        void log(const T& t, const LogLevel level = LogLevel::DEBUG)
        {
            std::stringstream ss;
            ss << t;
            log(ss.str(), level);
        }

    private:
        std::mutex m_logMutex;

        std::string m_fileName;
        std::fstream m_file;

        LogOutput m_output = LogOutput::EVERYWHERE;

        BlockWrapper m_blockWrapper = { "[","]" }; 

        bool m_isOpened = false;

    };
}  //namespace LOG
