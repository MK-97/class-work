#include "Interpreter.h"
#include "lexScanner.h"
#include "expEvaluator.h"
#include <iostream>

using namespace std;

perLineTokenVector lines;
vectOfTokenVects linesInLine;
vectOfCategoryVects catVect;
expVector inFix, postFix;

LexicalScanner interpret;
ExpressionEvaluator temp_Object;

map<string, float> doraTheExplorer;
float temp_Value;

bool evaluateExpression(int y, int & x, int & mod)
{
	//both comma and semicolon denote the end of this function
	bool Comma = false, Semicolon = false;
	while(!Comma)	
	{
		if (catVect[y][x] != SEMICOLON && catVect[y][x] != COLON)
		{
			inFix.push_back(linesInLine[y][x]);
		}
		else if (catVect[y][x] == SEMICOLON || catVect[y][x] == COMMA)
		{
			Comma = true;
			if (catVect[y][x] == SEMICOLON)
			{
				Semicolon = true;
				break;
			}
		}
		x++;
	}
	ExpressionEvaluator::infixEvaluator(inFix, doraTheExplorer, temp_Value);
	if (mod == 1)
		cout << temp_Value << " ";
	else
		mod = temp_Value;

	return Semicolon;
}


void interpretation::executeProgram(vector<string> & src)
{
	bool work = true;
	for (int x = 0; x < src.size(); x++)
	{
		lines.push_back(src[x]);
	}
	LexicalScanner::getLexicalInfo(lines, linesInLine, catVect);

	int x = 0;

	while (work)
	{
		//checking if the current line holds a semi-colon at the end
		if (lines[x][lines[x].size() - 1] != ';')
		{
			cout << "Line [" << x << "] is missing a semi-colon! ):" << endl;
			return;
		}
		//checking for assignment_op
		if (catVect[x][0] == ID_NAME && catVect[x][1] == ASSIGNMENT_OP)
		{
			assignment(x);
		}
		//checking for display keyword
		else if (lines[x][0] == 'd' && catVect[x][0] == KEYWORD)
		{
			display(lines[x],x);
		}
		//checking for read keyword
		else if (lines[x][0] == 'r' && catVect[x][0] == KEYWORD)
		{
			read(x);
		}
		//all else falls under unreadable syntax
		else
		{
			cout << "Line [" << x << "] holds invalid syntax! ):" << endl;
			return;
		}
		//moving from one line to the next
		x++;
		
		//if end of lines are reached, exit
		if (x == lines.size())
		{
			work = false;
		}
	}
	return;
}

void interpretation::display(string a, int y)
{
	int x = 0,fill = 1;
	bool comma_Needed = false;

	while (true)
	{
		/*cout << "<x is " << x << "y is " << y << ">"<< endl;*/

		//print string literal as it is
		if (catVect[y][x] == STRING_LITERAL)
		{	
			//checking for empty quotes
			if (linesInLine[y][x][0] == '\"' && linesInLine[y][x][1] == '\"')
				cout << endl;
			else
			{
				//printing all but quotes
				for (int z = 0; z < linesInLine[y][x].size(); z++)
				{
					if (linesInLine[y][x][z] == ';')
						break;
					if (linesInLine[y][x][z] == '\"')
						NULL;
					else
						cout << linesInLine[y][x][z];
				}
				cout << " ";
			}
			comma_Needed = true;
		}
		//print variable values
		else if (catVect[y][x] == ID_NAME)
		{
			//if there is a numerical_op next to the value, begin evaluating expression
			if (catVect[y][x + 1] == NUMERICAL_OP)
			{
				if (evaluateExpression(y, x, fill));
			}
			//else, print out the variable's value as normal
			else 
				cout << doraTheExplorer[linesInLine[y][x]] << " ";
			comma_Needed = true;
		}
		//if detecting a number, try to evaluate expression
		else if (catVect[y][x] == NUMERICAL_LITERAL && catVect[y][x+1] == NUMERICAL_OP)
		{
			if (evaluateExpression(y, x, fill) == true)
				return;
			comma_Needed = true;
		}
		//end of the line - return
		if (catVect[y][x] == SEMICOLON)
		{
			return;
		}
		//next token
		x++;
		//checking for comma
		if (comma_Needed && (catVect[y][x] != COMMA && catVect[y][x] != SEMICOLON))
		{
			cout << "\nMissing comma at: [" << x << "] " << lines[x] << endl;
			return;
		}
		else if (comma_Needed && catVect[y][x] == COMMA)
			comma_Needed = false;

	}
	return;
}

void interpretation::read(int y)
{
	string varName;
	int des_value;

	varName = linesInLine[y][1];

	cout << "What value would you like for variable: " << varName << "?" << endl;
	cin >> des_value;

	doraTheExplorer.insert(make_pair(varName, des_value));
}

void interpretation::assignment(int y)
{
	string varName;
	int des_value,trash, x = 2;

	varName = linesInLine[y][0];
	evaluateExpression(y, x, des_value);

	doraTheExplorer.insert(make_pair(varName, des_value));
	return;
}