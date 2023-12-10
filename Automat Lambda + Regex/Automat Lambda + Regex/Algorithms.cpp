#include "Algorithms.h"

#include <string>
#include <format>
#include <stack>

DFA Algorithms::DFAfromLFA(const LFA& lfa, const bool doPrint)
{
	DFA dfa;

	std::map<Transition, std::string> Delta;
	std::map<std::string, std::vector<std::string>> Q;
	Q["p0"] = lfa.LambdaEnclosing(lfa.GetQ0());

	if (doPrint)
	{
		std::cout << "p0 = { ";
		for (const auto& state : Q["p0"])
			std::cout << state << ' ';
		std::cout << "}\n";
	}

	int currentStateIndex = 0;

	while (currentStateIndex < Q.size())
	{
		std::string currentState = std::format("p{}", currentStateIndex);

		for (char symbol : lfa.GetSigma())
		{
			std::vector<std::string> lambdaEnclosings = DetermineNewStates(lfa, Q[currentState], symbol);

			if (doPrint)
			{
				std::cout << std::format("({}, {}) -> ", currentState, symbol) << "{ ";
				for (const auto& state : lambdaEnclosings)
					std::cout << state << ' ';
				std::cout << "} ";
			}

			if (lambdaEnclosings.empty())
			{
				if (doPrint)
					std::cout << '\n';

				continue;
			}

			Transition currentTransition{ currentState, symbol };

			bool stateAlreadyExists = false;
			for (const auto& existingState : Q)
				if (existingState.second == lambdaEnclosings)
				{
					stateAlreadyExists = true;
					Delta[currentTransition] = existingState.first;

					if (doPrint)
						std::cout << std::format("= {}\n", existingState.first);

					break;
				}

			if (!stateAlreadyExists)
			{
				std::string newState = std::format("p{}", Q.size());
				Q[newState] = std::move(lambdaEnclosings);
				Delta[currentTransition] = newState;

				if (doPrint)
					std::cout << std::format("= {}\n", newState);
			}
		}

		currentStateIndex++;
	}

	const char q = 'q';

	dfa.SetQ({});
	for (const auto& pair : Q)
		dfa.InsertIntoQ(std::format("{}{}", q, Utils::GetNumberFromState(pair.first)));

	dfa.SetDelta({});
	for (const auto& pair : Delta)
	{
		std::string startingState = std::format("{}{}", q, Utils::GetNumberFromState(pair.first.state));
		std::string destinationState = std::format("{}{}", q, Utils::GetNumberFromState(pair.second));
		Transition tranzitie{ startingState, pair.first.symbol };
		dfa.InsertIntoDelta(tranzitie, destinationState);
	}

	std::set<std::string> newFinalStates;
	for (const auto& newState : Q)
		for (const auto& componentState : newState.second)
			if (componentState == lfa.GetF())
			{
				newFinalStates.insert(std::format("{}{}", q, Utils::GetNumberFromState(newState.first)));
				break;
			}
	dfa.SetF(newFinalStates);

	dfa.SetQ0("q0");
	dfa.SetSigma(lfa.GetSigma());

	return dfa;
}

std::vector<std::string> Algorithms::DetermineNewStates(const LFA& lfa, const std::vector<std::string>& newStateComponents, char symbol)
{
	std::set<std::string> newStates;
	for (const auto& componentState : newStateComponents)
	{
		Transition transition{ componentState, symbol };
		const auto& it = lfa.GetDelta().find(transition);
		if (it != lfa.GetDelta().end())
			newStates.insert(it->second.begin(), it->second.end());
	}

	std::set<std::string> lambdaEnclosings = lfa.LambdaEnclosings(newStates);

	return { lambdaEnclosings.begin(), lambdaEnclosings.end() };
}

std::string Algorithms::PolishPostfixFromRegex(const std::string& regex)
{
	std::stack<char> operatorStack;
	std::string postfix;
	std::string newRegex = InsertConcatenationOperator(regex);

	postfix.reserve(newRegex.size());

	for (char c : newRegex)
	{
		if (isalnum(c))
			postfix += c;

		else if (c == '(')
			operatorStack.push(c);

		else if (c == ')')
		{
			while (!operatorStack.empty() && operatorStack.top() != '(')
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.pop(); // Pop the '('
		}

		else if (IsOperator(c))
		{
			while (!operatorStack.empty() && GetPrecedence(operatorStack.top()) >= GetPrecedence(c))
			{
				postfix += operatorStack.top();
				operatorStack.pop();
			}
			operatorStack.push(c);
		}
	}

	while (!operatorStack.empty())
	{
		postfix += operatorStack.top();
		operatorStack.pop();
	}

	return postfix;
}

std::string Algorithms::RegexFromPolishPostfix(const std::string& postfix)
{
	std::stack<std::string> stack;

	for (char c : postfix)
	{
		if (!IsOperator(c))
		{
			stack.push(std::string(1, c));
			continue;
		}

		std::string operand1 = stack.top();
		stack.pop();
		std::string operand2 = stack.top();

		switch (c)
		{

		case '.':
			stack.pop();
			stack.push(std::format("{}.{}", operand2, operand1));
			break;

		case '|':
			stack.pop();
			stack.push(std::format("{}|{}", operand2, operand1));
			break;

		case '*':
			stack.push(std::format("({})*", operand1));
			break;

		}
	}

	return stack.top();
}

std::string Algorithms::InsertConcatenationOperator(const std::string& regex)
{
	std::string newRegex;
	newRegex.reserve(regex.size() * 2);
	for (size_t i = 0; i < regex.size() - 1; i++)
	{
		newRegex += regex[i];

		if (regex[i] == '(' || regex[i] == '|')
			continue;

		if (regex[i + 1] == '*' || regex[i + 1] == ')' || regex[i + 1] == '|')
			continue;

		newRegex += '.';
	}
	newRegex += regex.back();
	return newRegex;
}

std::string Algorithms::RemoveConcatenationOperator(const std::string& regex)
{
	std::string newRegex;
	newRegex.reserve(regex.size());
	for (size_t i = 0; i < regex.size(); i++)
	{
		if (regex[i] == '.')
			continue;
		newRegex += regex[i];
	}
	return newRegex;
}

bool Algorithms::IsOperator(char c)
{
	return c == '|' || c == '*' || c == '.';
}

int Algorithms::GetPrecedence(char c)
{
	if (c == '|') return 1;
	if (c == '.') return 2;
	if (c == '*') return 3;
	return 0;
}