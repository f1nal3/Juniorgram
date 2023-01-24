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
    explicit ArgParser(int argc, const char** argv) noexcept;

    /**
    * @brief Used to bind the argument and its value.
    * @details Pairs arguments with their values.
    * @param const std::string parameter(argument name).
    */
    std::pair<std::string, std::string> getPair(const std::string parameter) noexcept;

private:
    argparse::ArgumentParser _argParser;
};
