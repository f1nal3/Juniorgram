#pragma once
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "KeysValidator.h"

class ArgumentParser
{
private:
    KeysValidator validator;
    std::map<std::string, int32_t> arguments;
    bool realDB;

public:
    ArgumentParser()                      = delete;
    ArgumentParser(const ArgumentParser&) = delete;
    explicit ArgumentParser(int argc, const char** argv,
                            const KeysValidator& validator = KeysValidator());

    uint16_t getPort() const;
    bool isRealDB() const noexcept { return realDB; }

private:
    bool doMapContainKey(const std::string& incomingKey) const noexcept
    {
        return arguments.find(incomingKey) != arguments.end();
    }

    void checkOnBadAmount(std::vector<std::string>& params) const;
    std::string trim(std::string& row) const noexcept;
    bool isInteger(const std::string& str) const noexcept;
    void tryPushToMap(const std::string& key, const std::string& value);
};

ArgumentParser::ArgumentParser(int argc, const char** argv, const KeysValidator& validator)
{
    this->validator = validator;
    std::vector<std::string> tempParams(&argv[0], &argv[argc]);

    for (auto&& param : tempParams)
    {
        trim(param);
    }

    checkOnBadAmount(tempParams);

    // path to project don't need for us, so we start from i = 1.
    size_t i = 1;
    while (i < tempParams.size())
    {
        std::string key = tempParams[i];
        std::string value;

        if (!validator.isKeyValid(key))
        {
            throw std::runtime_error("Key '" + key + "' is unvalid");
        }

        if (validator.doKeyNeedValue(key))
        {
            value = tempParams[i + 1];
            i += 2;
        }
        else
        {
            value = "0";
            i++;
        }

        if (doMapContainKey(key))
        {
            throw std::runtime_error("Arguments have duplicated keys '" + key + "'");
        }

        tryPushToMap(key, value);
    }

    realDB      = doMapContainKey(validator.keys.realDB);
    bool fileDB = doMapContainKey(validator.keys.fileDB);

    if (realDB && fileDB)
    {
        throw std::runtime_error("Coexistence realDB and fileDB in arguments");
    }

    if (!realDB && !fileDB)
    {
        throw std::runtime_error("realDB or fileDB key must be in arguments");
    }
}

uint16_t ArgumentParser::getPort() const
{
    if (!realDB)
    {
        throw std::runtime_error("FileDB doesn't have a port");
    }

    int32_t port = arguments.find(validator.keys.realDB)->second;

    if (port < std::numeric_limits<uint16_t>::min() || port > std::numeric_limits<uint16_t>::max())
    {
        throw std::runtime_error("Port value is too small or big");
    }

    return static_cast<uint16_t>(port);
}

void ArgumentParser::checkOnBadAmount(std::vector<std::string>& params) const
{
    const auto amountOfKeysWithValueAndPath =
        std::count_if(params.begin(), params.end(),
                      [this](std::string& param) { return validator.doKeyNeedValue(param); });

    if (amountOfKeysWithValueAndPath % 2 == 0 || params.size() == 1)
    {
        throw std::runtime_error("Bad arguments amount");
    }
}

bool ArgumentParser::isInteger(const std::string& str) const noexcept
{
    auto numberElement = str.begin();
    if (str[0] == '-')
    {
        numberElement++;
    }

    while (numberElement != str.end())
    {
        if ((*numberElement < '0') || (*numberElement > '9'))
        {
            return false;
        }

        numberElement++;
    }

    return true;
}

std::string ArgumentParser::trim(std::string& row) const noexcept
{
    if (!row.empty())
    {
        while (row[0] == ' ')
        {
            row.erase(row.begin());
        }

        while (row[row.size() - 1] == ' ')
        {
            row.erase(row.end() - 1);
        }
    }

    return row;
}

void ArgumentParser::tryPushToMap(const std::string& key, const std::string& value)
{
    if (!isInteger(value))
    {
        throw std::runtime_error("key " + key + " has unvalid value");
    }

    arguments.emplace(key, stoi(value));
}
