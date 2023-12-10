#include "LambdaFiniteAutomaton.h"

#include <fstream>
#include <format>
#include <stack>

const char LAMBDA = '$';

// Validarea corectitudinii automatului
bool LambdaFiniteAutomaton::VerifyAutomaton() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return false;

	// Verifica daca F si q0 sunt incluse în Q
	if (Q.find(F) == Q.end() || Q.find(q0) == Q.end())
		return false;

	// Verifica daca tranzitiile sunt valide
	for (const auto& transitionList : Delta)
	{
		const Transition& transition = transitionList.first;
		if (Q.find(transition.state) == Q.end())
			return false;
		if (transition.symbol != LAMBDA && Sigma.find(transition.symbol) == Sigma.end())
			return false;
		for (const auto& state : transitionList.second)
			if (Q.find(state) == Q.end())
				return false;
	}

	// Exista minim 1 tranzitie care contine starea intiaiala
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

void LambdaFiniteAutomaton::PrintAutomaton(std::ostream& os) const
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
		for (const auto& state : transition.second)
			os << std::format("\t({}, {}) -> {}\n", transition.first.state, transition.first.symbol, state);

	os << "q0:\t" << q0 << '\n';

	os << "F:\t" << F << '\n';
}

// Citirea automatului dintr-un fisier
void LambdaFiniteAutomaton::ReadAutomaton(std::istream& is)
{
	int n;
	is >> n;

	// Citirea starilor (Q)
	for (int i = 0; i < n; i++)
	{
		std::string str;
		is >> str;
		Q.insert(str);
	}

	is >> n;

	// Citirea alfabetului (Sigma)
	for (int i = 0; i < n; i++)
	{
		char symbol;
		is >> symbol;
		Sigma.insert(symbol);
	}

	// Citirea tranzitiilor (Delta)
	is >> n;
	for (int i = 0; i < n; i++)
	{
		Transition transition;
		is >> transition.state >> transition.symbol;
		std::string newState;
		is >> newState;
		Delta[transition].push_back(newState);
	}

	// Citirea starii initiale (q0)
	is >> q0;

	// Citirea starii finale (F)
	is >> F;
}

void LambdaFiniteAutomaton::ReadAutomaton(const std::string& fileName)
{
	std::ifstream fin(fileName);
	ReadAutomaton(fin);
}

bool LambdaFiniteAutomaton::operator!() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return true;
	return false;
}

std::vector<std::string> LambdaFiniteAutomaton::LambdaEnclosing(const std::string& state) const
{
	std::map<std::string, bool> visited;
	std::vector<std::string> enclosing;
	std::stack<std::string> statesStack;

	visited[state] = true;
	statesStack.push(state);
	do
	{
		std::string currentState = statesStack.top();
		statesStack.pop();
		enclosing.push_back(currentState);

		auto currentLambdaStates = Delta.find(Transition{ currentState, LAMBDA });
		if (currentLambdaStates == Delta.end())
			continue;

		for (const auto& newState : currentLambdaStates->second)
			if (!visited[newState])
			{
				visited[newState] = true;
				statesStack.push(newState);
			}

	} while (!statesStack.empty());
	return enclosing;
}

std::set<std::string> LambdaFiniteAutomaton::LambdaEnclosings(const std::set<std::string>& states) const
{
	std::set<std::string> allEnclosings;
	for (const auto& state : states)
	{
		const auto singleEnclosing = LambdaEnclosing(state);
		allEnclosings.insert(singleEnclosing.begin(), singleEnclosing.end());
	}
	return allEnclosings;
}

LambdaFiniteAutomaton::LambdaFiniteAutomaton(char c) :
	Q{ "q0", "q1" },
	Sigma{ c },
	Delta{ {{ "q0", c }, { "q1" }} },
	q0{ "q0" },
	F{ "q1" }
{
	/* empty */
}

void LambdaFiniteAutomaton::Concatenate(LambdaFiniteAutomaton& other)
{
	other.OffsetStatesIndeces(Q.size() - 1);

	// Toate tranzitiile in afara de cele cu starea initiala a lui other
	Delta.insert(++other.Delta.begin(), other.Delta.end());

	// Tranzitiile cu starea initiala a lui other
	for(const auto& symbol : other.Sigma)
		for(const auto& state : other.Delta[{other.q0, symbol}])
			Delta[{other.q0, symbol}].push_back(state);

	Q.insert(other.Q.begin(), other.Q.end());

	q0 = this->q0;
	F = other.F;
	Sigma.insert(other.Sigma.begin(), other.Sigma.end());
}

void LambdaFiniteAutomaton::Alternate(LambdaFiniteAutomaton& other)
{
	OffsetStatesIndeces(1);
	other.OffsetStatesIndeces(Q.size() + 1);

	std::string newBeginState{ "q0" };
	std::string newFinalState{ std::format("q{}", Utils::GetNumberFromStateInt(other.F) + 1)};

	Delta.insert(other.Delta.begin(), other.Delta.end());
	Delta[{newBeginState, LAMBDA}] = { *Q.begin(), *other.Q.begin() };
	Delta[{*Q.rbegin(), LAMBDA}] = { newFinalState };
	Delta[{*other.Q.rbegin(), LAMBDA}] = { newFinalState };

	Q.insert(other.Q.begin(), other.Q.end());
	Q.insert(newBeginState);
	Q.insert(newFinalState);

	q0 = newBeginState;
	F = newFinalState;
	Sigma.insert(other.Sigma.begin(), other.Sigma.end());
}

