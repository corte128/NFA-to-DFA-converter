/*
 * NFA.h
 *
 *  Created on: Feb 18, 2017
 *      Author: William
 */

#ifndef NFA_H_
#define NFA_H_

#include <vector>
#include <string>
#include <map>

using std::string;
class NFA {
public:
	NFA();
	NFA(char*);
	virtual ~NFA();

	std::vector<string> getStates();
	std::vector<string> getSymbols();
	string getStartState();
	std::vector<string> getAcceptStates();
	std::vector<string> getNextState(string, string);

private:
	std::vector<string> states;
	std::vector<string> symbols;
	string startState;
	std::vector<string> acceptStates;
	//Indexes are current state and symbol entered.  The data is the next state.
	std::map<string, std::map<string, std::vector<string> > > transitionFunction;

};

#endif /* NFA_H_ */
