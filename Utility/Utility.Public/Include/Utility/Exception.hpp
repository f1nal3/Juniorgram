#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include <FileLogger.hpp>

namespace Utility
{
/// Formats exception error message with file:line
inline std::string formatExceptionMessage(const std::string& msg, const std::string& file, std::uint16_t line)
{
    std::stringstream ss;

    ss << msg << " in file: " << file << " on the line: " << line << std::endl;

    return ss.str();
}

class NotImplementedException : public std::exception
{
public:
    explicit NotImplementedException(const char* msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
    }

    explicit NotImplementedException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
    }

    [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

private:
    std::string   _msg;
    std::string   _file;
    std::uint16_t _line;
};

class OperationDBException : public std::exception
{
public:
    explicit OperationDBException(const char* msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
    }

    explicit OperationDBException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
    }

    [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

private:
    std::string   _msg;
    std::string   _file;
    std::uint16_t _line;
};

class LoggingException : public std::exception
{
public:
    explicit LoggingException(const char* msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    explicit LoggingException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_)), _file(file_), _line(line_)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

private:
    std::string   _msg;
    std::string   _file;
    std::uint16_t _line;
};
}  // namespace Utility
