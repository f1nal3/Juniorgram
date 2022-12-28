#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class KeysValidator class.
 * @brief Class is used to validate incoming keys.
 * @details A class that checks given arguments for correctness. 
 *  Contains the validation logic and a list of all arguments that work for all operating systems.
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
        const std::string _listenedPort = "-p";
        const std::string _fileDB       = "-d";
    };

private:
    std::vector<std::string> _validKeys;
    std::vector<std::string> _keysWithoutValues;

public:
    const Keys _keys;
    /**
     * @brief It's default noexcept constructor
     * @details Used to initialize vectors with already valid keys to compare them with the incoming keys
     */
    KeysValidator() noexcept
    {
        _validKeys.emplace_back(_keys._listenedPort);
        _validKeys.emplace_back(_keys._fileDB);

        _keysWithoutValues.emplace_back(_keys._fileDB);
    }
    /**
     * @brief The custom constructor initialized by extra arguments
     * @details In case we want to use some extra arguments and don't want to store that argument in the keyValidator list, we can use this constructor. Exception safe method. Calls default constructor inside.
     * @param const initList& validKeysList - list of arguments that don't need additional value, unary argument
     *const initList& keysWithoutValuesList - list of arguments that need additional value
     */
    KeysValidator(const initList& validkeysList, const initList& keysWithoutValuesList) noexcept : KeysValidator()
    {
        for (auto&& key : validkeysList) _validKeys.emplace_back(key);

        for (auto&& key : keysWithoutValuesList) _keysWithoutValues.emplace_back(key);
    }

    /**
     * @brief Method for checking the need for key values.
     * @param const std::string& incomingKey.
     * @return true if incomingKey should have a value, if not -> false
     */
    bool doKeyNeedValue(const std::string& incomingKey) const noexcept
    {
        return std::find(_keysWithoutValues.begin(), _keysWithoutValues.end(), incomingKey) == _keysWithoutValues.end();
    }

    /**
     * @brief Checks for the presence in the general list of arguments.
     * @param const std::string& incomingKey - argument to check.
     * @return Returns true if the argument in the incomingKey variable is present in the list.
     */
    bool isKeyValid(const std::string& incomingKey) const noexcept
    {
        return std::find(_validKeys.begin(), _validKeys.end(), incomingKey) != _validKeys.end();
    }

    KeysValidator& operator=(const KeysValidator& validator)
    {
        this->_validKeys = validator._validKeys;

        this->_keysWithoutValues = validator._keysWithoutValues;

        return *this;
    }
};
