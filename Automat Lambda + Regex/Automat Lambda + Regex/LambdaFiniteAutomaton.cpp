#include "LambdaFiniteAutomaton.h"

#include <fstream>
#include <format>
#include <stack>

const char LAMBDA = '\'';

// Validarea corectitudinii automatului
bool LambdaFiniteAutomaton::VerifyAutomaton() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return false;

	// Verifica daca F este inclus în Q
	if (Q.find(F) == Q.end())
		return false;

	// Verifica daca q0 este inclus în Q
	if (Q.find(q0) == Q.end())
		return false;

	// Verifica daca tranzitiile sunt valide
	for (const auto& transitionList : Delta)
	{
		const Transition& transition = transitionList.first;
		if (Q.find(transition.state) == Q.end())
			return false;
		if (Sigma.find(transition.symbol) == Sigma.end())
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
		for (const auto& stare : transition.second)
			os << std::format("\t({}, {}) -> {}\n", transition.first.state, transition.first.symbol, stare);

	os << "q0:\t" << q0 << '\n';

	os << "F:\t" << F << '\n';
}

// Validarea unui cuvant pentru orice tip de automat (AFD sau AFN)
bool LambdaFiniteAutomaton::CheckWord(const std::string& word) const
{
	return true;
	// TODO
}

// Validarea unui cuvant pentru un automat finit nedeterminist (AFN)
bool LambdaFiniteAutomaton::CheckWordNFA(std::string word) const
{
	// Starile posibile curente,incepand cu starea initiala
	std::vector<std::string> possibleStates{ q0 };

	// Parcurgem cuvantul si starile posibile curente pana cand cuvantul este vid sau nu mai exista stari posibile
	while (!word.empty() && !possibleStates.empty())
	{
		// Starile posibile noi pentru urmatoarea litera din cuvant
		std::vector<std::string> newPossibleStates;

		// Iteram prin fiecare stare posibila curenta
		for (const auto& state : possibleStates)
		{
			// Gasim tranzitia pentru perechea (stare curenta, caracter curent)
			auto iterator = Delta.find({ state, word[0] });
			// Daca tranzitia exista, adaugam noile stari posibile
			if (iterator != Delta.end())
				for (const auto& newState : iterator->second)
					newPossibleStates.push_back(newState);
		}
		// Actualizam starile posibile curente cu noile stari posibile
		possibleStates = std::move(newPossibleStates);
		// Stergem prima litera din cuvant
		word.erase(0, 1);
	}

	// Daca cuvantul nu a fost sters complet, acesta nu este acceptat
	if (word.size() != 0)
		return false;

	// Verificam daca exista cel putin o stare posibila curenta care este stare finala
	for (const auto& state : possibleStates)
		if (state == F)
			return true;

	// Daca nu exista nicio stare finala in starile posibile curente, cuvantul nu este acceptat
	return false;
}

// Citirea automatului dintr-un fisier
void LambdaFiniteAutomaton::ReadAutomaton(std::istream& is)
{
	int n;
	is >> n;

	// Citirea starilor (Q)
	for (int i = 0; i < n; i++)
	{
		std::string state;
		is >> state;
		Q.insert(state);
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

bool LambdaFiniteAutomaton::operator!() const
{
	if (Q.empty() || Sigma.empty() || q0.empty() || F.empty() || Delta.empty())
		return true;
	return false;
}

std::vector<std::string> LambdaFiniteAutomaton::LambdaEnclosing(const std::string& state) const
{
	// DFS
	std::unordered_map<std::string, bool> visited;
	std::vector<std::string> enclosing;
	std::stack<std::string> statesStack;

	visited[state] = true;
	statesStack.push(state);
	do
	{

	} while (!enclosing.empty());
	return {};
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
const std::unordered_set<std::string>& LambdaFiniteAutomaton::GetQ() const
{
	return this->Q;
}

void LambdaFiniteAutomaton::SetQ(const std::unordered_set<std::string>& Q)
{
	this->Q = Q;
}

void LambdaFiniteAutomaton::InsertIntoQ(const std::string& state)
{
	this->Q.insert(state);
}

const std::unordered_set<char>& LambdaFiniteAutomaton::GetSigma() const
{
	return this->Sigma;
}

void LambdaFiniteAutomaton::SetSigma(const std::unordered_set<char>& Sigma)
{
	this->Sigma = Sigma;
}

void LambdaFiniteAutomaton::InsertIntoSigma(char symbol)
{
	this->Sigma.insert(symbol);
}

const std::unordered_map<Transition, std::vector<std::string>, Transition::Hash>& LambdaFiniteAutomaton::GetDelta() const
{
	return this->Delta;
}

void LambdaFiniteAutomaton::SetDelta(const std::unordered_map<Transition, std::vector<std::string>, Transition::Hash>& Delta)
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

// Suprascrierea operatorului '<' pentru a permite sortarea obiectelor de tip Transition
bool Transition::operator<(const Transition& other) const
{
	if (state != other.state)
		return state < other.state;
	return symbol < other.symbol;
}

std::size_t Transition::Hash::operator()(const Transition& transition) const
{
	return std::hash<std::string>()(transition.state + transition.symbol);
}