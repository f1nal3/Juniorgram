#include "Logger.hpp"
#include <iostream>

std::string Logger::stringify(const LogLevel level)
{
    std::string result                                    = "NONE";
    const static std::map<LogLevel, std::string> LevelMap = {
        {LogLevel::ERROR, "ERROR"},
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

Logger::Logger() : mThread{}, mCV{}, mMutex{}, mQueue{}, mStop{false}
{
    mThread = std::thread(&Logger::Run, this);
}

Logger::~Logger()
{
    if (mThread.joinable())
    {
        mThread.join();
    }
}

Logger& Logger::getInstance() const
{
    static Logger self;
    return self;
}

void Logger::init(const std::string& filename, const LogOutput output = LogOutput::EVERYWHERE) const
{
    m_fileName = filename;
    m_output   = output;
}

void Logger::open() const
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

void Logger::close() const
{
    if (m_output == LogOutput::EVERYWHERE || m_output == LogOutput::FILE)
    {
        m_file.close();
    }
}

void Logger::stop()
{
    std::lock_guard<std::mutex> lk(mMutex);
    mStop = true;
    mCV.notify_one();
}

void Logger::log(const std::string& msg, const LogLevel level) const
{
    std::string result = wrapValue(timestamp(), m_blockWrapper) + " " +
                         wrapValue(threadID(), m_blockWrapper) + " " +
                         wrapValue(stringify(level), m_blockWrapper) + " " + msg;

    std::lock_guard<std::mutex> lk(mMutex);
    std::map<std::string, LogLevel> mapForQueue;
    mapForQueue.insert(make_pair(result, level));
    mQueue.push(mapForQueue);
    mCV.notify_one();
}

std::string Logger::timestamp() const
{
    using namespace std::chrono;
    auto now = system_clock::now();
    struct tm localtime;
    time_t tt = system_clock::to_time_t(now);
    auto ms = duration_cast<microseconds>(now.time_since_epoch()) % 1000;

    const unsigned sizeBuffer = 26;
    char buffer[sizeBuffer];
    localtime_s(&localtime, &tt);
    strftime(buffer, sizeBuffer, "%F %T", &localtime);
    std::string str = std::string(buffer);

    std::stringstream ss;
    ss << str << "." << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}
std::string Logger::threadID() const
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}
std::string Logger::wrapValue(const std::string& value, const BlockWrapper& blockWrapper) const
{
    return blockWrapper.first + value + blockWrapper.second;
}

void Logger::run()
{
    while (true)
    {
        // Wait until some request comes or stop flag is set
        std::unique_lock<std::mutex> lock(mMutex);
        mCV.wait(lock, [&]() { return mStop || !mQueue.empty(); });

        // Stop if needed
        if (mStop)
        {
            break;
        }

        std::string msg = std::move(mQueue.front().begin()->first);
        mQueue.pop();

        if (m_output == LogOutput::CONSOLE)
        {
            std::cout << msg << std::endl;
        }
        else if (m_output == LogOutput::FILE)
        {
            open();
            m_file << msg << std::endl;
        }
        else
        {
            open();
            std::cout << msg << std::endl;
            m_file << msg << std::endl;
        }

        lock.unlock();
    }
}
