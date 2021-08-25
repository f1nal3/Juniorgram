#pragma once

#include "Utility/Utility.hpp"

namespace Logger
{
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
     * CONSOLE -    console output
     * FILE -       file output
     * EVERYWHERE - output to console and file
     */
    enum class LogOutput
    {
        CONSOLE,
        FILE,
        EVERYWHERE
    };

    /**
     * @brief Required storage period for log files
     */
    const uint64_t lifeTime = 7;

    /** @class ILogger
     *  @brief An interface for logger
     */
    class ILogger
    {
    public:
        /**
         * @brief Get required storage period for log files
         * @param basic file storage 7 days
         * @return storage period in days
         */
        virtual unsigned getPeriodLife(unsigned periodLife) = 0;

        /**
         * @brief Initialization of the current class object
         * @param file name to write data
         * @param the place where the data will be displayed
         */
        virtual void init(const std::string& filename, const LogOutput output) = 0;

        /**
         * @brief Opening the current class file
         */
        virtual void open() = 0;

        /**
         * @brief Closing the current class file
         */
        virtual void close() = 0;

        /**
         * @brief Log entry
         * @param required log message
         * @param log level
         */
        virtual void log(const std::string& msg, const LogLevel severity = LogLevel::DEBUG) = 0;

        /**
         * @brief Stringify the log level to its name
         * @param required LogLevel
         */
        virtual std::string stringify(const LogLevel level) = 0;
    
    };
}  // namespace Logger
