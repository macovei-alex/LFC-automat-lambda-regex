#pragma once

#include <string>

struct Transition
{
	std::string state;
	char symbol;

	Transition() = default;
	Transition(const std::string& state, char symbol) : state(state), symbol(symbol) {}
	Transition(const Transition& other) = default;
	Transition& operator=(const Transition& other) = default;

	bool operator<(const Transition& other) const;
	bool operator==(const Transition& other) const = default;
};
