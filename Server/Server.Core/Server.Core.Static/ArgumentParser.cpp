#include "ArgumentParser.hpp"


ArgumentParser::ArgumentParser(int argc, const char** argv, const KeysValidator& validator)
{
    this->validator = validator;
    std::vector<std::string> tempParams(&argv[0], &argv[argc]);

    for (auto&& param : tempParams)
        trim(param);

    validateArgumentsAmount(tempParams);

    // path to project isn't necessary for us, so we start from i = 1.
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

    bool listenedPort = isKeyExist(validator.keys.listenedPort);
    bool fileDB       = isKeyExist(validator.keys.fileDB);

    if (listenedPort && fileDB) 
        throw std::runtime_error("Simultaneous existence both listenedPort and fileDB keys is forbidden. Use only one key.");

    if (!listenedPort && !fileDB) 
        throw std::runtime_error("There is no used key. Use only one key.");
}

uint16_t ArgumentParser::getPort() const
{
    if (!isKeyExist(validator.keys.listenedPort)) 
        throw std::runtime_error("FileDB doesn't have a port");

    int32_t port = arguments.find(validator.keys.listenedPort)->second;

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
    auto iter = str.begin();
    if (str[0] == '-') 
        ++iter;
    while (iter != str.end())
    {
        if (std::isdigit(*iter))
            ++iter;
        else
            return false;
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
