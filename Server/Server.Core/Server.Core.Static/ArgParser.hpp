#pragma once

#include <argparse/argparse.hpp>
#include <FileLogger.hpp>
#include <Settings.hpp>

using Server::Builder::ParamType;

/**
* @class ArgParser.
* @brief This class parses arguments with which application is started.
* @details
* For example: "--serverport=65001", "--port=5432", "--dbname=juniorgram", "--hostaddr=127.0.0.1", "--user=postgres", "--password=postgres".
* Sending the port to the Server class constructor and list of all arguments to the PostgreAdapter Instance Method.
*/
class ArgParser
{
public:
    /**
    * @brief It's deleted default constructor.
    * @details There is no need to use this.
    */
    ArgParser() = delete;
   
    /**
    * @brief It's deleted copy constructor.
    * @details There is no need to use this.
    */
    ArgParser(const ArgParser&) = delete;
   
    /**
    * @brief Used to take parameters from main function.
    * @details Parses arguments which were set when application was run.
    * @param int argc(amount of arguments), const char** argv(C-string arguments).
    */
    explicit ArgParser(int argc, const char** argv) 
    {
        _argParser = argparse::ArgumentParser("ServerArguments", "0.6",
        argparse::default_arguments::none);

        _argParser.set_assign_chars("=");
        _argParser.add_description("Server side of juniorgram messenger. Just have a fun!");
        _argParser.add_epilog("Some parts of the program may be configured by default.");

        _argParser.add_argument(ParamType::ServerPort)
            .default_value<std::string>("65001")
            .help("Set the port value for server needs")
            .action([](const std::string& value){
                auto port = static_cast<uint32_t>(std::stoi(value));
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    return std::to_string(65001);
                }
                return std::to_string(port);
            });

        _argParser.add_argument(ParamType::DBPort)
            .default_value<std::string>("5432")
            .help("Set the database port value")
            .action([](const std::string& value){
                auto port = static_cast<uint32_t>(std::stoi(value));
                if (port > std::numeric_limits<uint16_t>::max())
                {
                    return std::to_string(5432);
                }
                return std::to_string(port);
            });

        _argParser.add_argument(ParamType::DBName)
            .default_value<std::string>("juniorgram")
            .help("set the name of database")
            .action([](const std::string& value) {
                if (const std::vector<std::string> choices = {"juniorgram", "testdb"};
                    std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"juniorgram"};
            });

        _argParser.add_argument(ParamType::HostAddress)
            .default_value<std::string>("127.0.0.1")
            .help("set the host address (optional argument)")
            .action([](const std::string& value) {
                if (const std::vector<std::string> choices = {"127.0.0.1"};
                    std::find(choices.begin(), choices.end(), value) != choices.end())
                {
                    return value;
                }
                return std::string{"127.0.0.1"};
            });

        _argParser.add_argument(ParamType::DBUser)
            .default_value<std::string>("postgres")
            .help("enter the user name (optional argument)");

        _argParser.add_argument(ParamType::DBPassword)
            .default_value<std::string>("postgres")
            .help("enter the user password (optional argument)");

        _argParser.add_argument("-h", "--help")
            .action([=](const std::string&) {
                std::cout << _argParser << std::endl;
            })
            .default_value(false)
            .help("shows help message")
            .implicit_value(true)
            .nargs(0);

        try
        {
            _argParser.parse_args(argc, argv);
        }
        catch (const std::exception& err)
        {
            Base::Logger::FileLogger::getInstance().log
            (
                err.what(), 
                Base::Logger::LogLevel::ERR
            );
            throw;
        }
    }

    /**
    * @brief Used to bind the argument and its value.
    * @details Pairs arguments with their values.
    * @param std::string const parameter(argument name).
    */

    std::pair<std::string, std::string> getPair(std::string const& parameter) const
    {
        std::pair<std::string, std::string> args = 
            std::make_pair(parameter, _argParser.get(parameter));
        return args;
    }
    
    /**
    * @brief Used to generate Settings class object
    * @details Fills Settings class object with the current parameter values and returns it
    */
    std::unique_ptr<Server::Builder::Settings> getSettings() const
    {
        auto getPair = [this](const std::string& str)
                       {
                           return std::pair<std::string, std::string>(str, _argParser.get(str));
                       };

        auto result = std::make_unique<Server::Builder::Settings>();
        (*result).setValue(getPair(ParamType::ServerPort))
                 .setValue(getPair(ParamType::HostAddress))
                 .setValue(getPair(ParamType::DBName))
                 .setValue(getPair(ParamType::DBPort))
                 .setValue(getPair(ParamType::DBUser))
                 .setValue(getPair(ParamType::DBPassword));
        return result;
    }

private:
    argparse::ArgumentParser _argParser;
};
