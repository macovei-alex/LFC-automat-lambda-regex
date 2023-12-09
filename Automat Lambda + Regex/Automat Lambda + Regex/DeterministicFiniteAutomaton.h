#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>

#include "Transition.h"

class DeterministicFiniteAutomaton
{
public:
	bool VerifyAutomaton() const;
	void PrintAutomaton(std::ostream& os = std::cout) const;
	bool CheckWord(const std::string& word) const;

public:
	void ReadAutomaton(std::istream& is = std::cin);
	void ReadAutomaton(const std::string& fileName);
	bool operator!() const;

public:
	DeterministicFiniteAutomaton() = default;
	DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other) = default;
	DeterministicFiniteAutomaton& operator=(const DeterministicFiniteAutomaton& other) = default;

public:
	const std::set<std::string>& GetQ() const;
	void SetQ(const std::set<std::string>& Q);
	void InsertIntoQ(const std::string& state);

	const std::set<char>& GetSigma() const;
	void SetSigma(const std::set<char>& Sigma);
	void InsertIntoSigma(char symbol);

	const std::map<Transition, std::string>& GetDelta() const;
	void SetDelta(const std::map<Transition, std::string>& Delta);
	void InsertIntoDelta(const Transition& transition, const std::string& state);

	const std::string& GetQ0() const;
	void SetQ0(const std::string& q0);

	const std::set<std::string>& GetF() const;
	void SetF(const std::set<std::string>& F);

private:
	std::set<std::string> Q;
	std::set<char> Sigma;
	std::map<Transition, std::string> Delta;
	std::string q0;
	std::set<std::string> F;
};

std::ostream& operator<<(std::ostream& os, const DeterministicFiniteAutomaton& automaton);
std::istream& operator>>(std::istream& is, DeterministicFiniteAutomaton& automaton);
