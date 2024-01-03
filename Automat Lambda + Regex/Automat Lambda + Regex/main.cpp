#include <iostream>
#include <fstream>
#include <format>

#include "LambdaFiniteAutomaton.h"
#include "DeterministicFiniteAutomaton.h"
#include "Algorithms.h"

using LFA = class LambdaFiniteAutomaton;
using DFA = class DeterministicFiniteAutomaton;

enum MenuOption
{
	PRINT_DFA = 1,
	PRINT_REGEX,
	CHECK_WORD
};

void PrintMenu()
{
	std::cout << "Optiuni:\n";
	std::cout << '\t' << MenuOption::PRINT_DFA << ". Afiseaza automatul in fluxul stdout si in fisier\n";
	std::cout << '\t' << MenuOption::PRINT_REGEX << ". Afiseaza expresia regulata in fluxul stdout\n";
	std::cout << '\t' << MenuOption::CHECK_WORD << ". Verifica daca un cuvant apartine limbajului\n";
	std::cout << "Introduceti optiunea: ";
}

void TestRegexValidation(std::ifstream& testIn)
{
	int counter = 1;
	while(!testIn.eof())
	{
		std::string regex;
		int rightAnswer;

		testIn >> regex;
		testIn >> rightAnswer;

		if (Algorithms::VerifyRegex(regex) != rightAnswer)
		{
			std::cout << std::format("Testul {} nu a trecut\n", counter);
			Algorithms::VerifyRegex(regex);
		}

		counter++;
	}
}

int main()
{
	std::ifstream testIn("regexTest.in");
	TestRegexValidation(testIn);

	std::ifstream fin("regex.in");
	std::ofstream fout("DFA.out");
	std::string regex;
	int option;

	fin >> regex;
	if(Algorithms::VerifyRegex(regex))
		std::cout << "Expresia este valida\n\n";
	else
	{
		std::cout << "Expresia nu este valida\n";
		return 0;
	}

	if (!Algorithms::VerifyRegex(regex))
	{
		std::cout << "Expresia nu este valida\n";
		return 0;
	}

	DFA dfa = Algorithms::DFAfromRegex(regex);

	while (true)
	{
		PrintMenu();
		std::cin >> option;

		switch (option)
		{

		case MenuOption::PRINT_DFA:
			std::cout << dfa << "\n\n";
			fout << dfa;
			fout.close();
			fout.open("DFA.out");
			break;

		case MenuOption::PRINT_REGEX:
			std::cout << regex << "\n\n";
			break;

		case MenuOption::CHECK_WORD:
			std::string word;
			std::cout << "Introduceti cuvantul: ";
			std::cin >> word;
			std::cout << (dfa.CheckWord(word) ? "Cuvantul apartine limbajului\n\n" : "Cuvantul nu apartine limbajului\n\n");
			break;
		}
	}

	return 0;
}