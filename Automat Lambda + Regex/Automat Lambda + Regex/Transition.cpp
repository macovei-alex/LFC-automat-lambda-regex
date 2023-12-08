#include "Transition.h"

// Suprascrierea operatorului '<' pentru a permite sortarea obiectelor de tip Transition
bool Transition::operator<(const Transition& other) const
{
	if (state != other.state)
		return state < other.state;
	return symbol < other.symbol;
}

std::size_t Transition::Hash::operator()(const Transition& transition) const
{
	return std::hash<std::string>()(transition.state + transition.symbol);
}