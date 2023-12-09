#include "Algorithms.h"

#include <string>
#include <format>

DFA Algorithms::DFAfromLFA(const LFA& lfa, const bool print)
{
	DFA dfa;

	std::map<Transition, std::string> Delta;
	std::map<std::string, std::vector<std::string>> Q;
	Q["p0"] = lfa.LambdaEnclosing(lfa.GetQ0());

	if (print)
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

			if (print)
			{
				std::cout << std::format("({}, {}) -> ", currentState, symbol) << "{ ";
				for (const auto& state : lambdaEnclosings)
					std::cout << state << ' ';
				std::cout << "} ";
			}

			if (lambdaEnclosings.empty())
			{
				if (print)
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

					if (print)
						std::cout << std::format("= {}\n", existingState.first);

					break;
				}

			if (!stateAlreadyExists)
			{
				std::string newState = std::format("p{}", Q.size());
				Q[newState] = std::move(lambdaEnclosings);
				Delta[currentTransition] = newState;

				if (print)
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