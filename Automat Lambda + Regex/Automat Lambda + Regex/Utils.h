#pragma once

#include <string>

class Utils
{
public:
	Utils() = delete;

	static std::string GetNumberFromStateStr(const std::string& str);
	static int GetNumberFromStateInt(const std::string& str);
	static std::string GetLetterFromState(const std::string& str);

	static struct StateComparator
	{
		bool operator()(const std::string& state1, const std::string& state2) const;
	};
};
