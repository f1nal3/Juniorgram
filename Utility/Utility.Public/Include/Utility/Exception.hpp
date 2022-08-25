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

    ss << msg << "\n in file: " << file << "\n on the line: " << line << std::endl;

    return ss.str();
}

class NotImplementedException : public std::exception
{
public:
    explicit NotImplementedException(const char* msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
    {
    }

    explicit NotImplementedException(const std::string& msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
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
    explicit OperationDBException(const char* msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
    {
    }

    explicit OperationDBException(const std::string& msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
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
    explicit LoggingException(const char* msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
    {
        Base::Logger::FileLogger::getInstance().error(_msg);
    }

    explicit LoggingException(const std::string& msg, const char* file, std::uint16_t line)
        : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
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
