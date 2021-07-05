#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

class KeysValidator
{
private:
    using initList = std::initializer_list<std::string>;
private:
    struct Keys
    {
        const std::string realDB = "-p";
        const std::string fileDB = "-d";
    };

private:
    std::vector<std::string> validKeys;
    std::vector<std::string> keysWithoutValues;

public:
    const Keys keys;

    KeysValidator() noexcept
    {
        validKeys.emplace_back(keys.realDB);
        validKeys.emplace_back(keys.fileDB);

        keysWithoutValues.emplace_back(keys.fileDB);
    }

    KeysValidator(const initList& validkeysList, const initList& keysWithoutValuesList) noexcept
        : KeysValidator()
    {
        for (auto&& key : validkeysList)
            validKeys.emplace_back(key);

        for (auto&& key : keysWithoutValuesList)
            keysWithoutValues.emplace_back(key);
    }

    bool doKeyNeedValue(const std::string& incomingKey) const noexcept
    {
        return std::find(keysWithoutValues.begin(), keysWithoutValues.end(), incomingKey) ==
               keysWithoutValues.end();
    }

    bool isKeyValid(const std::string& incomingKey) const noexcept
    {
        return std::find(validKeys.begin(), validKeys.end(), incomingKey) != validKeys.end();
    }

    KeysValidator& operator=(const KeysValidator& validator)
    {
        this->validKeys = validator.validKeys;

        this->keysWithoutValues = validator.keysWithoutValues;

        return *this;
    }
};
