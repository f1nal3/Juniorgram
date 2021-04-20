#pragma once

#include <stdexcept>
#include <sstream>
#include <string>

inline std::string formatExceptionMessage(const std::string& msg, const std::string& file, std::uint8_t line)
{
    std::stringstream ss;

    ss << msg << "\n in file: " << file << "\n on the line: " << line << std::endl;

    return ss.str();
}
namespace Utility
{
    class NotImplementedException : public std::exception
    {
        public:
        explicit NotImplementedException(const char* msg, const char* file, std::uint8_t line)
            : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
        {}

        explicit NotImplementedException(const std::string& msg, const char* file, std::uint8_t line)
            : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
        {}

        virtual const char* what() const noexcept
        {
            return _msg.c_str();
        }
        virtual ~NotImplementedException() = default;
        
        protected:
        std::string _msg;
        std::string _file;
        std::uint8_t _line;
    };

    class OperationDBException : public std::exception
    {
    public:
        explicit OperationDBException(const char* msg, const char* file, std::uint8_t line)
            : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
        {
        }

        explicit OperationDBException(const std::string& msg, const char* file, std::uint8_t line)
            : _msg(formatExceptionMessage(msg, file, line)), _file(file), _line(line)
        {
        }

        virtual const char* what() const noexcept { return _msg.c_str(); }
        virtual ~OperationDBException() = default;

    protected:
        std::string _msg;
        std::string _file;
        std::uint8_t _line;
    };
}
