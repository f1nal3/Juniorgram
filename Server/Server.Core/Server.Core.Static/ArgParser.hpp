#pragma once

#include <argparse/argparse.hpp>
#include <FileLogger.hpp>

/**
 * @class ArgParser.
 * @brief This class parses agruments with which application is started.
 * @details
 * For example: "--port 5432 --dbname juniorgram --hostaddr 127.0.0.1 --user postgres --password postgres".
 * Sending the port to the Server class constructor and list of all arguments to the PostgreAdapter Instance Method.
 */

class ArgParser
{
private:
    static uint16_t    _port;
    static std::string _arguments;
    
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
        argparse::ArgumentParser program("ServerArguments");

        std::cout << program << std::endl;

        program.add_argument("--port")
            .default_value("5432")
            .required()
            .help("set the port number (required argument)")
            .action([](const std::string& value) {
                static const std::vector<std::string> choices = {"5432", "65001"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"5432"};
            });

        program.add_argument("--dbname")
            .default_value(std::string{"juniorgram"})
            .help("set the name of database (optional argument)")
            .action([](const std::string& value) {
                static const std::vector<std::string> choices = {"juniorgram", "testdb"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"juniorgram"};
            });

        program.add_argument("--hostaddr")
            .default_value(std::string{"127.0.0.1"})
            .help("set the host address (optional argument)")
            .action([](const std::string& value) {
                static const std::vector<std::string> choices = {"127.0.0.1"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"127.0.0.1"};
            });

        program.add_argument("--user")
            .default_value(std::string{"postgres"})
            .help("enter the user name (optional argument)");

        program.add_argument("--password")
            .default_value(std::string{"postgres"})
            .help("enter the user password (optional argument)");

        try
        {
            program.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err)
        {
            Base::Logger::FileLogger::getInstance().log(err.what(), Base::Logger::LogLevel::ERR);
            std::cerr << program;
            std::exit(1);
        }
    
        std::string dbname   = program.get<std::string>("--dbname");
        addArguments("dbname", dbname);

        std::string hostaddr = program.get<std::string>("--hostaddr");
        addArguments("hostaddr", hostaddr);

        std::string user     = program.get<std::string>("--user");
        addArguments("user", user);

        std::string password = program.get<std::string>("--password");
        addArguments("password", password);

        std::string port     = program.get<std::string>("--port");
        addArguments("port", port);
        _port = static_cast<uint16_t>(std::stoi(port));
    }
        
     /**
     * @brief Method for adding all values in arguments.
     * @param std::string& key, const std::string& value.
     */
    void addArguments(const std::string key, const std::string& value) 
    { 
        if (!_arguments.empty())
        {
            _arguments += " ";
        }
        _arguments += key + "=" + value;
    }

    /**
     * @brief Method for getting the port value.
     * @return uint16_t value of the port db.
     */
    static uint16_t getPort() 
    { 
        return _port;
    };

    /**
    * @brief Method for getting the options values.
    * @return arguments to the PostgreAdapter Instance Method.
    */
    static std::string getArguments() 
    { 
        return _arguments;
    }
};

uint16_t ArgParser::_port = 0;
std::string ArgParser::_arguments = "";




