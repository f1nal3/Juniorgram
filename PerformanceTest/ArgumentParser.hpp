#pragma once
#include <map>
#include <array>
#include <variant>
#include <algorithm>
#include <filesystem>
#include <string_view>

namespace PerformanceTest
{
    namespace fs = std::filesystem;

    enum class eKeys : char
    {
        path = 'p',
        count = 'c',
        test = 't'
    };

	class ArgumentParser
	{
    private:

        using argument = eKeys;
        using argumentValue = std::variant<fs::path, std::uint8_t, bool>;

        class Validator
        {
        private:

            static inline const std::uint8_t _MAX_COUNT = 255;

        public:

            static bool isArgument(const std::string_view& arg);

            static bool isArgumentValid(const std::string_view& arg);

            static bool isValueValid(argument arg, const std::string_view& value);

        };

        std::map<argument, argumentValue> _args;

    private:

        argument      privateParseArgument(const std::string_view& arg);
        argumentValue privateParseValue(const argument arg, const std::string_view& value);

    public:
        
	    explicit        ArgumentParser(int argc, char** argv);

    public:

        bool            isArgumentDefind(const argument key);

        const fs::path& getPathArgument(void) const;

        std::uint8_t    getCountArgument(void) const;

        bool            getTestArgument(void) const;

    };
}
