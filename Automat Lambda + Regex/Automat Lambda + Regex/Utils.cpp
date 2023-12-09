#include "Utils.h"

#include <regex>

std::string Utils::GetNumberFromState(const std::string& state)
{
	if(!regex_match(state, std::regex("([a-z]|[A-Z])([0-9])+")))
		return "-1";

	return state.substr(1, state.size() - 1);
}