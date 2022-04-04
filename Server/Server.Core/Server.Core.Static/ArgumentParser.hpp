#pragma once
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

#include "KeysValidator.hpp"

/**
 * @class ArgumentParser.
 * @brief This class parses agruments with which application is started
 * @details
 * For example: "your_path -p 65001" 
 * Ignoring your_path because it isn't necessary
 * Using -p as KeyValidator variable -> incoming port will be listened
 * Sending the port to the Server class constructor
 */
class ArgumentParser
{
private:
    KeysValidator                  validator;
    std::map<std::string, int32_t> arguments;

public:
    /**
    *@brief It's deleted default constructor. There is no need to use this.
    */
    ArgumentParser()                      = delete;
    /**
     *@brief It's deleted copy constructor. There is no need to use this.
     */
    ArgumentParser(const ArgumentParser&) = delete;
    /**
     * @brief Used to take parameters from main function
     * @details Parses arguments which were set when application was run
     * @param int argc(amount of arguments), const char** argv(C-string arguments), const KeysValidator& validator = KeysValidator()
     */
    explicit ArgumentParser(int argc, const char** argv,
                            const KeysValidator& validator = KeysValidator());

public:
    /**
     * @brief Method for getting the port value.
     * @return uint16_t value of the port db.
     */
    uint16_t getPort() const;

    /**
     * @brief Method for checking the equality of incoming key
     * @param const std::string& incomingKey (we will check it)
     * @return true if incomingKey is equal to listenedPort key(Validator), if not -> false
     */
    bool isListenedPort(const std::string& incomingKey) const noexcept { return validator.keys.listenedPort == incomingKey; }

private:
    /**
     * @brief Method for checking if arguments have an input key.
     * @param const std::string& incoming key.
     * @return true if incomingKey exists in the memory, if not -> false
     */
    bool isKeyExist(const std::string& incomingKey) const noexcept { return arguments.find(incomingKey) != arguments.end(); }

    /**
     * @brief Method for checking the value of the number of arguments.
     * @param std::vector<std::string>& parameters.
     */
    void validateArgumentsAmount(std::vector<std::string>& params) const;

    /**
     * @brief Method to remove spaces in a row
     * @param std::string& row
     * @return std::string without space
     */
    std::string trim(std::string& row) const noexcept;
	
    /**
     * @brief Method for checking a string for numbers.
     * @details Checks if incoming port is valid
     * @param const std::string& str(port).
     * @return true if incoming str(port) consists of only digits, if not -> false
     */
    bool isInteger(const std::string& str) const noexcept;

    /**
     * @brief Method for adding values in arguments.
     * @param const std::string& key, const std::string& value.
     */
    void tryPushToMap(const std::string& key, const std::string& value);  
};

