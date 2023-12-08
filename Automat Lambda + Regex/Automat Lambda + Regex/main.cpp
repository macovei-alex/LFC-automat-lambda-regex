#include <iostream>
#include <fstream>

#include "LambdaFiniteAutomaton.h"
#include "DeterministicFiniteAutomaton.h"

using LFA = class LambdaFiniteAutomaton;
using DFA = class DeterministicFiniteAutomaton;

void main()
{
	LFA lfa;

	lfa.ReadAutomaton("LFA.in");
	lfa.PrintAutomaton();

	std::cout << "Automat corect: " << std::boolalpha << lfa.VerifyAutomaton() << '\n';

	for (const auto& state : lfa.GetQ())
	{
		std::cout << state << ": ";
		for (const auto& enclosingState : lfa.LambdaEnclosing(state))
			std::cout << enclosingState << ' ';
		std::cout << '\n';
	}

	std::cout << "\n\n\n";

	DFA dfa;

	dfa.ReadAutomaton("DFA.in");
	dfa.PrintAutomaton();

	std::cout << "Automat corect: " << std::boolalpha << dfa.VerifyAutomaton() << '\n';
}