#include "FileLogger.hpp"
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <functional>

#include <Utility/Utility.hpp>
#include "Utility/UtilityTime.hpp"

using namespace Base::Logger;
using UtilityTime::safe_localtime;

FileLogger& FileLogger::getInstance()
{
    static FileLogger self;
    return self;
}

FileLogger::FileLogger() { _loggerThread = std::thread(&FileLogger::run, this); }

FileLogger::~FileLogger()
{
    if (_loggerThread.joinable())
    {
        stop();
        _loggerThread.join();
    }
}

void FileLogger::init(const std::string& filename, const LogOutput output = LogOutput::EVERYWHERE)
{
    _fileNamePrefix = filename;
    _output   = output;
}

void FileLogger::open()
{
    if (_output == LogOutput::EVERYWHERE || _output == LogOutput::FILE)
    {
        _file.open(Utility::getFldPath() + "\\" + genDateTimeFileName(), std::ios::app);
        fileSync();
        _isOpened = _file.is_open();

        if (!_isOpened)
        {
            throw std::runtime_error("Couldn't open a log file");
        }
    }
}

void FileLogger::close()
{
    if (_output == LogOutput::EVERYWHERE || _output == LogOutput::FILE)
    {
        _file.close();
    }
}

void FileLogger::stop()
{
    std::unique_lock lk(_mutex);
    _stop = true;
    _inputWait.notify_one();
}

void FileLogger::log(const std::string& msg, const LogLevel level)
{
    std::string result = wrapValue(timestamp(), _blockWrapper) +
        " " + wrapValue(threadID(), _blockWrapper) + 
        " " + wrapValue(stringifyLogLvl(level), _blockWrapper) +
        " " + msg;

    std::unique_lock lock(_mutex);
    _msgQueue.push_back(result);
    _inputWait.notify_one();
}

std::string FileLogger::timestamp()
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::system_clock;
    using UtilityTime::RTC;
    using UtilityTime::timestamp_t;

    timestamp_t secDivider      = 1000;
    timestamp_t logMilliseconds = duration_cast<milliseconds>(RTC::now().time_since_epoch()).count() % secDivider;
    timestamp_t rawTime         = system_clock::to_time_t(RTC::now());

    std::tm safeTime = safe_localtime(rawTime);

    const unsigned sizeBuffer      = 26;
    char           buf[sizeBuffer] = {0};

    /// MinGW will warning if we put this string directly
    std::string_view timeFormat = "%F %T";
    std::strftime(buf, sizeBuffer, timeFormat.data(), &safeTime);

    std::string str = std::string(buf);

    std::stringstream ss;
    ss << str << "." << std::setfill('0') << std::setw(3) << logMilliseconds;

    return ss.str();
}

std::string FileLogger::threadID()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

std::time_t FileLogger::to_time_t(std::filesystem::file_time_type timeType)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(timeType - std::filesystem::file_time_type::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

std::string FileLogger::wrapValue(const std::string& value, const BlockWrapper& blockWrapper)
{
    return blockWrapper.first + value + blockWrapper.second;
}

void FileLogger::run()
{
    while (true)
    {
        // Wait until some request comes or stop flag is set
        std::unique_lock<std::mutex> lock(_mutex);
        _inputWait.wait(lock, [&]() { return _stop || !_msgQueue.empty(); });

        // Stop if needed
        if (_stop) break;

        std::string msg = _msgQueue.pop_front();
        
        switch (_output)
        {
            case Base::Logger::LogOutput::CONSOLE:
                std::cout << msg << std::endl;
                break;
            case Base::Logger::LogOutput::FILE:
                open();
                _file << msg << std::endl;
                close();
                break;
            case Base::Logger::LogOutput::EVERYWHERE:
                open();
                std::cout << msg << std::endl;
                _file << msg << std::endl;
                close();
                break;
            default:
                break;
        }
    }
}

std::string FileLogger::genDateTimeFileName() const
{
    return _fileNamePrefix + UtilityTime::getStringifiedCurrentDate() + ".log";
}

void FileLogger::fileSync() const
{
    using hz = std::pair<std::time_t, std::filesystem::path>;
    std::vector<hz> listLogFiles;

    for (auto& p : std::filesystem::directory_iterator(Utility::getFldPath()))
    {
        std::time_t tt = to_time_t(std::filesystem::last_write_time(p.path()));
        listLogFiles.emplace_back(tt, p.path());
    }

    sort(listLogFiles.rbegin(), listLogFiles.rend());

    while (listLogFiles.size() > lifeTime)
    {
        std::filesystem::remove(listLogFiles.back().second);
        listLogFiles.pop_back();
    }
}

std::string FileLogger::stringifyLogLvl(const LogLevel level)
{
    std::string                                  result   = "NONE";
    const static std::map<LogLevel, std::string> LevelMap = {
        {LogLevel::ERR, "ERROR"},
        {LogLevel::WARNING, "WARNING"},
        {LogLevel::INFO, "INFO"},
        {LogLevel::DEBUG, "DEBUG"},
    };

    auto it = LevelMap.find(level);
    if (it != LevelMap.end())
    {
        result = it->second;
    }
    return result;
}

void FileLogger::error(const std::string& message)
{
    log(message, LogLevel::ERR);
}

void FileLogger::warning(const std::string& message)
{
    log(message, LogLevel::WARNING);
}

void FileLogger::info(const std::string& message)
{
    log(message, LogLevel::INFO);
}

void FileLogger::debug(const std::string& message)
{
    log(message, LogLevel::DEBUG);
}

void FileLogger::logAndThrow(const char* filename, std::uint16_t line, std::exception* exception, LogLevel severity)
{
    log('"' + std::string(exception->what()) + "\" on line " + std::to_string(line) + " in the file " + filename, severity);
    throw exception;
}
