#include "Utils.h"

#include <regex>

std::string Utils::GetNumberFromStateStr(const std::string& str)
{
	if(!regex_match(str, std::regex("([a-z]|[A-Z])([0-9])+")))
		return "-1";

	return str.substr(1, str.size() - 1);
}

int Utils::GetNumberFromStateInt(const std::string& str)
{
	return atoi(str.substr(1, str.size() - 1).c_str());
}

std::string Utils::GetLetterFromState(const std::string& str)
{
	return str.substr(0, 1);
}

bool Utils::StateComparator::operator()(const std::string& state1, const std::string& state2) const
{
	if (Utils::GetLetterFromState(state1) == Utils::GetLetterFromState(state2))
		return Utils::GetNumberFromStateInt(state1) < Utils::GetNumberFromStateInt(state2);
	return state1 < state2;
}