void LambdaFiniteAutomaton::KleeneStar()
{
	OffsetStatesIndeces(1);

	std::string newBeginState = "q0";
	std::string newFinalState = std::format("q{}", Utils::GetNumberFromStateInt(F) + 1);

	Delta[{newBeginState, LAMBDA}] = { *Q.begin(), newFinalState };
	Delta[{*Q.rbegin(), LAMBDA}].push_back(*Q.begin());
	Delta[{*Q.rbegin(), LAMBDA}].push_back(newFinalState);

	Q.insert(newBeginState);
	Q.insert(newFinalState);

	q0 = newBeginState;
	F = newFinalState;
	Sigma = this->Sigma;
}

void LambdaFiniteAutomaton::OffsetStatesIndeces(const size_t offest)
{
	std::set<std::string, Utils::StateComparator> tempQ;
	for (const auto& str : Q)
	{
		size_t newIndex = Utils::GetNumberFromStateInt(str) + offest;
		tempQ.insert(std::format("{}{}", Utils::GetLetterFromState(str), newIndex));
	}

	std::map<Transition, std::vector<std::string>> tempDelta;
	for (const auto& transition : Delta)
	{
		Transition tempTransition;
		size_t newIndex = Utils::GetNumberFromStateInt(transition.first.state) + offest;
		tempTransition.state = std::format("{}{}", Utils::GetLetterFromState(transition.first.state), newIndex);
		tempTransition.symbol = transition.first.symbol;

		std::vector<std::string> tempStates;
		for (const auto& state : transition.second)
		{
			size_t newIndex = Utils::GetNumberFromStateInt(state) + offest;
			tempStates.push_back(std::format("{}{}", Utils::GetLetterFromState(state), newIndex));
		}

		tempDelta[tempTransition] = tempStates;
	}

	size_t newIndex = Utils::GetNumberFromStateInt(q0) + offest;
	std::string tempQ0 = std::format("{}{}", Utils::GetLetterFromState(q0), newIndex);

	newIndex = Utils::GetNumberFromStateInt(F) + offest;
	std::string tempF = std::format("{}{}", Utils::GetLetterFromState(F), newIndex);

	Q = std::move(tempQ);
	Delta = std::move(tempDelta);
	q0 = std::move(tempQ0);
	F = std::move(tempF);
}

LambdaFiniteAutomaton& LambdaFiniteAutomaton::operator+=(LambdaFiniteAutomaton& other)
{
	Concatenate(other);
	return *this;
}

LambdaFiniteAutomaton& LambdaFiniteAutomaton::operator|=(LambdaFiniteAutomaton& other)
{
	Alternate(other);
	return *this;
}

LambdaFiniteAutomaton& LambdaFiniteAutomaton::operator*()
{
	KleeneStar();
	return *this;
}

std::ostream& operator<<(std::ostream& os, const LambdaFiniteAutomaton& automaton)
{
	automaton.PrintAutomaton(os);
	return os;
}

std::istream& operator>>(std::istream& is, LambdaFiniteAutomaton& automaton)
{
	automaton.ReadAutomaton(is);
	return is;
}

// Obtinerea multimii starilor (Q)
const std::set<std::string, Utils::StateComparator>& LambdaFiniteAutomaton::GetQ() const
{
	return this->Q;
}

void LambdaFiniteAutomaton::SetQ(const std::set<std::string, Utils::StateComparator>& Q)
{
	this->Q = Q;
}

void LambdaFiniteAutomaton::InsertIntoQ(const std::string& state)
{
	this->Q.insert(state);
}

const std::set<char>& LambdaFiniteAutomaton::GetSigma() const
{
	return this->Sigma;
}

void LambdaFiniteAutomaton::SetSigma(const std::set<char>& Sigma)
{
	this->Sigma = Sigma;
}

void LambdaFiniteAutomaton::InsertIntoSigma(char symbol)
{
	this->Sigma.insert(symbol);
}

const std::map<Transition, std::vector<std::string>>& LambdaFiniteAutomaton::GetDelta() const
{
	return this->Delta;
}

void LambdaFiniteAutomaton::SetDelta(const std::map<Transition, std::vector<std::string>>& Delta)
{
	this->Delta = Delta;
}

void LambdaFiniteAutomaton::InsertIntoDelta(const Transition& transition, const std::vector<std::string>& states)
{
	this->Delta[transition] = states;
}

void LambdaFiniteAutomaton::InsertIntoDelta(const Transition& transition, const std::string& state)
{
	this->Delta[transition].push_back(state);
}

const std::string& LambdaFiniteAutomaton::GetQ0() const
{
	return this->q0;
}

void LambdaFiniteAutomaton::SetQ0(const std::string& q0)
{
	this->q0 = q0;
}

const std::string& LambdaFiniteAutomaton::GetF() const
{
	return this->F;
}

void LambdaFiniteAutomaton::SetF(const std::string& F)
{
	this->F = F;
}