#pragma once

#include <argparse/argparse.hpp>
#include <FileLogger.hpp>

/**
 * @class ArgParser.
 * @brief This class parses arguments with which application is started.
 * @details
 * For example: "--port 5432 --dbport 65001 --dbname juniorgram --hostaddr 127.0.0.1 --user postgres --password postgres".
 * Sending the port to the Server class constructor and list of all arguments to the PostgreAdapter Instance Method.
 */

class ArgParser
{
public:
    /**
     *@brief It's deleted default constructor. There is no need to use this.
     */
    ArgParser() = delete;
   
    /**
     *@brief It's deleted copy constructor. There is no need to use this.
     */
    ArgParser(const ArgParser&) = delete;
   
    /**
     * @brief Used to take parameters from main function.
     * @details Parses arguments which were set when application was run.
     * @param int argc(amount of arguments), const char** argv(C-string arguments).
     */
    ArgParser(int argc, const char** argv) 
    {
        _program = argparse::ArgumentParser("ServerArguments", "0.6", argparse::default_arguments::none);
        
        _program.add_description("Server side of juniorgram messenger. Just have a fun!");
        _program.add_epilog("Some part of program could be gluchennie.");

        _program.add_argument("--port")
            .default_value("5432")
            .help("Set the port value for server needs")
            .action([](std::string value) -> int {
                int port = std::stoi(value);
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    Base::Logger::FileLogger::getInstance().log("Bad port value", Base::Logger::LogLevel::ERR);
                    
                    return 5432;
                }
                return port;
            });

        _program.add_argument("--dbport")
            .default_value("65001")
            .help("Set the database port value")
            .action([](std::string value) -> int {
                int port = std::stoi(value);
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    Base::Logger::FileLogger::getInstance().log("Bad DataBase port value", Base::Logger::LogLevel::ERR);

                    return 65001;
                }
                return port;
            });

        _program.add_argument("--dbname")
            .default_value(std::string{"juniorgram"})
            .help("set the name of database")
            .action([](const std::string& value) {
                const std::vector<std::string> choices = {"juniorgram", "testdb"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"juniorgram"};
            });

        _program.add_argument("--hostaddr")
            .default_value(std::string{"127.0.0.1"})
            .help("set the host address (optional argument)")
            .action([](const std::string& value) {
                const std::vector<std::string> choices = {"127.0.0.1"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"127.0.0.1"};
            });

        _program.add_argument("--user")
            .default_value(std::string{"postgres"})
            .help("enter the user name (optional argument)");

        _program.add_argument("--password")
            .default_value(std::string{"postgres"})
            .help("enter the user password (optional argument)");

        _program.add_argument("-h", "--help")
            .action([=](const std::string& ) {
                std::cout << _program << std::endl;
                std::exit(0);
            })
            .default_value(false)
            .help("shows help message")
            .implicit_value(true)
            .nargs(0);

        try
        {
            _program.parse_args(argc, argv);
        }
        catch (std::exception err)
        {
            Base::Logger::FileLogger::getInstance().log(err.what(), Base::Logger::LogLevel::ERR);
            std::cerr << err.what() << std::endl << _program;
            std::exit(1);
        }
    }  

    std::pair<std::string, std::string> getPair(std::string parameter)
    {
        std::string val = _program.get<std::string>(parameter);
        std::pair<std::string, std::string> arg = std::make_pair(parameter, val);
        return arg;
    }

private:
    argparse::ArgumentParser _program;
};
