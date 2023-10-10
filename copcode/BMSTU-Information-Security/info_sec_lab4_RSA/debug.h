#pragma once

#include <sstream>
#include <stdlib.h>
#include <string>
#include <iostream>

#define VARIADIC_OUT(ostream) \


class Debug final
{
public:
	enum class Type
	{
		Debug,
		Info,
		Warning,
		Critical
	};

	static void setType(Type type, bool state);
	static void debug(const std::string& msg);
	static void info(const std::string& msg);
	static void warning(const std::string& msg);
	static void critical(const std::string& msg);

	template<typename T, typename ... Args>
	static void debug(const std::string& format, T value, Args&& ...args);

	template<typename T, typename ... Args>
	static void info(const std::string& format, T value, Args&& ...args);

	template<typename T, typename ... Args>
	static void warning(const std::string& format, T value, Args&& ...args);

	template<typename T, typename ... Args>
	static void critical(const std::string& format, T value, Args&& ... args);

	[[ noreturn ]] static void fatal(const std::string& msg);

	template<typename T, typename ...Args>
	[[ noreturn ]] static void fatal(const std::string& format, T value, Args &&...args);
private:
	static bool checkState(Debug::Type type);
	static void raw_format(std::ostream& stream, const std::string& format, std::string::size_type index);

	template<typename T, typename ... Args>
	static void raw_format(std::ostream& stream, const std::string& format, std::string::size_type index, T value, Args&& ...args)
	{
		auto last_idx = format.find_first_of('%', index);
		stream << format.substr(index, last_idx - index);
		if (last_idx != std::string::npos)
		{
			stream << value;
			raw_format(stream, format, last_idx + 1, args...);
		}
	}

};

template<typename T, typename ... Args>
void Debug::info(const std::string& format, T value, Args && ... args)
{
	if (checkState(Debug::Type::Info))
	{
		std::cout << "Info: ";
		raw_format(std::cout, format, 0, value, args...);
		std::cout << std::endl;
	}
}

template<typename T, typename ...Args>
void Debug::warning(const std::string& format, T value, Args && ... args)
{
	if (checkState(Debug::Type::Warning))
	{
		std::cout << "Warning: ";
		raw_format(std::cout, format, 0, value, args...);
		std::cout << std::endl;
	}
}

template<typename T, typename ... Args>
void Debug::debug(const std::string& format, T value, Args && ... args)
{
	if (checkState(Debug::Type::Debug))
	{
		std::cout << "Debug: ";
		raw_format(std::cout, format, 0, value, args...);
		std::cout << std::endl;
	}

}

template<typename T, typename ... Args>
void Debug::critical(const std::string& format, T value, Args && ... args)
{
	if (checkState(Debug::Type::Critical))
	{
		std::cerr << "Critical: ";
		raw_format(std::cerr, format, 0, value, args...);
		std::cerr << std::endl;
	}
}

template<typename T, typename ... Args>
void Debug::fatal(const std::string& format, T value, Args && ... args)
{
	std::cerr << "Fatal: ";
	raw_format(std::cerr, format, 0, value, args...);
	std::cerr << std::endl;
	std::terminate();
}
