#include "ArgumentParser.hpp"


ArgumentParser::ArgumentParser(int argc, const char** argv, const KeysValidator& validator)
{
    this->validator = validator;
    std::vector<std::string> tempParams(&argv[0], &argv[argc]);

    for (auto&& param : tempParams)
        trim(param);

    validateArgumentsAmount(tempParams);

    // path to project don't need for us, so we start from i = 1.
    size_t i = 1;
    while (i < tempParams.size())
    {
        std::string key = tempParams[i];
        std::string value;

        if (!validator.isKeyValid(key))
            throw std::runtime_error("Key '" + key + "' is invalid");

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

        if (isKeyExist(key))
            throw std::runtime_error("Arguments have duplicated keys '" + key + "'");

        tryPushToMap(key, value);
    }

    withPort      = isKeyExist(validator.keys.withPort); // replace to the new name
    bool fileDB = isKeyExist(validator.keys.fileDB);

    if (withPort && fileDB)
        throw std::runtime_error("Coexistence withPort and fileDB in arguments");

    if (!withPort && !fileDB)
        throw std::runtime_error("withPort or fileDB key must be in arguments");
}

uint16_t ArgumentParser::getPort() const
{
    if (!withPort)
        throw std::runtime_error("FileDB doesn't have a port");

    int32_t port = arguments.find(validator.keys.withPort)->second;

    if (port < std::numeric_limits<uint16_t>::min() || port > std::numeric_limits<uint16_t>::max())
        throw std::runtime_error("Port value is too small or big");

    return static_cast<uint16_t>(port);
}

void ArgumentParser::validateArgumentsAmount(std::vector<std::string>& params) const
{
    const auto amountOfKeysWithValueAndPath =
        std::count_if(params.begin(), params.end(),
                      [this](std::string& param) { return validator.doKeyNeedValue(param); });

    if (amountOfKeysWithValueAndPath % 2 == 0 || params.size() == 1)
        throw std::runtime_error("Bad arguments amount");
}

bool ArgumentParser::isInteger(const std::string& str) const noexcept
{
    auto numberElement = str.begin();
    if (str[0] == '-')
        numberElement++;

    while (numberElement != str.end())
    {
        if ((*numberElement < '0') || (*numberElement > '9'))
            return false;

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
        throw std::runtime_error("key " + key + " has invalid value");

    arguments.emplace(key, stoi(value));
}
