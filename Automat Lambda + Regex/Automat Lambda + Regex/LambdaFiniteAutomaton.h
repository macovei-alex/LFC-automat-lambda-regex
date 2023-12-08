#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>

#include "Transition.h"

class LambdaFiniteAutomaton
{
public:
	bool VerifyAutomaton() const;
	void PrintAutomaton(std::ostream& os = std::cout) const;

public:
	void ReadAutomaton(std::istream& is = std::cin);
	void ReadAutomaton(const std::string& fileName);
	bool operator!() const;
	std::vector<std::string> LambdaEnclosing(const std::string& state) const;

public:
	LambdaFiniteAutomaton() = default;
	LambdaFiniteAutomaton(const LambdaFiniteAutomaton& other) = default;
	LambdaFiniteAutomaton& operator=(const LambdaFiniteAutomaton& other) = default;

public:
	const std::unordered_set<std::string>& GetQ() const;
	void SetQ(const std::unordered_set<std::string>& Q);
	void InsertIntoQ(const std::string& state);

	const std::unordered_set<char>& GetSigma() const;
	void SetSigma(const std::unordered_set<char>& Sigma);
	void InsertIntoSigma(char symbol);

	const std::unordered_map<Transition, std::vector<std::string>, Transition::Hash>& GetDelta() const;
	void SetDelta(const std::unordered_map<Transition, std::vector<std::string>, Transition::Hash>& Delta);
	void InsertIntoDelta(const Transition& transition, const std::vector<std::string>& states);
	void InsertIntoDelta(const Transition& transition, const std::string& state);

	const std::string& GetQ0() const;
	void SetQ0(const std::string& q0);

	const std::string& GetF() const;
	void SetF(const std::string& F);

private:
	std::unordered_set<std::string> Q;
	std::unordered_set<char> Sigma;
	std::unordered_map<Transition, std::vector<std::string>, Transition::Hash> Delta;
	std::string q0;
	std::string F;
};

std::ostream& operator<<(std::ostream& os, const LambdaFiniteAutomaton& automaton);
std::istream& operator>>(std::istream& is, LambdaFiniteAutomaton& automaton);
