#include "DeterministicFiniteAutomaton.h"

#include <fstream>
#include <format>
#include <algorithm>

bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return false;

	if (Q.find(q0) == Q.end())
		return false;

	for (const auto& state : F)
		if (Q.find(state) == Q.end())
			return false;

	for (const auto& transitionProduction : Delta)
	{
		const Transition& transition = transitionProduction.first;
		if (Q.find(transition.state) == Q.end())
			return false;
		if (Q.find(transitionProduction.second) == Q.end())
			return false;
		if (Sigma.find(transition.symbol) == Sigma.end())
			return false;
	}

	bool transitionWithS = false;
	for (const char& caracter : Sigma)
		if (Delta.find({ q0, caracter }) != Delta.end())
		{
			transitionWithS = true;
			break;
		}
	if (!transitionWithS)
		return false;

	return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton(std::ostream& os) const
{
	os << "Q:\t";
	for (const auto& state : Q)
		os << state << ' ';
	os << '\n';

	os << "Sigma:\t";
	for (const auto& symbol : Sigma)
		os << symbol << ' ';
	os << '\n';

	os << "Delta:";
	for (const auto& transition : Delta)
		os << std::format("\t({}, {}) -> {}\n", transition.first.state, transition.first.symbol, transition.second);

	os << "q0:\t" << q0 << '\n';

	os << "F:\t";
	for (const auto& state : F)
		os << state << ' ';
	os << '\n';
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const
{
	std::string wordCopy{ word };
	std::string currentState{ q0 };
	while (!wordCopy.empty())
	{
		char symbol = wordCopy[0];
		wordCopy.erase(0, 1);
		if (Sigma.find(symbol) == Sigma.end())
			return false;
		auto found = Delta.find({ currentState, symbol });
		if (found == Delta.end())
			return false;
		currentState = found->second;
	}
	if (F.find(currentState) == F.end())
		return false;

	return true;
}

void DeterministicFiniteAutomaton::ReadAutomaton(std::istream& is)
{
	int n;
	is >> n;

	for (int i = 0; i < n; i++)
	{
		std::string state;
		is >> state;
		Q.insert(state);
	}

	is >> n;

	for (int i = 0; i < n; i++)
	{
		char symbol;
		is >> symbol;
		Sigma.insert(symbol);
	}

	is >> n;
	for (int i = 0; i < n; i++)
	{
		Transition transition;
		is >> transition.state >> transition.symbol;
		std::string newState;
		is >> newState;
		Delta[transition] = newState;
	}

	is >> q0;

	is >> n;

	for (size_t i = 0; i < n; i++)
	{
		std::string state;
		is >> state;
		F.insert(state);
	}
}

void DeterministicFiniteAutomaton::ReadAutomaton(const std::string& fileName)
{
	std::ifstream fin(fileName);
	ReadAutomaton(fin);
}

DeterministicFiniteAutomaton::operator bool() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return false;
	return true;
}

std::ostream& operator<<(std::ostream& os, const DeterministicFiniteAutomaton& automaton)
{
	automaton.PrintAutomaton(os);
	return os;
}

std::istream& operator>>(std::istream& is, DeterministicFiniteAutomaton& automaton)
{
	automaton.ReadAutomaton(is);
	return is;
}

// Obtinerea multimii starilor (Q)
const std::set<std::string, Utils::StateComparator>& DeterministicFiniteAutomaton::GetQ() const
{
	return this->Q;
}

void DeterministicFiniteAutomaton::SetQ(const std::set<std::string, Utils::StateComparator>& Q)
{
	this->Q = Q;
}

void DeterministicFiniteAutomaton::InsertIntoQ(const std::string& state)
{
	this->Q.insert(state);
}

const std::set<char>& DeterministicFiniteAutomaton::GetSigma() const
{
	return this->Sigma;
}

void DeterministicFiniteAutomaton::SetSigma(const std::set<char>& Sigma)
{
	this->Sigma = Sigma;
}

void DeterministicFiniteAutomaton::InsertIntoSigma(char symbol)
{
	this->Sigma.insert(symbol);
}

const std::map<Transition, std::string>& DeterministicFiniteAutomaton::GetDelta() const
{
	return this->Delta;
}

void DeterministicFiniteAutomaton::SetDelta(const std::map<Transition, std::string>& Delta)
{
	this->Delta = Delta;
}

void DeterministicFiniteAutomaton::InsertIntoDelta(const Transition& transition, const std::string& state)
{
	this->Delta[transition] = state;
}

const std::string& DeterministicFiniteAutomaton::GetQ0() const
{
	return this->q0;
}

void DeterministicFiniteAutomaton::SetQ0(const std::string& q0)
{
	this->q0 = q0;
}

const std::set<std::string, Utils::StateComparator>& DeterministicFiniteAutomaton::GetF() const
{
	return this->F;
}

void DeterministicFiniteAutomaton::SetF(const std::set<std::string, Utils::StateComparator>& F)
{
	this->F = F;
}
