#include "Utils.h"

#include <regex>

std::string Utils::GetNumberFromStateStr(const std::string& state)
{
	if(!regex_match(state, std::regex("([a-z]|[A-Z])([0-9])+")))
		return "-1";

	return state.substr(1, state.size() - 1);
}

int Utils::GetNumberFromStateInt(const std::string& state)
{
	return atoi(state.substr(1, state.size() - 1).c_str());
}

std::string Utils::GetLetterFromState(const std::string& state)
{
	return state.substr(0, 1);
}
