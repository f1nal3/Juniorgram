#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class KeysValidator class.
 * @brief Class is used to validate incoming keys
 * @details A class that checks given arguments for correctness. Contains the validation logic and a list of all arguments that work for all operating systems
 */
class KeysValidator
{
private:
    using initList = std::initializer_list<std::string>;

private:
    /**
     * @struct Keys
     * @brief Includes valid keys for this platform
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
     * @brief The custom constructor initialized by extra arguments
     * @details In case we want to use some extra arguments and don't want to store that argument in the keyValidator list, we can use this constructor. Exception safe method. Calls default constructor inside.
     * @param const initList& validKeysList - list of arguments that don't need additional value, unary argument
     *const initList& keysWithoutValuesList - list of arguments that need additional value
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
     * @brief Checks for the presence in the general list of arguments.
     * @param const std::string& incomingKey - argument to check.
     * @return Returns true if the argument in the incomingKey variable is present in the list.
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
