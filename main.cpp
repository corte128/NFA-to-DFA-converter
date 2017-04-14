/*
 * main.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: William
 */

#include <iostream>
#include "NFA.h"
#include "DFA.h"

using std::cout;
using std::endl;
int main(int argc, char** argv)
{
	if(argc <= 1)
	{
		cout << "No Command Line Argument Entered.";
		exit(0);
	}
	NFA input = NFA(argv[1]);
	DFA output = DFA(input);
	output.printDFA();
	return 0;
}


