#include "Logger.hpp"
#include <iostream>

namespace LOG
{
    std::string Logger::stringify(const LogLevel level)
    {
        std::string result = "NONE";
        const static std::map<LogLevel, std::string> S_LEVEL_MAP =
        {
            {LogLevel::ERROR, "ERROR"},
            {LogLevel::WARNING, "WARNING"},
            {LogLevel::INFO, "INFO"},
            {LogLevel::DEBUG, "DEBUG"},
        };

        auto it = S_LEVEL_MAP.find(level);
        if (it != S_LEVEL_MAP.end())
        {
            result = it->second;
        }
        return result;
    }

    Logger& Logger::get()
    {
        static Logger self;
        return self;
    }

    void Logger::init(const std::string& filename, const LogOutput output = LogOutput::EVERYWHERE)
    {
        m_fileName = filename;
        m_output = output;
    }

    void Logger::open()
    {
        if (m_isOpened)
        {
            return;
        }

        if (m_output == LogOutput::EVERYWHERE || m_output == LogOutput::FILE)
        {
            m_file.open(m_fileName, std::ios::out);
            m_isOpened = m_file.is_open();

            if (!m_isOpened)
            {
                throw std::runtime_error("Couldn't open a log file");
            }
        }
    }

    void Logger::close()
    {
        if (m_output == LogOutput::EVERYWHERE || m_output == LogOutput::FILE)
        {
            m_file.close();
        }
    }

    void Logger::log(const std::string& msg, const LogLevel level)
    {
        if (!m_isOpened)
        {
            return;
        }

        std::string result = wrapValue(timestamp(), m_blockWrapper) +
            " " + wrapValue(threadID(), m_blockWrapper) +
            " " + wrapValue(stringify(level), m_blockWrapper) +
            " " + msg;

        std::unique_lock<std::mutex> locker(m_logMutex);
        if (m_output == LogOutput::CONSOLE)
        {
            std::cout << result << std::endl;
        }
        else if (m_output == LogOutput::FILE)
        {
            m_file << stringify(level) << "." << msg << std::endl;
        }
        else
        {
            std::cout << result << std::endl;
            m_file << result << std::endl;
        }
    }


    std::string Logger::timestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        time_t tt = system_clock::to_time_t(now);
        auto ms = duration_cast<microseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(localtime(&tt), "%F %T");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();

        return ss.str();
    }
    std::string Logger::threadID()
    {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str();
    }
    std::string Logger::wrapValue(const std::string& value, const BlockWrapper& blockWrapper)
    {
        return blockWrapper.first + value + blockWrapper.second;
    }
} // namespace LOG
