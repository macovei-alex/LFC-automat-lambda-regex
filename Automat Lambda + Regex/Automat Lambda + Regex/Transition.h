#pragma once

#include <string>

struct Transition
{
	std::string state;
	char symbol;

	Transition& operator=(const Transition& other) = default;
	bool operator<(const Transition& other) const;
	bool operator==(const Transition& other) const = default;

	struct Hash
	{
		std::size_t operator()(const Transition& transition) const;
	};
};

