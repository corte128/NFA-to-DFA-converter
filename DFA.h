/*
 * DFA.h
 *
 *  Created on: Feb 27, 2017
 *      Author: William
 */

#ifndef DFA_H_
#define DFA_H_

#include "NFA.h"
#include <vector>
#include <string>
#include <map>

using std::string;
class DFA {
public:
	DFA(NFA &input);
	virtual ~DFA();
	void printDFA();
private:
	NFA input;
	std::vector<std::vector<string> > states;
	std::vector<string> symbols;
	std::vector<string> startStates;
	std::vector<std::vector<string> > acceptStates;
	std::map<std::vector<string>, std::map<string, std::vector<string> > > transitionFunction;

	void createStates();
	void createSymbols();
	void createStartStates();
	void createAcceptStates();
	void createTransitionFunction();
	//gets the epsilon closure of the inputed state and adds it to the vector entered
	void getEpsilonClosure(std::vector<string> &container, string);
	//this function exists to check if a state is already a member of states and if not add it to states
	void addToStates(std::vector<string> &container);
};

#endif /* DFA_H_ */
