#pragma once

#include <string_view>
#include <fmt/printf.h>
#include <fmt/color.h>

#include "Utils/Types.h"

namespace Lux
{

class Console
{
public:
	static void Init()
	{
		std::system("cd .");

	}

	template<class... Arguments>
	static void Fatal(const String& message, Arguments &&... args)
	{
		fmt::print(fg(fmt::color::crimson), fmt::format("[FATAL] {}\n", message), args...);
	}

	template<class... Arguments>
	static void Error(const String& message, Arguments &&... args)
	{
		fmt::print(fg(fmt::color::red), fmt::format("[Error] {}\n", message), args...);
	}

	template<class... Arguments>
	static void Warn(const String& message, Arguments &&... args)
	{
		fmt::print(fg(fmt::color::orange), fmt::format("[Warning] {}\n", message), args...);
	}

	template<class... Arguments>
	static void Info(const String& message, Arguments &&... args)
	{
		fmt::print(fg(fmt::color::light_green), fmt::format("[Info] {}\n", message), args...);
	}


};

}