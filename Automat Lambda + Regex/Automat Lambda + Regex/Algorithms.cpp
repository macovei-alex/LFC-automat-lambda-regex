#include "Algorithms.h"

#include <string>
#include <format>
#include <stack>

char Algorithms::concatOp = '.';

bool Algorithms::VerifyRegex(const std::string& regex)
{
	std::string reg = InsertConcatenationOperator(regex);
	int parenthesisCount = 0;

	for (size_t i = 0; i < reg.size(); i++)
	{
		char c = reg[i];

		if (c == '(')
		{
			parenthesisCount++;
		}

		else if (c == ')')
		{
			parenthesisCount--;
			if (parenthesisCount < 0)
				return false;
		}

		else if (!isalpha(c) && !IsOperator(c))
		{
			return false;
		}

		else if (c == '*')
		{
			if (i == 0)
				return false;

			else if (reg[i - 1] == '*')
				return false;

			else if (i >= 2 && reg[i - 2] == '(' && reg[i - 1] == ')')
				return false;
		}

		else if (c == concatOp || c == '|')
		{
			if (i == 0 || i == reg.size() - 1)
				return false;

			else if (reg[i - 1] == '(' || reg[i - 1] == concatOp || reg[i - 1] == '|')
				return false;

			else if (reg[i + 1] == ')' || IsOperator(reg[i + 1]))
				return false;
		}

		else if (isalpha(c))
		{
			if (i > 0 && isalpha(reg[i - 1]))
				return false;

			else if (i < reg.size() - 1 && isalpha(reg[i + 1]))
				return false;
		}
	}

	return parenthesisCount == 0;
}

DFA Algorithms::DFAfromRegex(const std::string& regex, bool doPrint, std::ostream& os)
{
	return DFAfromLFA(LFAfromRegex(regex, doPrint, os));
}

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
		std::string currentState{ std::format("p{}", currentStateIndex) };

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

	dfa.SetQ({});
	for (const auto& pair : Q)
		dfa.InsertIntoQ(std::format("q{}", Utils::GetNumberFromStateStr(pair.first)));

	dfa.SetDelta({});
	for (const auto& pair : Delta)
	{
		std::string startingState = std::format("q{}", Utils::GetNumberFromStateStr(pair.first.state));
		std::string destinationState = std::format("q{}", Utils::GetNumberFromStateStr(pair.second));
		Transition tranzitie{ startingState, pair.first.symbol };
		dfa.InsertIntoDelta(tranzitie, destinationState);
	}

	std::set<std::string, Utils::StateComparator> newFinalStates;
	for (const auto& newState : Q)
		for (const auto& componentState : newState.second)
			if (componentState == lfa.GetF())
			{
				newFinalStates.insert(std::format("q{}", Utils::GetNumberFromStateStr(newState.first)));
				break;
			}
	dfa.SetF(newFinalStates);

	dfa.SetQ0("q0");
	dfa.SetSigma(lfa.GetSigma());

	return dfa;
}

LFA Algorithms::LFAfromRegex(const std::string& regex, bool doPrint, std::ostream& os)
{
	std::string postfix = PolishPostfixFromRegex(regex);
	std::stack<LFA*> stack;

	if (doPrint)
		os << "BEGIN ALGORITHM\n\n";

	for (char c : postfix)
	{
		if (doPrint)
			os << "BEGIN ITERATION\n";

		if (doPrint && !stack.empty())
		{
			os << "TOP =\n";
			stack.top()->PrintAutomatonDebugForm();

			if (stack.size() >= 2)
			{
				os << "NEXT =\n";
				LFA* temp = stack.top();
				stack.pop();
				stack.top()->PrintAutomatonDebugForm();
				stack.push(temp);
			}
		}

		if (!IsOperator(c))
		{
			LFA* newLFA = new LFA(c);
			stack.push(newLFA);

			if (doPrint)
			{
				os << std::format("Character = [{}]\nNEW =\n", c);
				newLFA->PrintAutomatonDebugForm();
				os << "END ITERATION\n\n";
			}

			continue;
		}

		LFA* operand1 = stack.top();
		stack.pop();
		LFA* operand2;

		if (c == concatOp)
		{
			operand2 = stack.top();
			stack.pop();
			*operand2 += *operand1;
			stack.push(operand2);
			delete operand1;
		}
		else if (c == '|')
		{
			operand2 = stack.top();
			stack.pop();
			*operand2 |= *operand1;
			stack.push(operand2);
			delete operand1;
		}
		else if (c == '*')
		{
			*(*operand1);
			stack.push(operand1);
		}

		if (doPrint)
		{
			os << std::format("Character = [{}]\nNEW =\n", c);
			stack.top()->PrintAutomatonDebugForm();
			os << "END ITERATION\n\n";
		}
	}

	if (doPrint)
		os << "END ALGORITHM\n\n";

	return *stack.top();
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
			operatorStack.pop();
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

		if (c == concatOp)
		{
			stack.pop();
			stack.push(std::format("{}{}{}", operand2, concatOp, operand1));
		}
		else if (c == '|')
		{
			stack.pop();
			stack.push(std::format("{}|{}", operand2, operand1));
		}
		else if (c == '*')
		{
			stack.push(std::format("({})*", operand1));
		}
	}

	return stack.top();
}

std::string Algorithms::InsertConcatenationOperator(const std::string& regex)
{
	std::string newRegex = RemoveConcatenationOperator(regex);
	std::string ret;
	ret.reserve(regex.size() * 2);

	for (size_t i = 0; i < newRegex.size() - 1; i++)
	{
		ret += newRegex[i];

		if (newRegex[i] == '(' || newRegex[i] == '|')
			continue;

		if (newRegex[i + 1] == '*' || newRegex[i + 1] == ')' || newRegex[i + 1] == '|')
			continue;

		ret += concatOp;
	}

	ret += newRegex.back();

	return ret;
}

std::string Algorithms::RemoveConcatenationOperator(const std::string& regex)
{
	std::string newRegex;
	newRegex.reserve(regex.size());

	for (size_t i = 0; i < regex.size(); i++)
		if (regex[i] != concatOp)
			newRegex += regex[i];

	return newRegex;
}

bool Algorithms::IsOperator(char c)
{
	return c == '|' || c == '*' || c == concatOp;
}

int Algorithms::GetPrecedence(char c)
{
	if (c == '|') return 1;
	if (c == concatOp) return 2;
	if (c == '*') return 3;
	return 0;
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