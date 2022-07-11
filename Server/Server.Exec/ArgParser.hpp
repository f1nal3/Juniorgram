#pragma once

#include <argparse/argparse.hpp>
#include <FileLogger.hpp>

/**
 * @class ArgParser.
 * @brief This class parses arguments with which application is started.
 * @details
 * For example: "--port 5432 --dbname juniorgram --hostaddr 127.0.0.1 --user postgres --password postgres".
 * Sending the port to the Server class constructor and list of all arguments to the PostgreAdapter Instance Method.
 */

class ArgParser
{
private:
    //uint16_t    _port;
    //std::string _arguments;
    
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
        //_port   = 0;
        //_arguments = "";
        _program    = argparse::ArgumentParser("ServerArguments", "0.6", argparse::default_arguments::none);
        
        _program.add_description("Server side of juniorgram messenger. Just have a fun!");
        _program.add_epilog("Some part of program could be gluchennie.");

        _program.add_argument("-p","--port")
            .default_value("5432")
            .help("Set the port value for server needs")
            .action( [](std::string value) -> int {
                int port = std::stoi(value);
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    //@todo error 
                    return 5432;
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

      /* 
        std::string dbname = program.get<std::string>("--dbname");
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
        
        */
    }
        
     /**
     * @brief Method for adding all values in arguments.
     * @param std::string& key, const std::string& value.
     */
    /* void addArguments(const std::string key, const std::string& value) 
    { 
        if (!_arguments.empty())
        {
            _arguments += " ";
        }
        _arguments += key + "=" + value;
    }*/

    /**
     * @brief Method for getting the port value.
     * @return uint16_t value of the port db.
     */
    uint16_t getPort() 
    { 
        return 0;
    };

    /**
    * @brief Method for getting the options values.
    * @return arguments to the PostgreAdapter Instance Method.
    */
    std::string getArguments() 
    { 
        return "";
    }

    template<typename T>
    T get(std::string_view parameter) 
    {
        return _program.get<T>(parameter);
    }

    template <typename T>
    std::pair<std::string_view, T> getPair(std::string_view parameter)
    {
        auto val = _program.get<T>(parameter);
        return std::make_pair<std::string_view, T>(std::move(parameter), std::move(val));
    }

private:
    argparse::ArgumentParser _program;
};
