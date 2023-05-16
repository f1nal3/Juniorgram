#pragma once

#include <string>

namespace Base::Logger
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
        ERR,
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
         * @brief Log entry
         * @param msg log message
         * @param severity type of message
         */
        virtual void log(const std::string& msg, LogLevel severity) = 0;
        virtual ~ILogger() = default;
    };
}  // namespace Logger
