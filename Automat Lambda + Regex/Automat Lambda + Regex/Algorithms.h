#pragma once

#include "DeterministicFiniteAutomaton.h"
#include "LambdaFiniteAutomaton.h"
#include "Utils.h"

using DFA = class DeterministicFiniteAutomaton;
using LFA = class LambdaFiniteAutomaton;

class Algorithms
{
	Algorithms() = delete;

public:
	static bool VerifyRegex(const std::string& regex);
	static DFA DFAfromRegex(const std::string& regex, bool doPrint = false, std::ostream& os = std::cout);

public:
	static char concatOp;

	static DFA DFAfromLFA(const LFA& lfa, const bool doPrint = false);
	static LFA LFAfromRegex(const std::string& regex, bool doPrint = false, std::ostream& os = std::cout);
	static std::string PolishPostfixFromRegex(const std::string& regex);
	static std::string RegexFromPolishPostfix(const std::string& postfix);
	static std::string InsertConcatenationOperator(const std::string& regex);
	static std::string RemoveConcatenationOperator(const std::string& regex);

private:
	static inline bool IsOperator(char c);
	static int GetPrecedence(char c);
	static std::vector<std::string> DetermineNewStates(const LFA& lfa, const std::vector<std::string>& newStatesComponents, char symbol);
};
