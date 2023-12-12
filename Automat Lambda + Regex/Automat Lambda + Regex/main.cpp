#include <iostream>
#include <fstream>

#include "LambdaFiniteAutomaton.h"
#include "DeterministicFiniteAutomaton.h"
#include "Algorithms.h"

using LFA = class LambdaFiniteAutomaton;
using DFA = class DeterministicFiniteAutomaton;

int main()
{
	std::ifstream fin("regex.in");
	std::string regex;
	fin >> regex;
	if (!Algorithms::VerifyRegex(regex))
	{
		std::cout << "Expresia nu este valida\n";
		return 0;
	}
	LFA lfa = Algorithms::LFAfromRegex(regex);

	std::cout << "LFA:\n" << lfa << '\n';
	std::cout << "Automat corect: " << std::boolalpha << lfa.VerifyAutomaton() << "\n\n";

	DFA dfa = Algorithms::DFAfromRegex(regex);

	std::cout << "DFA:\n" << dfa << '\n';
	std::cout << "Automat corect: " << std::boolalpha << dfa.VerifyAutomaton() << "\n\n";

	std::cout << std::boolalpha << dfa.CheckWord("abbaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbaaaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abacbcba") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbc") << "\n\n";

	dfa = Algorithms::DFAfromLFA(lfa);
	std::cout << "DFA:\n" << dfa << '\n';
	std::cout << "Automat corect: " << std::boolalpha << dfa.VerifyAutomaton() << "\n\n";

	std::cout << std::boolalpha << dfa.CheckWord("abbaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbaaaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abaa") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abacbcba") << '\n';
	std::cout << std::boolalpha << dfa.CheckWord("abbc") << '\n';

	return 0;
}