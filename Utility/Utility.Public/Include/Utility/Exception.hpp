#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include <FileLogger.hpp>

namespace Utility
{

inline std::string formatExceptionMessage(const std::string& msg, const std::string& file, std::uint16_t line)
{
    std::stringstream ss;

    ss << "File: \"" << file << "\" line: " << line << " - " << msg;

    return ss.str();
}

class JuniorgramException : public std::exception
{
public:
    explicit JuniorgramException(const char* msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)) , _file(file_), _line(line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    explicit JuniorgramException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

protected:
    std::string   _msg;
    std::string   _file;
    std::uint16_t _line;
};

class NotImplementedException : public JuniorgramException
{
public:
    explicit NotImplementedException(const char* msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    explicit NotImplementedException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }
};

class OperationDBException : public JuniorgramException
{
public:
    explicit OperationDBException(const char* msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_) 
    {
    }

    explicit OperationDBException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
    }
};

class LoggingException : public JuniorgramException
{
public:
    explicit LoggingException(const char* msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    explicit LoggingException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);     
    }
};
}  // namespace Utility
