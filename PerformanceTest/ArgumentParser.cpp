#include "ArgumentParser.hpp"

namespace PerformanceTest
{
	bool ArgumentParser::Validator::isArgument(const std::string_view& arg)
	{
		return arg[0] == '-';
	}

	bool ArgumentParser::Validator::isArgumentValid(const std::string_view& arg)
	{
		std::array<argument, 3> argTypes
		{
			argument::path, 
			argument::count, 
			argument::test 
		};
		auto isValid = [arg](argument type) { return arg[1] == static_cast<char>(type); };

		return std::any_of(argTypes.begin(), argTypes.end(), isValid);
	}

	bool ArgumentParser::Validator::isValueValid(const argument arg, const std::string_view& value)
	{
		switch (arg)
		{
		case argument::path:
			return fs::exists(value);
		case argument::count:
			return !value.empty()
				&& (std::atoi(value.data()) < _MAX_COUNT);
				// WHY IS IT NOT COMPILING!??!!?
				//&& std::all_of(value.cbegin(), value.cend(), std::isdigit);
		case argument::test:
			return value == "true" || value == "false";
		}

		return false;
	}

	ArgumentParser::argument	  ArgumentParser::privateParseArgument(const std::string_view& arg)
	{
		if (Validator::isArgument(arg) && Validator::isArgumentValid(arg))
		{
			return static_cast<argument>(arg.at(1));
		}
			
		throw std::runtime_error("Invalid ArgumentParser::argument! - " + std::string(arg));
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
			case argument::test:
				return value == "true" ? true : false;
			}
		}

		throw std::runtime_error("Invalid value! - " + std::string(value));
	}

	ArgumentParser::ArgumentParser(int argc, char** argv)
	{
		if (argc > 7)
			throw std::runtime_error("Too many arguments!");

		std::vector<std::string_view> args(&argv[0], &argv[argc]);

		argument	  arg;
		argumentValue value;
		for (std::size_t argIndex = 1; argIndex < argc; argIndex += 2)
		{
			
			arg   = this->privateParseArgument(args.at(argIndex));
			value = this->privateParseValue(arg, args.at(argIndex));

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

	bool ArgumentParser::getTestArgument(void) const
	{
		return std::get<bool>(_args.at(eKeys::test));
	}
}
