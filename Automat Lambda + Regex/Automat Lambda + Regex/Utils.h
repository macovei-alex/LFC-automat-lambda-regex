#pragma once

#include <string>

class Utils
{
public:
	Utils() = delete;

	static std::string GetNumberFromStateStr(const std::string& state);
	static int GetNumberFromStateInt(const std::string& state);
	static std::string GetLetterFromState(const std::string& state);
};
