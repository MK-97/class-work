#include "Interpreter.h"
#include "lexScanner.h"
#include "expEvaluator.h"
#include <iostream>

using namespace std;

perLineTokenVector lines;
vectOfTokenVects linesInLine;
vectOfCategoryVects catVect;

LexicalScanner interpret;
ExpressionEvaluator temp_Object;

map<string, float> doraTheExplorer;
float temp_Value;

bool evaluateExpression(int y, int & x, int & mod)
{
	expVector inFix;
	//both comma and semicolon denote the end of this function
	bool Comma = false, Semicolon = false;
	while (!Comma)
	{
		if (catVect[y][x] != SEMICOLON && catVect[y][x] != COLON && x != catVect[y].size() - 1)
		{
			inFix.push_back(linesInLine[y][x]);
			//cout << linesInLine[y][x];
		}
		else if (catVect[y][x] == SEMICOLON || catVect[y][x] == COMMA || x == catVect[y].size() - 1)

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
	//cout << endl;
	//for (int x = 0; x < inFix.size(); x++)
	//{
	//	cout << inFix[x];
	//	cout << "size" << inFix.size() << endl;;
	//}
	ExpressionEvaluator::infixEvaluator(inFix, doraTheExplorer, temp_Value);
	//determine if needing to print or pass temp_Value back to function
	if (mod == 1)
		cout << temp_Value << " ";
	else
	{
	/*	cout << "modding!" << endl;
		cout << temp_Value << endl;*/
		mod = temp_Value;
	}

	inFix.clear();

	return Semicolon;
}


void interpretation::executeProgram(vector<string> & src)
{
	bool work = true, while_cond = false, else_cond;
	for (int x = 0; x < src.size(); x++)
	{
		lines.push_back(src[x]);
	}
	LexicalScanner::getLexicalInfo(lines, linesInLine, catVect);

	int x = 0, else_Brace, y = 0;
	
	while (work)
	{
		//cout << "Line!" << x << endl;
		//checking if the current line holds a semi-colon at the end
		if (lines[x][0] == ' ' || lines[x].size() == 0)
		{
			//cout << "spice!" << endl;
			x++;
		}
		else if (linesInLine[x][0] == "	") {
			//cout << "tab" << endl;
			y++;
		}
		else if (lines[x][lines[x].size() - 1] != ';' &&
			(
			linesInLine[x][0] != "if" &&
			linesInLine[x][0] != "while" &&

			linesInLine[x][0] != "else" &&
			linesInLine[x][0] != "function" &&
			catVect[x][0] != LEFT_CURLYBRACE &&
			catVect[x][0] != RIGHT_CURLYBRACE
			)
			)
		{
			cout << "Line [" << x << "] is missing a semi-colon! ):" << endl;
			return;
		}


		/*cout << "a" << linesInLine[x][0] << "a" << endl;*/
		if (lines[x][0] == ' ' || lines[x].size() == 0)
			NULL;
		else if (linesInLine[x][0] == "if" && catVect[x][0] == KEYWORD && catVect[x][1] == LEFT_PARENTHESIS)
		{

			if (if_Function(x, else_Brace, else_cond))
				NULL;
			else
			{
				cout << "'if' function missing end brace! ):" << endl;
				return;
			}
		}
		else if (linesInLine[x][0] == "else" && catVect[x][0] == KEYWORD)
		{
			//	cout << "fail here" << endl;
			if (else_cond)
			{
				//	cout << "elsecond!" << endl;
				x++;
			}
			else
			{
				x = else_Brace;
			}
		}
		//checking for assignment_op
		else if (catVect[x][0] == ID_NAME && catVect[x][1] == ASSIGNMENT_OP)
		{
			assignment(x);
		}
		//checking for display keyword
		else if (linesInLine[x][y] == "display" && catVect[x][0] == KEYWORD)
		{
			//cout << "Display" << endl;
			display(lines[x], x);
		}
		//checking for read keyword
		else if (linesInLine[x][y] == "read" && catVect[x][0] == KEYWORD)
		{
			read(x);
		}
		else if (catVect[x][0] == RIGHT_CURLYBRACE)
			NULL;
		else if (catVect[x][0] == LEFT_CURLYBRACE)
			NULL;
		else if ((linesInLine[x][y] == "while" && catVect[x][0] == KEYWORD && catVect[x][1] == LEFT_PARENTHESIS) || while_cond == true)
		{
			while_cond = whileLoop(x, while_cond);
		}
		else if (linesInLine[x][y] == "function" && catVect[x][0] == KEYWORD) 
		{
			NULL;
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
	lines.clear();
	return;
}

void interpretation::display(string a, int y)
{
	//cout << "Here I am" << endl;
	int x = 0,fill = 1;
	bool comma_Needed = false;

	while (true)
	{
		//cout << "Here I am" << endl;
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
		cout << endl;
	}
	return;
}

void interpretation::read(int y)
{
	string varName;
	int des_value;

	varName = linesInLine[y][1];
	cout << endl;
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
	//cout << "hey! "<< des_value << endl;


	doraTheExplorer.erase(varName);

	doraTheExplorer.insert(make_pair(varName, des_value));

	//cout << "wanted result of "<< varName << ": " << des_value << endl;
	//cout << "finished result of " << varName <<":  "<<doraTheExplorer[varName] << endl;
	return;
}

bool interpretation::if_Function(int &y, int& else_Brace, bool& else_cond2)
{
	else_cond2 = false;
	bool else_cond = false;
	int x = 2, des_value, else_int;
	bool if_cond, right_Brace = false;

	//looks for right brace
	for (int z = y; z < catVect.size(); z++)
	{
		//cout << linesInLine[z][0] << endl;
		//cout << endl;
		if (catVect[z][0] == RIGHT_CURLYBRACE && z == catVect.size() - 1)
		{
			right_Brace = true;
			break;
		}
		//looks for else
		else if ((catVect[z][0] == KEYWORD && linesInLine[z][0] == "else") && catVect[z - 1][0] == RIGHT_CURLYBRACE)
		{
				right_Brace = true;

				else_cond = true;

				else_int = z + 2;
				for (z; z < catVect.size(); z++)
				{
					if (catVect[z][0] == RIGHT_CURLYBRACE)
					{
						else_Brace = z;
					}

				}
			break;
		}
		else if (catVect[z][0] == RIGHT_CURLYBRACE)
		{
			right_Brace = true;
			break;
		}
	}

	evaluateExpression(y, x, des_value);

	if_cond = des_value;
	//cout << "if_cond:" << if_cond << endl;
	if (if_cond)
	{
		y++;
	}
	else if (else_cond)
	{
		y = else_int;
		else_cond2 = true;
	}
	else
	{
		for (y; y < catVect.size(); y++)
		{
			if (catVect[y][0] == RIGHT_CURLYBRACE)
			{
				return true;
			}
		}
	}
	if (right_Brace)
		return true;
	else
		return false;
}

bool interpretation::whileLoop(int &y, bool a)
{
	int x = 2, des_value = 0, endofWhile;
	//statement_val retains positioning for the condition of the while loop
	static int statement_val;
	//statement_val assignment
	if (a)
		NULL;
	else
	{
		statement_val = y;
	}
	//evaluating whether condition is true
	evaluateExpression(statement_val, x, des_value);
	bool while_cond = des_value;

	//if true, remain to do the while
	if (while_cond)
	{
		y = statement_val + 1;
		//cout << "Line!" << y << endl;
		return while_cond;
	}
	else
	{
		for (int z = y; z < catVect.size(); z++)
		{
			if (catVect[z][0] == RIGHT_CURLYBRACE)
			{
				y = z - 1;
				break;
			}
		}
		return while_cond;
	}
}