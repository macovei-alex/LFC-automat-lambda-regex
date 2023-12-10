#include "Transition.h"

#include "Utils.h"

// Suprascrierea operatorului '<' pentru a permite sortarea obiectelor de tip Transition
bool Transition::operator<(const Transition& other) const
{
	if(state == other.state)
		return symbol < other.symbol;
	if(Utils::GetLetterFromState == Utils::GetLetterFromState)
		return Utils::GetNumberFromStateInt(this->state) < Utils::GetNumberFromStateInt(other.state);
	return this->state < other.state;
}