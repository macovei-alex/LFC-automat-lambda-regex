#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>

#include "Transition.h"
#include "Utils.h"

class LambdaFiniteAutomaton
{
public:
	bool VerifyAutomaton() const;
	void PrintAutomaton(std::ostream& os = std::cout) const;
	void ReadAutomaton(std::istream& is = std::cin);
	void ReadAutomaton(const std::string& fileName);
	bool operator!() const;
	std::vector<std::string> LambdaEnclosing(const std::string& str) const;
	std::set<std::string> LambdaEnclosings(const std::set<std::string>& states) const;

public:
	LambdaFiniteAutomaton(char c);
	void Concatenate(LambdaFiniteAutomaton& other);
	void Alternate(LambdaFiniteAutomaton& other);
	void KleeneStar();
	void OffsetStatesIndeces(const size_t offest);

	LambdaFiniteAutomaton& operator+=(LambdaFiniteAutomaton& other);
	LambdaFiniteAutomaton& operator|=(LambdaFiniteAutomaton& other);
	LambdaFiniteAutomaton& operator*();

public:
	LambdaFiniteAutomaton() = default;
	LambdaFiniteAutomaton(const LambdaFiniteAutomaton& other) = default;
	LambdaFiniteAutomaton& operator=(const LambdaFiniteAutomaton& other) = default;

public:
	const std::set<std::string, Utils::StateComparator>& GetQ() const;
	void SetQ(const std::set<std::string, Utils::StateComparator>& Q);
	void InsertIntoQ(const std::string& str);

	const std::set<char>& GetSigma() const;
	void SetSigma(const std::set<char>& Sigma);
	void InsertIntoSigma(char symbol);

	const std::map<Transition, std::vector<std::string>>& GetDelta() const;
	void SetDelta(const std::map<Transition, std::vector<std::string>>& Delta);
	void InsertIntoDelta(const Transition& transition, const std::vector<std::string>& states);
	void InsertIntoDelta(const Transition& transition, const std::string& str);

	const std::string& GetQ0() const;
	void SetQ0(const std::string& q0);

	const std::string& GetF() const;
	void SetF(const std::string& F);

private:
	std::set<std::string, Utils::StateComparator> Q;
	std::set<char> Sigma;
	std::map<Transition, std::vector<std::string>> Delta;
	std::string q0;
	std::string F;
};

std::ostream& operator<<(std::ostream& os, const LambdaFiniteAutomaton& automaton);
std::istream& operator>>(std::istream& is, LambdaFiniteAutomaton& automaton);
