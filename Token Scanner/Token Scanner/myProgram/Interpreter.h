#include "lexScanner.h"
#include "expEvaluator.h"
#include <iostream>

#ifndef INTERPRETER_H
#define INTERPRETER_H


class interpretation
{
public:
	void executeProgram(vector<string> & src);

	void display(string a,int y);

	void read(int y);

	void assignment(int y);
	
	bool if_Function(int &y, int &x, bool& a);

	bool whileLoop(int &y, bool a);
	
};
#endif