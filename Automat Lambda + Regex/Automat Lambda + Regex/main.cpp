#include <iostream>
#include <fstream>
#include <unordered_map>

#include "LambdaFiniteAutomaton.h"
#include "DeterministicFiniteAutomaton.h"
#include "Algorithms.h"

using LFA = class LambdaFiniteAutomaton;
using DFA = class DeterministicFiniteAutomaton;

int main()
{
	LFA lfa = Algorithms::LFAfromRegex("ab(ac|bc)*b(a)*");

	std::cout << lfa << '\n';

	std::cout << "Automat corect: " << std::boolalpha << lfa.VerifyAutomaton() << '\n';

	for (const auto& state : lfa.GetQ())
	{
		std::cout << state << ": ";
		for (const auto& enclosingState : lfa.LambdaEnclosing(state))
			std::cout << enclosingState << ' ';
		std::cout << '\n';
	}

	std::cout << "\n\n\n";

	DFA dfa = Algorithms::DFAfromLFA(lfa);

	std::cout << dfa << '\n';

	std::cout << "Automat corect: " << std::boolalpha << dfa.VerifyAutomaton() << '\n';

	std::cout << std::boolalpha << dfa.CheckWord("abbaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbaaaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abacbcba") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbc") << '\n';

	dfa = Algorithms::DFAfromLFA(lfa);
	std::cout << "\n\n";
	dfa.PrintAutomaton();

	std::cout << std::boolalpha << dfa.CheckWord("abbaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbaaaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abacbcba") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbc") << '\n';

	std::cout << Algorithms::InsertConcatenationOperator("aba(aa|bb)*c(ab)*") << '\n';
	std::cout << Algorithms::PolishPostfixFromRegex("aba(aa|bb)*c(ab)*") << '\n';
	std::cout << Algorithms::RemoveConcatenationOperator(
		Algorithms::RegexFromPolishPostfix(
			Algorithms::PolishPostfixFromRegex("aba(aa|bb)*c(ab)*"))) << '\n';

	return 0;
}