#include <iostream>
#include <fstream>

#include "LambdaFiniteAutomaton.h"

void main()
{
	LambdaFiniteAutomaton automat;
	std::ifstream fin("automaton.in");
	automat.ReadAutomaton(fin);
	automat.PrintAutomaton();

	std::cout << std::boolalpha << automat.VerifyAutomaton() << '\n';

	for (const auto& state : automat.GetQ())
	{
		std::cout << state << ": ";
		for (const auto& enclosingState : automat.LambdaEnclosing(state))
			std::cout << enclosingState << ' ';
		std::cout << '\n';
	}
}