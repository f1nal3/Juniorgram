#include "FileLogger.hpp"

using namespace Logger;

std::string FileLogger::stringifyLogLvl(const LogLevel level)
{
    std::string result =        "NONE";
    const static std::map<LogLevel, std::string> LevelMap = {
        {LogLevel::ERROR,       "ERROR"},
        {LogLevel::WARNING,     "WARNING"},
        {LogLevel::INFO,        "INFO"},
        {LogLevel::DEBUG,       "DEBUG"},
    };

    auto it = LevelMap.find(level);
    if (it != LevelMap.end())
    {
        result = it->second;
    }
    return result;
}

FileLogger::FileLogger()
{
    _Thread = std::thread(&FileLogger::run, this);
}

FileLogger::~FileLogger()
{
    if (_Thread.joinable())
    {
        stop();
        _Thread.join();
    }
}

FileLogger& FileLogger::getInstance()
{
    static FileLogger self;
    return self;
}

void FileLogger::init(const std::string& filename, const LogOutput output = LogOutput::EVERYWHERE)
{
    _fileName = filename;
    _output   = output;
}

void FileLogger::open()
{
    if (_output == LogOutput::EVERYWHERE || _output == LogOutput::FILE)
    {
        _file.open(getFldName() + std::string{"\\"} + getFileName(), std::ios::app);
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
    std::lock_guard<std::mutex> lk(_Mutex);
    _Stop = true;
    _CV.notify_one();
}

void FileLogger::log(const std::string& msg, const LogLevel level)
{
    std::string result = wrapValue(timestamp(), _blockWrapper) + " " +
                         wrapValue(threadID(), _blockWrapper) + " " +
                         wrapValue(stringifyLogLvl(level), _blockWrapper) + " " + msg;

    std::lock_guard<std::mutex> lk(_Mutex);
    _msgQueue.push(result);
    _CV.notify_one();
}

std::string FileLogger::timestamp()
{
    using std::chrono::system_clock;
    system_clock::time_point tp = system_clock::now();

    time_t raw_time = system_clock::to_time_t(tp);

    std::tm tt        = Utility::safe_localtime(raw_time);
    std::tm* timeinfo = &tt;
    auto ms           = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()) % 1000;

    const unsigned sizeBuffer = 26;
    char buf[sizeBuffer]      = {0};

    strftime(buf, sizeBuffer, "%F %T", timeinfo);

    std::string str = std::string(buf);

    std::stringstream ss;
    ss << str << "." << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}
std::string FileLogger::threadID()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

std::time_t Logger::FileLogger::to_time_t(std::filesystem::file_time_type timeType)
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
        std::unique_lock<std::mutex> lock(_Mutex);
        _CV.wait(lock, [&]() { return _Stop || !_msgQueue.empty(); });

        // Stop if needed
        if (_Stop)
        {
            break;
        }

        std::string msg = std::move(_msgQueue.front());
        _msgQueue.pop();

        if (_output == LogOutput::CONSOLE)
        {
            std::cout << msg << std::endl;
        }
        else if (_output == LogOutput::FILE)
        {
            open();
            _file << msg << std::endl;
            close();
        }
        else
        {
            open();
            std::cout << msg << std::endl;
            _file << msg << std::endl;
            close();
        }
    }
}

std::string FileLogger::getCurrentDate()
{
    using std::chrono::system_clock;

    system_clock::time_point tp = system_clock::now();

    time_t raw_time = system_clock::to_time_t(tp);

    std::tm tt = Utility::safe_localtime(raw_time);
    struct tm* timeinfo = &tt;

    char buf[24] = {0};

    strftime(buf, 24, "%d.%m.%Y", timeinfo);

    return std::string(buf);
}

std::string FileLogger::getFileName()
{
    _fileName = _fileName + getCurrentDate() + ".txt";
    return _fileName;
}

std::string FileLogger::getFldName()
{
    std::filesystem::path path = "Log";
    std::filesystem::create_directory(path);
    return path.string();
}

void FileLogger::fileSync()
{
    std::vector<std::pair<std::time_t, std::filesystem::path>> VecLogFiles;

    for (auto& p : std::filesystem::directory_iterator(getFldName()))
    {
        std::time_t tt = to_time_t(std::filesystem::last_write_time(p.path()));
        VecLogFiles.push_back(std::make_pair(tt, p.path()));
    }

    sort(VecLogFiles.rbegin(), VecLogFiles.rend());

    while (VecLogFiles.size() > lifeTime)
    {
        std::filesystem::remove(VecLogFiles.back().second);
        VecLogFiles.pop_back();
    }
}
