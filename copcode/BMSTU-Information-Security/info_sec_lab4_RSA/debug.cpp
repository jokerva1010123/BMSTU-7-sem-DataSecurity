#include "debug.h"

#include <vector>

static std::vector<bool> flags = { true, true, true , true};

bool Debug::checkState(Debug::Type type)
{
	return flags[static_cast<uint8_t>(type)];
}

void Debug::raw_format(std::ostream &stream, const std::string &format, std::string::size_type index)
{
	stream << format.substr(index);
}

void Debug::setType(Debug::Type type, bool state)
{
	flags[static_cast<int8_t>(type)] = state;
}

void Debug::debug(const std::string &msg)
{
	if (checkState(Type::Debug))
	{
		std::cout << "Debug: " << msg << std::endl;
	}
}

void Debug::info(const std::string &msg)
{
	if (checkState(Type::Info))
	{
		std::cout << "Info: " << msg << std::endl;
	}
}

void Debug::warning(const std::string &msg)
{
	if (checkState(Type::Warning))
	{
		std::cout << "Warning: "<< msg << std::endl;
	}
}

void Debug::critical(const std::string &msg)
{
	if (checkState(Type::Critical))
	{
		std::cerr << "Critical: " << msg << std::endl;
	}
}

void Debug::fatal(const std::string &msg)
{
	std::cerr << "Fatal: " <<  msg << std::endl;
	std::terminate();
}
