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
        : _msg(formatExceptionMessage(msg_, file_, line_))
        , _file(file_)
        , _line(line_)
    {
    }

    explicit JuniorgramException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : _msg(formatExceptionMessage(msg_, file_, line_))
        , _file(file_)
        , _line(line_)
    {
    }

    [[nodiscard]] const char* what() const noexcept override { return _msg.c_str(); }

private:
    std::string   _msg;
    std::string   _file;
    [[maybe_unused]]std::uint16_t _line;
};

class NotImplementedException : public JuniorgramException
{
public:
    using JuniorgramException::JuniorgramException;
};

class OperationDBException : public JuniorgramException
{
public:
    using JuniorgramException::JuniorgramException;
};

class LoggingException : public JuniorgramException
{
public:
    explicit LoggingException(const char* msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(JuniorgramException::what());
    }

    explicit LoggingException(const std::string& msg_, const char* file_, std::uint16_t line_)
        : JuniorgramException(msg_, file_, line_)
    {
        Base::Logger::FileLogger::getInstance().error(JuniorgramException::what());     
    }
};
}  // namespace Utility
