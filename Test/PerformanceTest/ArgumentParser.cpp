#include "ArgumentParser.hpp"

#include <cctype>
#include <vector>

namespace PerformanceTest
{
	bool ArgumentParser::Validator::isArgument(const std::string_view& arg)
	{
		return arg[0] == '-';
	}

	bool ArgumentParser::Validator::isArgumentValid(const std::string_view& arg)
	{
		return static_cast<argument>(arg[1]) == argument::path || static_cast<argument>(arg[1]) == argument::count;
	}

	bool ArgumentParser::Validator::isValueValid(const argument arg, const std::string_view& value)
	{
		switch (arg)
		{
		case argument::path:
			return fs::exists(value);
		case argument::count:
            auto isdig = [](char ch) { return std::isdigit(static_cast<unsigned char>(ch)); };
			return !value.empty()
				&& (std::atoi(value.data()) < _MAX_COUNT)
				&& std::all_of(value.cbegin(), value.cend(), isdig);
		}

		return false;
	}

	ArgumentParser::argument	  ArgumentParser::privateParseArgument(const std::string_view& arg)
	{
		if (Validator::isArgument(arg) && Validator::isArgumentValid(arg))
		{
			return static_cast<argument>(arg.at(1));
		}
			
		throw std::runtime_error("Invalid argument! --- " + std::string(arg));
	}

	ArgumentParser::argumentValue ArgumentParser::privateParseValue(const argument arg, const std::string_view& value)
	{
		if (Validator::isValueValid(arg, value))
		{
			switch (arg)
			{
			case argument::path:
				return fs::path(value);
			case argument::count:
				return static_cast<std::uint8_t>(std::atoi(value.data()));
			}
		}

		throw std::runtime_error("Invalid value! --- " + std::string(value));
	}

	ArgumentParser::ArgumentParser(int argc, char** argv)
	{
		if (argc > 5)
			throw std::runtime_error("Too many arguments!");

		std::vector<std::string_view> args(&argv[0], &argv[argc]);

		argument	  arg;
		argumentValue value;
		for (std::size_t argIndex = 1; argIndex < static_cast<std::size_t>(argc); argIndex += 2)
		{

			arg   = this->privateParseArgument(args.at(argIndex));
			value = this->privateParseValue(arg, args.at(argIndex + 1));

			_args.emplace(arg, value);
		}
	}

	bool ArgumentParser::isArgumentDefind(const argument key)
	{
		return _args.find(key) != _args.end();
	}

	const fs::path& ArgumentParser::getPathArgument(void) const
	{
		return std::get<fs::path>(_args.at(eKeys::path));
	}

	std::uint8_t ArgumentParser::getCountArgument(void) const
	{
		return std::get<std::uint8_t>(_args.at(eKeys::count));
	}
}
