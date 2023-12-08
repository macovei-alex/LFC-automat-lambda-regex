#include "Algorithms.h"

#include <string>
#include <format>

DFA Algorithms::DFAfromLFA(const LFA& lfa)
{
	DFA dfa;
	dfa.SetQ0("p0");
	dfa.SetSigma(lfa.GetSigma());

	std::unordered_map<Transition, std::string, Transition::Hash> newDelta;
	std::unordered_map<std::string, std::vector<std::string>> newStates;
	newStates["p0"] = lfa.LambdaEnclosing(lfa.GetQ0());

	int currentStateIndex = 0;

	while (currentStateIndex < newStates.size())
	{
		std::string currentState = std::format("p{}", currentStateIndex);

		for (char symbol : lfa.GetSigma())
		{
			std::vector<std::string> lambdaEnclosings = DetermineNewStates(lfa, newStates[currentState], symbol);
			Transition currentTransition{ currentState, symbol };

			bool stateAlreadyExists = false;
			for (const auto& existingState : newStates)
				if (existingState.second == lambdaEnclosings)
				{
					stateAlreadyExists = true;
					newDelta[currentTransition] = existingState.first;
					break;
				}

			if (!stateAlreadyExists)
			{
				std::string newState = std::format("p{}", newStates.size());
				newStates[newState] = lambdaEnclosings;
				newDelta[currentTransition] = newState;
			}
		}

		currentStateIndex++;
	}

	const char q = 'q';

	dfa.SetQ({});
	for (const auto& pair : newStates)
		dfa.InsertIntoQ(std::format("{}{}", q, pair.first[1]));

	dfa.SetDelta({});
	for (const auto& pair : newDelta)
	{
		std::string startingState = std::format("{}{}", q, pair.first.state[1]);
		std::string destinationState = std::format("{}{}", q, pair.second[1]);
		Transition tranzitie{ startingState, pair.first.symbol };
		dfa.InsertIntoDelta(tranzitie, destinationState);
	}

	std::unordered_set<std::string> newFinalStates;
	for (const auto& newstate : newStates)
		for (const auto& componentState : newstate.second)
			if (componentState == lfa.GetF())
			{
				newFinalStates.insert(std::format("{}{}", q, newstate.first[1]));
				break;
			}
	dfa.SetF(newFinalStates);

	return dfa;
}

std::vector<std::string> Algorithms::DetermineNewStates(const LFA& lfa, const std::vector<std::string>& newStateComponents, char symbol)
{
	std::unordered_set<std::string> newStates;
	for (const auto& componentState : newStateComponents)
	{
		Transition transition{ componentState, symbol };
		const auto& it = lfa.GetDelta().find(transition);
		if (it != lfa.GetDelta().end())
			newStates.insert(it->second.begin(), it->second.end());
	}

	if (newStates.empty())
		return {};

	std::unordered_set<std::string> lambdaEnclosings;
	for (const auto& state : newStates)
	{
		const auto& singleEnclosing = lfa.LambdaEnclosing(state);
		lambdaEnclosings.insert(singleEnclosing.begin(), singleEnclosing.end());
	}

	return { lambdaEnclosings.begin(), lambdaEnclosings.end() };
}