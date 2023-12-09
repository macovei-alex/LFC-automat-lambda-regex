#pragma once

#include "DeterministicFiniteAutomaton.h"
#include "LambdaFiniteAutomaton.h"
#include "Utils.h"

using DFA = class DeterministicFiniteAutomaton;
using LFA = class LambdaFiniteAutomaton;

class Algorithms
{
public:
	Algorithms() = delete;

	static DFA DFAfromLFA(const LFA& lfa, const bool print = false);
	static std::vector<std::string> DetermineNewStates(const LFA& lfa, const std::vector<std::string>& newStatesComponents, char symbol);
};
