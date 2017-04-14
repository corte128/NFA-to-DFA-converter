/*
 * NFA.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: William
 */

#include "NFA.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

using std::string;
NFA::NFA()
{

}
NFA::NFA(char* fileName)
{
	std::ifstream reader(fileName);
	if(reader.is_open())
	{
		string buffer;

		//get the states as an array
		std::getline(reader, buffer);
		std::istringstream iss(buffer);
		string token;
		while(std::getline(iss, token, '\t'))//split line up by tabs
		{
			token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
			token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
			this->states.push_back(token);
		}

		//get the input symbols as an array
		std::getline(reader, buffer);
		iss.clear();
		iss.str(buffer);
		while(std::getline(iss, token, '\t'))//split line up by tabs
		{
			this->symbols.push_back(token);
		}

		//get the start state as a string
		std::getline(reader, token);
		token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
		token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
		this->startState = token;

		//get the accept states as an array
		std::getline(reader, buffer);
		iss.clear();
		iss.str(buffer);
		while(std::getline(iss, token, '\t'))//split line up by tabs
		{
			token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
			token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
			this->acceptStates.push_back(token);
		}

		int i, j;
		for(i = 0;i < states.size();++i)
		{
			transitionFunction[states[i]]["EPS"] = std::vector<string>();
			for(j = 0;j < symbols.size();++j)
			{
				transitionFunction[states[i]][symbols[j]] = std::vector<string>();
			}
		}
		//get transition function as a matrix that maps the current state and input to the next state
		while(std::getline(reader, buffer))
		{
			//remove any spaces in the line to make things simpler
			buffer.erase(std::remove(buffer.begin(), buffer.end(), ' '), buffer.end());

			iss.clear();
			iss.str(buffer);

			//get the inputed state
			std::getline(iss, token, ',');
			token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
			token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
			string curState = token;

			//get the inputed symbol
			std::getline(iss, token, '=');
			string inputSymbol = token;

			//get the output state
			std::getline(iss, token);
			token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
			token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
			string nextState = token;

			//put data into transition function matrix
			this->transitionFunction[curState][inputSymbol].push_back(nextState);

		}
		reader.close();
	}
	else std::cout << "Unable to open file." << std::endl;


}

NFA::~NFA() {
	// TODO Auto-generated destructor stub
}

std::vector<string> NFA::getStates()
{
	return states;
}

std::vector<string> NFA::getSymbols()
{
	return symbols;
}

string NFA::getStartState()
{
	return startState;
}

std::vector<string> NFA::getAcceptStates()
{
	return acceptStates;
}

std::vector<string> NFA::getNextState(string curState, string symbol)
{
	return transitionFunction[curState][symbol];
}
