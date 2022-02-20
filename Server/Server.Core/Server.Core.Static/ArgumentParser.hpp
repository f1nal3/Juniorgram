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
 * @brief The ArgumentParser class.
 */
class ArgumentParser
{
private:
    KeysValidator                  validator;
    std::map<std::string, int32_t> arguments;
    bool                           withPort;

public:
    ArgumentParser()                      = delete;
    ArgumentParser(const ArgumentParser&) = delete;
    explicit ArgumentParser(int argc, const char** argv,
                            const KeysValidator& validator = KeysValidator());

public:
    /**
     * @brief Method for getting the port value.
     * @return uint16_t value the port db.
     */
    uint16_t getPort() const;
    /**
     * @brief Method for checking the port value.
     * @return bool base status(true or false).
     */
    bool isWithPort() const noexcept { return withPort; }

private:
    /**
     * @brief Method for checking if arguments have an input key.
     * @param const std::string& incoming key.
     * @return true or false.
     */
    bool doMapContainKey(const std::string& incomingKey) const noexcept
    {
        return arguments.find(incomingKey) != arguments.end();
    }

    /**
     * @brief Method for checking the value of the number of arguments.
     * @param std::vector<std::string>& parameters.
     */
    void checkOnBadAmount(std::vector<std::string>& params) const;

    /**
     * @brief Method to remove spaces in a row
     * @param std::string& row
     * @return std::string without spase
     */
    std::string trim(std::string& row) const noexcept;
	
    /**
     * @brief Method for checking a string for numbers.
     * @param const std::string& str.
     * @return true or false.
     */
    bool isInteger(const std::string& str) const noexcept;

    /**
     * @brief Method for adding values in arguments.
     * @param const std::string& key and const std::string& value.
     */
    void tryPushToMap(const std::string& key, const std::string& value);  
};

