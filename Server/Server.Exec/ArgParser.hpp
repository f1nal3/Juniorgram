#pragma once

#include <argparse/argparse.hpp>
#include <FileLogger.hpp>

/**
 * @class ArgParser.
 * @brief This class parses arguments with which application is started.
 * @details
 * For example: "--serverport 5432 --port 65001 --dbname juniorgram --hostaddr 127.0.0.1 --user postgres --password postgres".
 * Sending the port to the Server class constructor and list of all arguments to the PostgreAdapter Instance Method.
 */
class ArgParser
{
private:
    argparse::ArgumentParser _argParser;

public:
    /**
     * @brief It's deleted default constructor. There is no need to use this.
     */
    ArgParser() = delete;
   
    /**
     * @brief It's deleted copy constructor. There is no need to use this.
     */
    ArgParser(const ArgParser&) = delete;
   
    /**
     * @brief Used to take parameters from main function.
     * @details Parses arguments which were set when application was run.
     * @param int argc(amount of arguments), const char** argv(C-string arguments).
     */
    explicit ArgParser(int argc, const char** argv) noexcept
    {
        _argParser = argparse::ArgumentParser("ServerArguments", "1.0",
            argparse::default_arguments::none);
        
        _argParser.add_description("Server side of juniorgram messenger. Just have a fun!");
        _argParser.add_epilog("Some parts of the program may be configured by default.");

        _argParser.add_argument("--serverport")
            .default_value<std::string>("65001") 
            .help("Set the port value for server needs")
            .action([](const std::string& value) -> uint16_t {
                uint16_t port = std::stoi(value);
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    Base::Logger::FileLogger::getInstance().log
                    (
                        "Bad port value",
                        Base::Logger::LogLevel::ERR
                    );
                    
                    return 65001;
                }
                return port;
            });

        _argParser.add_argument("--port")
            .default_value<std::string>("5432")
            .help("Set the database port value")
            .action([](const std::string& value) -> uint16_t {
                uint16_t port = std::stoi(value);
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    Base::Logger::FileLogger::getInstance().log
                    (
                        "Bad DataBase port value",
                        Base::Logger::LogLevel::ERR
                    );
                    return 5432;
                }
                return port;
            });

        _argParser.add_argument("--dbname")
            .default_value<std::string>("juniorgram")
            .help("set the name of database")
            .action([](const std::string& value) {
                const std::vector<std::string> choices = {"juniorgram", "testdb"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"juniorgram"};
            });

        _argParser.add_argument("--hostaddr")
            .default_value<std::string>("127.0.0.1")
            .help("set the host address (optional argument)")
            .action([](const std::string& value) {
                const std::vector<std::string> choices = {"127.0.0.1"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"127.0.0.1"};
            });

        _argParser.add_argument("--user")
            .default_value<std::string>("postgres")
            .help("enter the user name (optional argument)");

        _argParser.add_argument("--password")
            .default_value<std::string>("postgres")
            .help("enter the user password (optional argument)");

        _argParser.add_argument("-h", "--help")
            .action([=](const std::string& ) {
                std::cout << _argParser << std::endl;
                std::exit(0);
            })
            .default_value(false)
            .help("shows help message")
            .implicit_value(true)
            .nargs(0);

        try
        {
            _argParser.parse_args(argc, argv);
        }
        catch (const std::exception err)
        {
            Base::Logger::FileLogger::getInstance().log
            (
                err.what(), 
                Base::Logger::LogLevel::ERR
            );
            std::exit(1);
        }
    }  

    std::pair<std::string, std::string> getPair(const std::string parameter) noexcept
    {
        std::pair<std::string, std::string> args = 
            std::make_pair(parameter, _argParser.get(parameter));
        return args;
    }
};
