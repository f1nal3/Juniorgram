#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class KeysValidator class.
 * @brief Class is used to validate incoming keys
 * @details listenedPort variable is used to tell the server what port to listen to /
 * fileDB variable means the zero port (ArgumentParser constructor)
 */
class KeysValidator
{
private:
    using initList = std::initializer_list<std::string>;

private:
    /**
     * @struct Keys
     * @brief Includes valid keys for this platform
     * @details We use keys for validating our port
     */
    struct Keys
    {
        const std::string listenedPort = "-p";
        const std::string fileDB       = "-d";
    };

private:
    std::vector<std::string> validKeys;
    std::vector<std::string> keysWithoutValues;

public:
    const Keys keys;
    /**
     * @brief It's default noexcept constructor
     * @details Used to initialize vectors with already valid keys to compare them with the incoming keys
     */
    KeysValidator() noexcept
    {
        validKeys.emplace_back(keys.listenedPort);
        validKeys.emplace_back(keys.fileDB);

        keysWithoutValues.emplace_back(keys.fileDB);
    }
    /**
     * @brief It's constructor which uses initializer list
     * @details Used to initialize vectors of valid keys
     * @param const initList&(Only keys which validate no-zero port), const initList&(Only keys which validate zero port)
     */
    KeysValidator(const initList& validkeysList, const initList& keysWithoutValuesList) noexcept : KeysValidator()
    {
        for (auto&& key : validkeysList) validKeys.emplace_back(key);

        for (auto&& key : keysWithoutValuesList) keysWithoutValues.emplace_back(key);
    }

    /**
     * @brief Method for checking the need for key values.
     * @param const std::string& incomingKey.
     * @return true if incomingKey should have a value, if not -> false
     */
    bool doKeyNeedValue(const std::string& incomingKey) const noexcept
    {
        return std::find(keysWithoutValues.begin(), keysWithoutValues.end(), incomingKey) == keysWithoutValues.end();
    }

    /**
     * @brief Method for checking the validity of a key.
     * @param const std::string& incoming key value.
     * @return true if incoming key exists in the memory, if not -> false
     */
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
