# NFA-to-DFA-converter
A program that takes in a Nondeterministic Finite Automaton specification in the form of a text file and outputs a Deterministic Finite Automaton specification that recognizes the same regular language.

Input Format:
  -the first line specifies all the states of the NFA seperated by tab character delimiters
  -the second line specifies the input alphabet for the recognized regular language
  -the third line specifies the start state of the NFA
  -the fourth line specifies the end states of the NFA seperated by tab character delimiters
  -all subsequent lines together define the transition function of the NFA
  
Output Format:
  -the output file follows the same format as the input file with the exception that states can now be defined by sets of identifiers rather than just single identifiers
 
 NOTE: EPS specifies an epsilon transition and EM, the empty state
