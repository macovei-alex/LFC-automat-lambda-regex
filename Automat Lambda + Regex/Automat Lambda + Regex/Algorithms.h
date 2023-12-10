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

	static DFA DFAfromLFA(const LFA& lfa, const bool doPrint = false);
	static std::vector<std::string> DetermineNewStates(const LFA& lfa, const std::vector<std::string>& newStatesComponents, char symbol);

	static std::string PolishPostfixFromRegex(const std::string& regex, const char concatOp = '.');
	static std::string RegexFromPolishPostfix(const std::string& postfix, const char concatOp = '.');
	static std::string InsertConcatenationOperator(const std::string& regex, const char concatOp = '.');
	static std::string RemoveConcatenationOperator(const std::string& regex, const char concatOp = '.');

private:
	static inline bool IsOperator(char c, const char concatOp = '+');
	static int GetPrecedence(char c, const char concatOp = '+');
};
