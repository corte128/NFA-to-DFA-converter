/*
 * DFA.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: William
 */

#include "DFA.h"
#include "NFA.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <set>

using std::string;
using std::endl;
DFA::DFA(NFA &input)
{
	this->input = input;
	createStates();
	createSymbols();
	createStartStates();
	createAcceptStates();
	createTransitionFunction();

}

DFA::~DFA() {
	// TODO Auto-generated destructor stub
}

void DFA::printDFA()
{
	std::ofstream writer;

	writer.open("output.DFA");

	//output the states to file
	int i, j;
	for(i = 0;i < (int)states.size();++i)
	{
		writer << "{";
		for(j = 0;j < (int)states[i].size() - 1;++j)
		{
			writer << states[i][j];
			writer << ",";
		}
		writer << states[i][j];
		writer << "}\t";
	}
	writer << "\n";

	//output the symbols to file
	for(i = 0;i < (int)symbols.size();++i)
	{
		writer << symbols[i] << "\t";
	}
	writer << "\n";

	//output start state
	writer << "{";
	for(i = 0;i < (int)startStates.size();++i)
	{
		writer << startStates[i];
		if(i != (int)(startStates.size() - 1))
		{
			writer << ",";
		}
	}
	writer << "}\n";

	//outputs the accept states to file
	for(i = 0;i < (int)acceptStates.size();++i)
	{
		writer << "{";
		for(j = 0;j < (int)acceptStates[i].size() - 1;++j)
		{
			writer << acceptStates[i][j];
			writer << ",";
		}
		writer << acceptStates[i][j];
		writer << "}\t";
	}
	writer << "\n";

	//outputs the transition function to file
	int k;
	for(i = 0;i < (int)states.size();++i)
	{
		for(j = 0;j < (int)symbols.size();++j)
		{
			writer << "{";
			for(k = 0;k < (int)states[i].size() - 1;++k)
			{
				writer << states[i][k];
				writer << ",";
			}
			writer << states[i][k];
			writer << "}, ";
			writer << symbols[j] << " = {";

			for(k = 0;k < (int)transitionFunction[states[i]][symbols[j]].size() - 1;++k)
			{
				writer << transitionFunction[states[i]][symbols[j]][k];
				writer << ",";
			}
			writer << transitionFunction[states[i]][symbols[j]][k];
			writer << "}";
			writer << "\n";
		}

	}
	writer.close();
}
void DFA::createStates()
{
	std::vector<string> container = std::vector<string>();
	container.push_back("EM");
	states.push_back(container);

	int i, j, k, h;
	for(i = 0;i < (int)input.getStates().size();++i)
	{
		container.clear();

		//first add the epsilon closures of each state in the NFA as a state of the DFA
		getEpsilonClosure(container, input.getStates()[i]);
		if(!container.empty())
		{
			std::set<string> s( container.begin(), container.end() );
			container.assign( s.begin(), s.end() );
			//states.push_back(container);
			addToStates(container);
		}

		container.clear();
		//use the transition function to find the rest of the states
		for(j = 0;j < (int)input.getSymbols().size();++j)
		{
			for(k = 0;k < (int)input.getNextState(input.getStates()[i], input.getSymbols()[j]).size();++k)
			{
				getEpsilonClosure(container, input.getNextState(input.getStates()[i], input.getSymbols()[j])[k]);
			}
			if(!container.empty())
			{
				std::set<string> s( container.begin(), container.end() );
				container.assign( s.begin(), s.end() );
				//states.push_back(container);
				addToStates(container);
			}
		}
	}

	//now the states found from the NFAs transition function are used to find all the states that could be
	//reached from them given each input
	//the results are then saved as states in the DFA
	int numOfStates = states.size();
	for(i = 0;i < numOfStates;++i)
	{
		for(j = 0;j < (int)input.getSymbols().size();++j)
		{
			container.clear();
			for(k = 0;k < (int)states[i].size();++k)
			{
				for(h = 0;h < (int)input.getNextState(states[i][k], input.getSymbols()[j]).size();++h)
				{
					getEpsilonClosure(container, input.getNextState(states[i][k], input.getSymbols()[j])[h]);
				}
			}
			if(!container.empty())
			{
				std::set<string> s( container.begin(), container.end() );
				container.assign( s.begin(), s.end() );
				//states.push_back(container);
				addToStates(container);
			}
		}
	}
}

void DFA::createSymbols()
{
	symbols = input.getSymbols();
}

void DFA::createStartStates()
{
	getEpsilonClosure(startStates, input.getStartState());
}

void DFA::createAcceptStates()
{
	int i, j, k;
	bool acceptedState = false;
	for(i = 0;i < (int)states.size();++i)
	{
		for(j = 0;j < (int)states[i].size();++j)
		{
			for(k = 0;k < (int)input.getAcceptStates().size();++k)
			{
				if(acceptedState)
				{
					acceptedState = false;
					break;
				}
				if(states[i][j] == (input.getAcceptStates()[k]))
				{
					acceptStates.push_back(states[i]);
					acceptedState = true;
					break;
				}
			}
		}
	}
}

void DFA::createTransitionFunction()
{
	int i, j, k, h;
	std::vector<string> container;
	//iterates through every DFA state found in the createStates function
	for(i = 0;i < (int)states.size();++i)
	{
		//iterates through all the symbols for use in the NFA transition function with each NFA state in states[i]
		for(j = 0;j < (int)symbols.size();++j)
		{
			container = std::vector<string>();
			//every NFA state in states[i] is used in the NFA transition function to find all the NFA states
			//that can be reached and use them to construct the DFA state that populates an entry in
			//the DFA transition function
			for(k = 0;k < (int)states[i].size();++k)
			{
				//iterates through every NFA state reached with states[i][k] and symbols[j]
				//the epsilon closure of that state is added to the vector
				for(h = 0;h < (int)input.getNextState(states[i][k], symbols[j]).size();++h)
				{
					getEpsilonClosure(container, input.getNextState(states[i][k], symbols[j])[h]);
				}
			}
			//if no transitions were found for a state then the empty state is used as its next state
			if(container.empty())
			{
				container.push_back("EM");
			}
			std::set<string> s( container.begin(), container.end() );
			container.assign( s.begin(), s.end() );
			transitionFunction[states[i]][symbols[j]] = container;
		}
	}
}
void DFA::getEpsilonClosure(std::vector<string> &container, string state)
{
	bool isDuplicate = false;
	container.push_back(state);
	for(int i = 0;i < (int)input.getNextState(state, "EPS").size();++i)
	{
		if(state != input.getNextState(state, "EPS")[i])
		{
			for(int j = 0;j < (int)container.size();++j)
			{
				if(container[j] == input.getNextState(state, "EPS")[i])
				{
					isDuplicate = true;
				}
			}
			if(!isDuplicate)
			{
				getEpsilonClosure(container, input.getNextState(state, "EPS")[i]);
			}
			else
			{
				isDuplicate = false;
			}
		}
	}
}
void DFA::addToStates(std::vector<string> &container)
{
	for(int i = 0;i < (int)states.size();++i)
	{
		if(states[i] == container)
		{
			return;
		}
	}
	states.push_back(container);
}
