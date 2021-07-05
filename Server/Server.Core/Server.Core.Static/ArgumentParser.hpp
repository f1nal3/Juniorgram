#pragma once
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>

#include "KeysValidator.hpp"

class ArgumentParser
{
private:
    KeysValidator                  validator;
    std::map<std::string, int32_t> arguments;
    bool                           realDB;

public:
    ArgumentParser()                      = delete;
    ArgumentParser(const ArgumentParser&) = delete;
    explicit ArgumentParser(int argc, const char** argv,
                            const KeysValidator& validator = KeysValidator());

public:
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

